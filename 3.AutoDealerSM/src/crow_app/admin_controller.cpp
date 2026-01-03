#include "crow_app/admin_controller.h"
#include "main.h"
#include "utils/input_validator.h"
#include "utils/password_tool.h"
#include "repositories/admin_info_dao.h"
#include "utils/session_manager.h"

extern SessionManager global_session_manager;

AdminController::AdminController(AdminInfoDAO& dao) : admin_dao_(dao) {}

// RESTful API 处理函数
crow::response AdminController::create_admin(const crow::request& req) {
    crow::json::wvalue res;
    try {
        AdminInfo admin;

        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON";
            return crow::response(400, res);
        }

        std::string admin_id = json["id"].s();
        if(admin_dao_.id_exists(admin_id)) {
            res["result"] = "此ID已存在，请选择其他ID";
            return crow::response(400, res);
        }
        admin.set_id(admin_id);

        std::string passwd_plaintext = json["password"].s();
        if(!InputValidator::validate_password(passwd_plaintext)) {
            res["result"] = "密码不符合要求，需要6-18位，且包含至少两种字符类型";
            return crow::response(400, res);
        }

        auto result = PasswordTool::generate_salt_and_hash(passwd_plaintext);
        if(result.has_value()){
            auto [salt, hash] = result.value();
            admin.set_salt(salt);
            admin.set_pwd(hash);
        } else {
            res["result"] = "密码处理失败";
            return crow::response(500, res);
        }

        admin.set_name(json["name"].s());
        if(admin_dao_.insert(admin)){
            res["result"] = "管理员账号创建成功";
            return crow::response(201, res);
        } else {
            res["result"] = "管理员账号创建失败";
            return crow::response(500, res);
        }

    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response AdminController::get_admin_by_id(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try {
        AdminInfo admin = admin_dao_.find_by_id(id);
        if(admin.get_id().empty()) {
            res["result"] = "未找到此管理员";
            return crow::response(404, res);
        }

        res["id"] = admin.get_id();
        res["name"] = admin.get_name();
        return crow::response(200, res.dump());

    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response AdminController::get_all_admin(const crow::request& req){
    crow::json::wvalue res;
    try {
        std::vector<AdminInfo> admins = admin_dao_.find_all();
        int index = 0;
        for (const auto& admin : admins) {
            res[index]["id"] = admin.get_id();
            res[index]["name"] = admin.get_name();
            index++;
        }
        
        return crow::response(200, res);
    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response AdminController::update_admin(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try {
        auto json = crow::json::load(req.body);
        if (!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }
        
        AdminInfo admin;
        admin.set_id(id);
        admin.set_name(json["name"].s());

        std::string passwd_plaintext_old = json["old_password"].s();
        std::string passwd_plaintext_new = json["new_password"].s();

        if(!InputValidator::validate_password(passwd_plaintext_new)) {
            res["result"] = "新密码不符合要求，需要6-18位，且包含至少两种字符类型";
            return crow::response(400, res);
        }

        AdminInfo existing_admin = admin_dao_.find_by_id(id);
        if(existing_admin.get_id().empty()) {
            res["result"] = "未找到此管理员";
            return crow::response(404, res);
        } else {    // 验证密码是否正确
            if(!PasswordTool::verify_password(passwd_plaintext_old, existing_admin.get_pwd(), existing_admin.get_salt())){
                res["result"] = "密码不正确";
                return crow::response(403, res);
            }
        }

        auto result = PasswordTool::generate_salt_and_hash(passwd_plaintext_new);
        if(result.has_value()){
            auto [salt, hash] = result.value();
            admin.set_salt(salt);
            admin.set_pwd(hash);
        } else {
            res["result"] = "密码处理失败";
            return crow::response(500, res);
        }

        if (admin_dao_.update(admin)) {
            res["result"] = "管理员信息更新成功";
            return crow::response(200, res);
        } else {
            res["result"] = "管理员信息更新失败";
            return crow::response(404, res);
        }
    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response AdminController::delete_admin(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try {
        if (admin_dao_.remove(id)) {
            res["result"] = "管理员账号删除成功";
            return crow::response(200, res);
        } else {
            res["result"] = "未找到此管理员，删除失败";
            return crow::response(404, res);
        }
    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response AdminController::login(const crow::request& req){
    crow::json::wvalue res;
    try{
        // 1. 验证用户和密码是否正确
        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON";
            return crow::response(400, res);
        }

        // 提取ID和密码
        const std::string admin_id = json["id"].s();
        const std::string passwd_plaintext = json["password"].s();
        if(admin_id.empty() || passwd_plaintext.empty()){
            res["result"] = "ID和密码不能为空";
            return crow::response(400, res);
        }
        
        AdminInfo existing_admin = admin_dao_.find_by_id(admin_id);
        if(existing_admin.get_id().empty()) {
            res["result"] = "未找到此管理员";
            return crow::response(404, res);
        } else {    // 验证密码是否正确
            if(!PasswordTool::verify_password(passwd_plaintext, existing_admin.get_pwd(), existing_admin.get_salt())){
                res["result"] = "密码不正确";
                return crow::response(403, res);
            }
        }

        // 2. 若正确创建会话，并返回会话 ID
        const std::string session_id = global_session_manager.create(admin_id, USER_TYPE_ADMIN);
        if(session_id.empty()){
            res["result"] = "会话创建失败";
            return crow::response(500, res);
        }

        res["session_id"] = session_id;
        res["id"] = existing_admin.get_id();
        res["name"] = existing_admin.get_name();
        res["user_type"] = USER_TYPE_ADMIN;
        res["result"] = "登录成功";
        return crow::response(200, res);
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::Blueprint AdminController::get_blueprint(){
    crow::Blueprint admin_bp("api/admin");

    // POST /api/admin/  创建管理员
    CROW_BP_ROUTE(admin_bp, "").methods("POST"_method)
    ([this](const crow::request& req) {
        return this->create_admin(req);
    });

    // GET /api/admin/<string>  根据ID获取管理员信息
    CROW_BP_ROUTE(admin_bp, "/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->get_admin_by_id(req, id);
    });

    // GET /api/admin/  获取所有管理员信息
    CROW_BP_ROUTE(admin_bp, "").methods("GET"_method)
    ([this](const crow::request& req) {
        return this->get_all_admin(req);
    });

    // PUT /api/admin/<string>  更新管理员信息
    CROW_BP_ROUTE(admin_bp, "/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->update_admin(req, id);
    });

    // DELETE /api/admin/<string>  删除管理员信息
    CROW_BP_ROUTE(admin_bp, "/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->delete_admin(req, id);
    });

    // POST /api/admin/auth  - 管理员用户登录
    CROW_BP_ROUTE(admin_bp, "/login").methods("POST"_method)
    ([this](const crow::request& req) {
        try{
            return this->login(req);
        }catch (const std::exception& e){
            throw crow::response(500, std::string("Error: ") + e.what());
        }
    });

    return admin_bp; // 返回蓝图对象
}
