#include "crow_app/user_controller.h"
#include <crow.h>
#include "model/user_type_t.h"
#include "model/user.h"
#include "model/v_user.h"
#include "dao/user_dao.h"
#include <sstream>
#include <string>

user_type_t parse_user_type(const crow::request& req) {
    const std::string user_type_s = req.get_header_value(AUTH_USER_TYPE);
    if(user_type_s == USER_TYPE_ADMIN){
        return user_type_t::ADMIN;
    } else if(user_type_s == USER_TYPE_USER) {
        return user_type_t::USER;
    } else if(user_type_s == USER_TYPE_GUEST) {
        return user_type_t::GUEST;
    } else {
        return user_type_t::INVALID;
    }
}

// 检查是否认证的宏
#define CHECK_AUTH(req, res, res_json) \
    { \
        const std::string auth_s = (req).get_header_value(AUTH_BOOL); \
        if (auth_s != "true") { \
            (res_json)["message"] = "无权限"; \
            (res) = crow::response(403, res_json); \
            (res).end(); \
            return; \
        } \
    }

UserController::UserController(UserDao& dao) : user_dao_(dao) {}

void UserController::create_user(const crow::request& req, crow::response& res) {
    crow::json::wvalue res_json;

    try {
        auto json = crow::json::load(req.body);
        if (!json) {
            res_json["message"] = "Invalid JSON";
            res = crow::response(400, res_json);
            res.end();
            return;
        }

        if (!json.has("username") || !json.has("password") || !json.has("email")) {
            res_json["message"] = "Missing required fields";
            res = crow::response(400, res_json);
            res.end();
            return;
        }

        const std::string username = json["username"].s();
        const std::string email = json["email"].s();
        const std::string password = json["password"].s();
        
        std::string nickname = "新用户";
        if (json.has("nickname")) {
            nickname = json["nickname"].s();
        }
        
        int gender_id = 0;  // 默认未知
        if (json.has("gender_id")) {
            gender_id = json["gender_id"].i();
        } else if (json.has("gender")) {
            std::string gender_str = json["gender"].s();
            if (gender_str == "男" || gender_str == "男性" || 
                gender_str == "male" || gender_str == "Male" || gender_str == "M") {
                gender_id = 1;
            } else if (gender_str == "女" || gender_str == "女性" || 
                       gender_str == "female" || gender_str == "Female" || gender_str == "F") {
                gender_id = 2;
            }
        }

        if (user_dao_.register_user(username, password, email, gender_id, nickname)) {
            res_json["message"] = "用户注册成功";
            auto user = user_dao_.get_user_by_username(username);
            if (user) {
                res_json["user_id"] = user->get_id();
                res_json["username"] = user->get_username();
            }
            res = crow::response(201, res_json);
            res.end();
            return;
        } else {
            res_json["message"] = "用户注册失败，用户名可能已存在";
            res = crow::response(409, res_json);
            res.end();
            return;  // 409 Conflict
        }

    } catch (const std::exception& e) {
        res_json["message"] = std::string("Error: ") + e.what();
        res = crow::response(500, res_json);
        res.end();
        return;
    }
}

