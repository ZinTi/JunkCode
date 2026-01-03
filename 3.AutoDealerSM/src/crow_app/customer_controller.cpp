#include "crow_app/customer_controller.h"
#include "main.h"
#include "utils/input_validator.h"
#include "utils/password_tool.h"
#include "repositories/customer_info_dao.h"
#include "utils/session_manager.h"

extern SessionManager global_session_manager;

CustomerController::CustomerController(CustomerInfoDAO& dao) : customer_dao_(dao){}

// RESTful API 处理函数
crow::response CustomerController::create_customer(const crow::request& req){
    crow::json::wvalue res;
    try{
        CustomerInfo customer;

        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }

        std::string customer_id = json["id"].s();
        if(customer_dao_.id_exists(customer_id)) {
            res["result"] = "此ID已存在，请选择其他ID";
            return crow::response(400, res);
        }
        customer.set_id(customer_id);

        customer.set_name(json["name"].s());
        std::string gender_str = json["gender"].s();
        if(gender_str == "Male" || gender_str == "male" || gender_str == "男"){
            customer.set_gender('M');
        }else if(gender_str == "Female" || gender_str == "female" || gender_str == "女"){
            customer.set_gender('F');
        }else{
            customer.set_gender('U');   // Unknown
        }

        std::string passwd_plaintext = json["password"].s();
        if(!InputValidator::validate_password(passwd_plaintext)) {
            res["result"] = "密码不符合要求，需要6-18位，且包含至少两种字符类型";
            return crow::response(400, res);
        }

        auto result = PasswordTool::generate_salt_and_hash(passwd_plaintext);
        if(result.has_value()){
            auto [salt, hash] = result.value();
            customer.set_salt(salt);
            customer.set_pwd(hash);
        } else {
            res["result"] = "密码处理失败";
            return crow::response(500, res);
        }

        customer.set_email(json["email"].s());
        if(!InputValidator::validate_email(customer.get_email())){
            res["result"] = "电子邮箱格式不合法";
            return crow::response(400, res);
        }

        customer.set_phone(json["phone"].s());
        if(!InputValidator::validate_phone_number(customer.get_phone())){
            res["result"] = "电话号码格式不合法";
            return crow::response(400, res);
        }

        customer.set_address(json["address"].s());
        customer.set_zip_code(json["zip_code"].s());
        customer.set_car_model(json["car_model"].s());
        customer.set_car_condition(json["car_condition"].s());
        customer.set_car_id(json["car_id"].s());

        if(customer_dao_.insert(customer)){
            res["result"] = "客户信息创建成功";
            return crow::response(201, res);
        } else {
            res["result"] = "客户信息创建失败";
            return crow::response(500, res);
        }

    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerController::get_customer_by_id(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try {
        CustomerInfo customer = customer_dao_.find_by_id(id);
        if(customer.get_id().empty()) {
            res["result"] = "未找到此客户";
            return crow::response(404, res);
        }

        res["id"] = customer.get_id();
        res["name"] = customer.get_name();
        char gender_ch = customer.get_gender();
        if(gender_ch == 'M'){
            res["gender"] = "Male";
        }else if(gender_ch == 'F'){
            res["gender"] = "Female";
        }else{
            res["gender"] = "Unknown";
        }
        res["email"] = customer.get_email();
        res["phone"] = customer.get_phone();
        res["address"] = customer.get_address();
        res["zip_code"] = customer.get_zip_code();
        res["car_model"] = customer.get_car_model();
        res["car_condition"] = customer.get_car_condition();
        res["car_id"] = customer.get_car_id();

        return crow::response(200, res);
    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerController::get_all_customers(const crow::request& req){
    crow::json::wvalue res;
    try {
        std::vector<CustomerInfo> customers = customer_dao_.find_all();
        crow::json::wvalue result;
        
        int index = 0;
        for (const auto& customer : customers) {
            result[index]["id"] = customer.get_id();
            result[index]["name"] = customer.get_name();
            char gender_ch = customer.get_gender();
            if(gender_ch == 'M'){
                result[index]["gender"] = "Male";
            }else if(gender_ch == 'F'){
                result[index]["gender"] = "Female";
            }else{
                result[index]["gender"] = "Unknown";
            }
            index++;
        }
        return crow::response(200, result);
    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerController::update_customer(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try {
        auto json = crow::json::load(req.body);
        if (!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }
        
        CustomerInfo customer;
        customer.set_id(id);
        customer.set_name(json["name"].s());
        std::string gender_str = json["gender"].s();
        if(gender_str == "Male" || gender_str == "male" || gender_str == "男"){
            customer.set_gender('M');
        }else if(gender_str == "Female" || gender_str == "female" || gender_str == "女"){
            customer.set_gender('F');
        }else{
            customer.set_gender('U');   // Unknown
        }
        customer.set_email(json["email"].s());
        if(!InputValidator::validate_email(customer.get_email())){
            return crow::response(400, "电子邮箱格式不合法");
        }
        customer.set_phone(json["phone"].s());
        if(!InputValidator::validate_phone_number(customer.get_phone())){
            return crow::response(400, "电话号码格式不合法");
        }
        customer.set_address(json["address"].s());
        customer.set_zip_code(json["zip_code"].s());
        customer.set_car_model(json["car_model"].s());
        customer.set_car_condition(json["car_condition"].s());
        customer.set_car_id(json["car_id"].s());
        if (customer_dao_.update(customer)) {
            return crow::response(200, "客户信息更新成功");
        } else {
            return crow::response(404, "客户信息更新失败");
        }
    } catch (const std::exception& e) {
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerController::delete_customer(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        if(customer_dao_.remove(id)){
            return crow::response(200, "客户信息删除成功");
        } else {
            return crow::response(404, "未找到此客户，删除失败");
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerController::login(const crow::request& req){
    crow::json::wvalue res;
    try{
        // 1. 验证用户和密码是否正确
        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON"; 
            return crow::response(400, res);
        }

        // 提取ID和密码
        const std::string cust_id = json["id"].s();
        const std::string passwd_plaintext = json["password"].s();
        if(cust_id.empty() || passwd_plaintext.empty()){
            res["result"] = "ID和密码不能为空";
            return crow::response(400, res);
        }

        CustomerInfo existing_cust = customer_dao_.find_by_id(cust_id);
        if(existing_cust.get_id().empty()) {
            res["result"] = "未找到此客户";
            return crow::response(404, res);
        } else {    // 验证密码是否正确
            if(!PasswordTool::verify_password(passwd_plaintext, existing_cust.get_pwd(), existing_cust.get_salt())){
                res["result"] = "密码不正确";
                return crow::response(403, res);
            }
        }

        // 2. 若正确创建会话，并返回会话 ID
        const std::string session_id = global_session_manager.create(cust_id, USER_TYPE_CUSTOMER);
        if(session_id.empty()){
            res["result"] = "会话创建失败";
            return crow::response(500, res);
        }
        res["session_id"] = session_id;
        res["id"] = existing_cust.get_id();
        res["name"] = existing_cust.get_name();
        res["user_type"] = USER_TYPE_CUSTOMER;
        res["result"] = "登录成功";
        return crow::response(200, res);
    }catch (const std::exception& e){
        crow::json::wvalue error_res;
        error_res["result"] = std::string("Error: ") + e.what();
        return crow::response(500, error_res);
    }
}

// 获取配置好路由的 Blueprint
crow::Blueprint CustomerController::get_blueprint(){
    crow::Blueprint customer_bp("api/customer");

    // POST /api/customer/ - 创建新客户
    CROW_BP_ROUTE(customer_bp, "").methods("POST"_method)
    ([this](const crow::request& req){
        return this->create_customer(req);
    });

    // GET /api/customer/<id> - 根据ID获取客户信息
    CROW_BP_ROUTE(customer_bp, "/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id){
        return this->get_customer_by_id(req, id);
    });

    // GET /api/customer/ - 获取所有客户信息
    CROW_BP_ROUTE(customer_bp, "").methods("GET"_method)
    ([this](const crow::request& req){
        return this->get_all_customers(req);
    });

    // PUT /api/customer/<id> - 更新客户信息
    CROW_BP_ROUTE(customer_bp, "/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id){
        return this->update_customer(req, id);
    });

    // DELETE /api/customer/<id> - 删除客户信息
    CROW_BP_ROUTE(customer_bp, "/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id){
        return this->delete_customer(req, id);
    });

    // POST /api/customer/login - 客户登录
    CROW_BP_ROUTE(customer_bp, "/login").methods("POST"_method)
    ([this](const crow::request& req){
        return this->login(req);
    });

    return customer_bp;
}
