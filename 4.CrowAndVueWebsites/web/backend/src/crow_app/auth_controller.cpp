#include "crow_app/auth_controller.h"
#include <crow.h>
#include "model/user.h"
#include "model/v_user.h"
#include "dao/user_dao.h"
#include <sstream>
#include <memory>
#include "util/session_manager.h"
#include "model/user_type_t.h"
#include "util/time_id_generator.h"

AuthController::AuthController(UserDao& dao) : user_dao_(dao) {}

void AuthController::login(const crow::request& req, crow::response& res) {
    crow::json::wvalue res_json;
    try {
        auto json = crow::json::load(req.body);
        if (!json) {
            res_json["message"] = "Invalid JSON";
            res = crow::response(400, res_json);
            res.end();
            return;
        }

        const std::string password = [&json]() -> std::string {
            if (!json.has("password")) {
                return "";
            } else {
                return json["password"].s();
            }
        }();
        if(password.empty()){
            res_json["message"] = "缺少密码";
            res = crow::response(400, res_json);
            res.end();
            return;
        }

        std::string username_or_email;
        const int8_t option = [&json](std::string& username_or_email) -> int8_t {
            if (json.has("username")) { // 1. 优先选择 username
                username_or_email = json["username"].s();
                return 1;
            } else if(json.has("email")) {  // 2. 若没有 username , 则使用 email
                username_or_email = json["email"].s();
                return 2;
            } else {    // 3. 既没有 username 也没有 email
                username_or_email = "";
                return -1;
            }
        }(username_or_email);

        std::unique_ptr<User> user;
        switch(option){
            case 1:{
                user = user_dao_.validate_by_username(username_or_email, password);
            } break;
            case 2:{
                user = user_dao_.validate_by_email(username_or_email, password);
            } break;
            default:{
                res_json["message"] = "缺少用户名/邮箱";
                res = crow::response(400, res_json);
                res.end();
                return;
            }
        }
        
        if (user) {
            const std::string session_id = global_session_manager.create(user->get_id(), [](const int user_id) -> user_type_t {
                if(user_id == 1){   // root 用户
                    return user_type_t::ADMIN;
                } else {
                    return user_type_t::USER;
                }
            }(user->get_id()));  // 默认创建的用户类型都属 "USER"
            if(!session_id.empty()){
                res_json["message"] = "登录成功";
                res_json["session_id"] = session_id;
                res_json["auth_method"] = [](const uint8_t opt) -> std::string { // 登录方式
                    switch(opt) {
                        case 1: return "username";
                        case 2: return "email";
                        default: return "invalid";
                    }
                }(option);
                res_json["timestamp"] = []() -> long {
                    TimeIdGenerator generator;
                    return generator.get_current_timestamp();
                }();
                res_json["user_id"] = user->get_id();
                res_json["username"] = user->get_username();
                res_json["nickname"] = user->get_nickname();
                res_json["email"] = user->get_email();
                res_json["gender"] = [](const int gender_id) -> std::string {
                    switch(gender_id) {
                        case 1: return "男";
                        case 2: return "女";
                        default: return "未知";
                    }
                }(user->get_gender_id());
                
                res = crow::response(200, res_json);
                res.end();
                return;
            } else {
                res_json["message"] = "系统错误，创建会话失败";
                res = crow::response(500, res_json);
                res.end();
                return;
            }
        } else {
            res_json["message"] = [](const uint8_t opt) -> std::string { // 登录方式
                switch(opt) {
                    case 1: return "用户名或密码错误";
                    case 2: return "邮箱或密码错误";
                    default: return "无效的登录请求";
                }
            }(option);
            res = crow::response(401, res_json);
            res.end();
            return;  // 401 Unauthorized
        }

    } catch (const std::exception& e) {
        res_json["message"] = std::string("Error: ") + e.what();
        res = crow::response(500, res_json);
        res.end();
        return;
    }
}

void AuthController::logout(const crow::request& req, crow::response& res) {
    const std::string session_id = req.get_header_value("session_id");
    global_session_manager.remove(session_id);
    res = crow::response(204);
    res.end();
    return;
}

void AuthController::test([[maybe_unused]] const crow::request& req, crow::response& res){
    res = crow::response(204);
    res.end();
    return;
}

void AuthController::bp_route(crow::Blueprint& auth_blueprint) {
    // POST /api/auth/       - 用户登录
    CROW_BP_ROUTE(auth_blueprint, "/").methods("POST"_method)
    ([this](const crow::request& req, crow::response& res) {
        return this->login(req, res);
    });

    // DELETE /api/auth/     - 用户删除
    CROW_BP_ROUTE(auth_blueprint, "/").methods("DELETE"_method)
    ([this](const crow::request& req, crow::response& res) {
        return this->logout(req, res);
    });

    // GET /api/auth/ - 用户状态
    CROW_BP_ROUTE(auth_blueprint, "/").methods("GET"_method)
    ([this](const crow::request& req, crow::response& res) {
        return this->test(req, res);
    });

}