void UserController::get_user_by_id(const crow::request& req, crow::response& res, const int id) {
    crow::json::wvalue res_json;
    try {

        // if(
        //     [&req, &res, &res_json]() -> bool {
        //         const std::string auth_s = req.get_header_value(AUTH_BOOL);
        //         if(auth_s == "true"){
        //             return true;
        //         } else {
        //             return false;
        //         }
        //     }() == false)
        // {
        //     res_json["message"] = "无权限";         // 理论上未认证或认证失败会直接在中间件拦截
        //     res = crow::response(403, res_json);
        //     res.end();
        // }
        CHECK_AUTH(req, res, res_json);

        std::stringstream ss1;
        const int auth_user_id = [&req, &ss1]() -> int {
            const std::string auth_user_id_s = req.get_header_value(AUTH_USER_ID);
            if(auth_user_id_s.empty()){
                return -1;
            }
            try{
                const int auth_user_id = std::stoi(auth_user_id_s);
                return auth_user_id;
            } catch (const std::exception& e) {
                ss1 << "Error: " << e.what();
                return -1;
            }
        }();
        if(auth_user_id == -1){
            res_json["message"] = ss1.str();
            res = crow::response(400, res_json);
            res.end();
            return;
        }

        if(parse_user_type(req) == user_type_t::ADMIN || auth_user_id == id){
            // 如果请求方是管理员，或者请求方是目标方自己，则允许获取完整数据

            auto user = user_dao_.get_user_by_id(id);

            if (!user) {
                res_json["message"] = "用户不存在";
                res = crow::response(404, res_json);
                res.end();
                return;
            }

            res_json["user_id"] = user->get_id();
            res_json["username"] = user->get_username();
            res_json["nickname"] = user->get_nickname();
            res_json["email"] = user->get_email();
            res_json["gender"] = [](const int gender_id) -> std::string {
                switch (gender_id) {
                    case 1: return "男";
                    case 2: return "女";
                    default: return "未知";
                }
            }(user->get_gender_id());

            res = crow::response(200, res_json);
            res.end();
            return;

        } else {
            // 否则返回无权限
            res_json["message"] = "无权限";
            res = crow::response(403, res_json);
            res.end();
            return;    // 403 Forbidden 无权限
        }
    } catch (const std::invalid_argument& e) {
        res_json["message"] = "无效的用户ID格式";
        res = crow::response(400, res_json);
        res.end();
        return;
    } catch (const std::exception& e) {
        std::stringstream ss2;
        ss2 << "Error: " << e.what();
        res_json["message"] = ss2.str();
        res = crow::response(500, res_json);
        res.end();
        return;
    }
}

void UserController::get_user_by_username(const crow::request& req, crow::response& res, const std::string& username) {
    crow::json::wvalue res_json;
    try {
        // if(
        //     [&req, &res, &res_json]() -> bool {
        //         const std::string auth_s = req.get_header_value(AUTH_BOOL);
        //         if(auth_s == "true"){
        //             return true;
        //         } else {
        //             return false;
        //         }
        //     }() == false)
        // {
        //     res_json["message"] = "无权限";         // 理论上未认证或认证失败会直接在中间件拦截
        //     res = crow::response(403, res_json);
        //     res.end();
        // }
        CHECK_AUTH(req, res, res_json);

        const std::string auth_username = [&req](UserDao& user_dao) -> std::string {
            const std::string auth_user_id_s = req.get_header_value(AUTH_USER_ID);
            if(auth_user_id_s.empty()){
                return "";
            }
            return user_dao.get_username_by_id(std::stoi(auth_user_id_s));
        }(this->user_dao_);

        if(parse_user_type(req) == user_type_t::ADMIN || auth_username == username){
            // 如果请求方是管理员，或者请求方是目标方自己，则允许获取完整数据
            auto user = user_dao_.get_user_by_username(username);
            
            if (!user) {
                res_json["message"] = "用户不存在";
                res = crow::response(404, res_json);
                res.end();
                return;
            }

            res_json["user_id"] = user->get_id();
            res_json["username"] = user->get_username();
            res_json["nickname"] = user->get_nickname();
            res_json["email"] = user->get_email();
            res_json["gender"] = [](const int gender_id) -> std::string {
                switch (gender_id) {
                    case 1: return "男";
                    case 2: return "女";
                    default: return "未知";
                }
            }(user->get_gender_id());

            res = crow::response(200, res_json);
            res.end();
            return;

        } else {
            // 否则返回无权限
            res_json["message"] = "无权限";
            res = crow::response(403, res_json);
            res.end();
            return;    // 403 Forbidden 无权限
        }

    } catch (const std::exception& e) {
        res_json["message"] = std::string("Error: ") + e.what();
        res = crow::response(500, res_json);
        res.end();
        return;
    }
}

void UserController::get_all_users(const crow::request& req, crow::response& res) {
    crow::json::wvalue res_json;
    try {
        // if(
        //     [&req, &res, &res_json]() -> bool {
        //         const std::string auth_s = req.get_header_value(AUTH_BOOL);
        //         if(auth_s == "true"){
        //             return true;
        //         } else {
        //             return false;
        //         }
        //     }() == false)
        // {
        //     res_json["message"] = "无权限";         // 理论上未认证或认证失败会直接在中间件拦截
        //     res = crow::response(403, res_json);
        //     res.end();
        // }
        CHECK_AUTH(req, res, res_json);

        if(parse_user_type(req) == user_type_t::ADMIN){
            // 如果请求方是管理员，则允许获取所有数据

            auto vusers = user_dao_.get_all_vusers();
            
            std::vector<crow::json::wvalue> users_json;
            for (const auto& vuser : vusers) {
                crow::json::wvalue user_json;
                user_json["id"] = vuser.get_id();
                user_json["username"] = vuser.get_username();
                user_json["nickname"] = vuser.get_nickname();
                user_json["email"] = vuser.get_email();
                user_json["sex"] = vuser.get_sex();
                users_json.push_back(user_json);
            }
            
            res_json["users"] = std::move(users_json);
            res_json["count"] = vusers.size();
            
            res = crow::response(200, res_json);
            res.end();
            return;

        } else {
            // 否则返回无权限
            res_json["message"] = "无权限";
            res = crow::response(403, res_json);
            res.end();
            return;    // 403 Forbidden 无权限
        }

    } catch (const std::exception& e) {
        res_json["message"] = std::string("Error: ") + e.what();
        res = crow::response(500, res_json);
        res.end();
        return;
    }
}

void UserController::update_user(const crow::request& req, crow::response& res, const int id) {
    crow::json::wvalue res_json;
    try {
        // if(
        //     [&req, &res, &res_json]() -> bool {
        //         const std::string auth_s = req.get_header_value(AUTH_BOOL);
        //         if(auth_s == "true"){
        //             return true;
        //         } else {
        //             return false;
        //         }
        //     }() == false)
        // {
        //     res_json["message"] = "无权限";         // 理论上未认证或认证失败会直接在中间件拦截
        //     res = crow::response(403, res_json);
        //     res.end();
        // }
        CHECK_AUTH(req, res, res_json);

        const int auth_user_id = [&req]() -> int {
            const std::string auth_user_id_s = req.get_header_value(AUTH_USER_ID);
            if(auth_user_id_s.empty()){
                return -1;
            }
            return std::stoi(auth_user_id_s);
        }();

        if(parse_user_type(req) == user_type_t::ADMIN || auth_user_id == id){
            // 如果请求方是管理员，或者请求方是目标方自己，则允许更新用户数据

            auto user = user_dao_.get_user_by_id(id);
            
            if (!user) {
                res_json["message"] = "用户不存在";
                res = crow::response(404, res_json);
                res.end();
                return;
            }

            auto json = crow::json::load(req.body);
            if (!json) {
                res_json["message"] = "Invalid JSON";
                res = crow::response(400, res_json);
                res.end();
                return;
            }

            // 更新用户信息
            if (json.has("username")) {
                std::string new_username = json["username"].s();
                // 检查新用户名是否已存在（除了当前用户）
                if (new_username != user->get_username() && user_dao_.username_exists(new_username)) {
                    res_json["message"] = "用户名已存在";
                    res = crow::response(409, res_json);
                    res.end();
                    return;
                }
                user->set_username(new_username);
            }
        
            if (json.has("nickname")) {
                user->set_nickname(json["nickname"].s());
            }
            
            if (json.has("email")) {
                user->set_email(json["email"].s());
            }
            
            const int gender_id = [](const std::string gender_str) -> int {
                if(gender_str == "男" || gender_str == "男性" || gender_str.at(0) == 'M' || gender_str.at(0) == 'm'){
                    return 1;   //Male
                } else if (gender_str == "女" || gender_str == "女性" || gender_str.at(0) == 'F' || gender_str.at(0) == 'f') {
                    return 2;   //Female
                } else {    // 设置为 “未知” 性别
                    return 0;
                }
            }(json["gender"].s());
            user->set_gender_id(gender_id);

            if (user_dao_.update_user(*user)) {
                res_json["message"] = "用户信息更新成功";
                res_json["user_id"] = user->get_id();
                res = crow::response(200, res_json);
                res.end();
                return;
            } else {
                res_json["message"] = "用户信息更新失败";
                res = crow::response(500, res_json);
                res.end();
                return;
            }

        } else {
            // 否则返回无权限
            res_json["message"] = "无权限";
            res = crow::response(403, res_json);
            res.end();
            return;    // 403 Forbidden 无权限
        }

    } catch (const std::invalid_argument& e) {
        res_json["message"] = "无效的用户ID格式";
        res = crow::response(400, res_json);
        res.end();
        return;
    } catch (const std::exception& e) {
        res_json["message"] = std::string("Error: ") + e.what();
        res = crow::response(500, res_json);
        res.end();
        return;
    }
}

void UserController::delete_user(const crow::request& req, crow::response& res, const int id) {
    crow::json::wvalue res_json;
    try {
        // if(
        //     [&req, &res, &res_json]() -> bool {
        //         const std::string auth_s = req.get_header_value(AUTH_BOOL);
        //         if(auth_s == "true"){
        //             return true;
        //         } else {
        //             return false;
        //         }
        //     }() == false)
        // {
        //     res_json["message"] = "无权限";         // 理论上未认证或认证失败会直接在中间件拦截
        //     res = crow::response(403, res_json);
        //     res.end();
        // }
        CHECK_AUTH(req, res, res_json);

        const int auth_user_id = [&req]() -> int {
            const std::string auth_user_id_s = req.get_header_value(AUTH_USER_ID);
            if(auth_user_id_s.empty()){
                return -1;
            }
            return std::stoi(auth_user_id_s);
        }();

        if(parse_user_type(req) == user_type_t::ADMIN || auth_user_id == id){
            // 如果请求方是管理员，或者请求方是目标方自己，则允许删除用户数据
        
            // 检查用户是否存在
            auto user = user_dao_.get_user_by_id(id);
            if (!user) {
                res_json["message"] = "用户不存在";
                res = crow::response(404, res_json);
                res.end();
                return;
            }

            // 防止删除root用户（ID为1的管理员）
            if (id == 1) {
                res_json["message"] = "不能删除管理员账户";
                res = crow::response(403, res_json);
                res.end();
                return;
            }

            if (user_dao_.delete_user(id)) {
                res_json["message"] = "用户删除成功";
                res_json["deleted_id"] = id;
                res = crow::response(200, res_json);
                res.end();
                return;
            } else {
                res_json["message"] = "用户删除失败";
                res = crow::response(500, res_json);
                res.end();
                return;
            }

        } else {
            // 否则返回无权限
            res_json["message"] = "无权限";
            res = crow::response(403, res_json);
            res.end();
            return;    // 403 Forbidden 无权限
        }

    } catch (const std::invalid_argument& e) {
        res_json["message"] = "无效的用户ID格式";
        res = crow::response(400, res_json);
        res.end();
        return;
    } catch (const std::exception& e) {
        res_json["message"] = std::string("Error: ") + e.what();
        res = crow::response(500, res_json);
        res.end();
        return;
    }
}

void UserController::bp_route(crow::Blueprint& user_blueprint) {
    // POST /api/users/  - 用户注册
    CROW_BP_ROUTE(user_blueprint, "/").methods("POST"_method)
    ([this](const crow::request& req, crow::response& res) {
        return this->create_user(req, res);
    });

    // GET /api/users/<id>  - 根据 ID 获取用户
    CROW_BP_ROUTE(user_blueprint, "/<int>").methods("GET"_method)
    ([this](const crow::request& req, crow::response& res, int id) {
        return this->get_user_by_id(req, res, id);
    });

    // GET /api/users/?username=<username>  - 根据 username 获取用户
    // GET /api/users/                      - 获取所有用户（视图）
    CROW_BP_ROUTE(user_blueprint, "/").methods("GET"_method)
    ([this](const crow::request& req, crow::response& res) {
        const std::string username = req.url_params.get("username"); // 获取查询参数
        
        if (username.empty()) { // 若没有提供 username 参数，返回所有用户
            return this->get_all_users(req, res);
        } else { // 获取目标用户
            return this->get_user_by_username(req, res, username);
        }
    });

    // PUT /api/users/<id>  - 更新用户信息
    CROW_BP_ROUTE(user_blueprint, "/<int>").methods("PUT"_method)
    ([this](const crow::request& req, crow::response& res, int id) {
        return this->update_user(req, res, id);
    });

    // DELETE /api/users/<id>   - 删除用户
    CROW_BP_ROUTE(user_blueprint, "/<int>").methods("DELETE"_method)
    ([this](const crow::request& req, crow::response& res, int id) {
        return this->delete_user(req, res, id);
    });

}
