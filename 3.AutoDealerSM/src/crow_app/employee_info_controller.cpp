#include "crow_app/employee_info_controller.h"
#include <crow.h>
#include "models/employee_info.h"
#include "repositories/employee_info_dao.h"

EmployeeInfoController::EmployeeInfoController(EmployeeInfoDAO& dao) : employee_dao_(dao){}

// RESTful API 处理函数
crow::response EmployeeInfoController::create_employee(const crow::request& req){
    crow::json::wvalue res;
    try{
        EmployeeInfo employee;
        
        auto json = crow::json::load(req.body);
        if (!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }

        employee.set_id(json["id"].s());
        employee.set_name(json["name"].s());
        std::string gender_str = json["gender"].s();
        if(gender_str == "Male" || gender_str == "male" || gender_str == "男") {
            employee.set_gender('M');
        }else if(gender_str == "Female" || gender_str == "female" || gender_str == "女"){
            employee.set_gender('F');
        }else{
            employee.set_gender('U');   // Unknown
        }
        employee.set_email(json["email"].s());
        employee.set_phone(json["phone"].s());
        employee.set_address(json["address"].s());
        employee.set_zip_code(json["zip_code"].s());
        employee.set_group_id(json["group_id"].s());
        employee.set_monthly_working_hours(json["monthly_working_hours"].d());
        employee.set_monthly_salary(json["monthly_salary"].d());
        if (employee_dao_.insert(employee)) {
            res["result"] = "员工信息创建成功";
            return crow::response(201, res);
        } else {
            res["result"] = "员工信息创建失败";
            return crow::response(500, res);
        }

    } catch (const std::exception& e) {
        return crow::response(500, "Server Error: " + std::string(e.what()));
    }
}

crow::response EmployeeInfoController::get_employee_by_id(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        EmployeeInfo employee = employee_dao_.find_by_id(id);
        if (employee.get_id().empty()) {
            res["result"] = "未找到此员工";
            return crow::response(404, res);
        }

        res["id"] = employee.get_id();
        res["name"] = employee.get_name();
        res["gender"] = (employee.get_gender() == 'M') ? "Male" :
                         (employee.get_gender() == 'F') ? "Female" : "Unknown";
        res["email"] = employee.get_email();
        res["phone"] = employee.get_phone();
        res["address"] = employee.get_address();
        res["zip_code"] = employee.get_zip_code();
        res["group_id"] = employee.get_group_id();
        res["monthly_working_hours"] = employee.get_monthly_working_hours();
        res["monthly_salary"] = employee.get_monthly_salary();

        return crow::response(200, res);
    } catch (const std::exception& e) {
        return crow::response(500, "Error: " + std::string(e.what()));
    }
}

crow::response EmployeeInfoController::get_all_employees(const crow::request& req){
    crow::json::wvalue res;
    try {
        std::vector<EmployeeInfo> employees = employee_dao_.find_all();

        int index = 0;
        for (const auto& employee : employees) {
            res[index]["id"] = employee.get_id();
            res[index]["name"] = employee.get_name();
            res[index]["gender"] = (employee.get_gender() == 'M') ? "Male" : (employee.get_gender() == 'F') ? "Female" : "Unknown";
            res[index]["email"] = employee.get_email();
            res[index]["phone"] = employee.get_phone();
            res[index]["address"] = employee.get_address();
            res[index]["zip_code"] = employee.get_zip_code();
            res[index]["group_id"] = employee.get_group_id();
            res[index]["monthly_working_hours"] = employee.get_monthly_working_hours();
            res[index]["monthly_salary"] = employee.get_monthly_salary();
            ++index;
        }
        return crow::response(200, res);
    } catch (const std::exception& e) {
        return crow::response(500, "Error: " + std::string(e.what()));
    }
}

crow::response EmployeeInfoController::update_employee(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        auto json = crow::json::load(req.body);
        if (!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }
        EmployeeInfo employee = employee_dao_.find_by_id(id);
        if (employee.get_id().empty()) {
            res["result"] = "未找到此员工";
            return crow::response(404, res);
        }
        employee.set_name(json["name"].s());
        std::string gender_str = json["gender"].s();
        if(gender_str == "Male" || gender_str == "male" || gender_str == "男") {
            employee.set_gender('M');
        } else if(gender_str == "Female" || gender_str == "female" || gender_str == "女"){
            employee.set_gender('F');
        } else{
            employee.set_gender('U');   // Unknown
        }
        employee.set_email(json["email"].s());
        employee.set_phone(json["phone"].s());
        employee.set_address(json["address"].s());
        employee.set_zip_code(json["zip_code"].s());
        employee.set_group_id(json["group_id"].s());
        employee.set_monthly_working_hours(json["monthly_working_hours"].d());
        employee.set_monthly_salary(json["monthly_salary"].d());

        if (employee_dao_.update(employee)) {
            res["result"] = "员工信息更新成功";
            return crow::response(200, res);
        } else {
            res["result"] = "员工信息更新失败";
            return crow::response(500, res);
        }
    } catch (const std::exception& e) {
        return crow::response(500, "Error: " + std::string(e.what()));
    }
}

crow::response EmployeeInfoController::delete_employee(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try {
        if(employee_dao_.remove(id)) {
            res["result"] = "员工信息删除成功";
            return crow::response(200, res);
        } else {
            res["result"] = "员工信息删除失败";
            return crow::response(404, res);
        }
    } catch (const std::exception& e) {
        return crow::response(500, "Error: " + std::string(e.what()));
    }
}

crow::Blueprint EmployeeInfoController::get_blueprint(){
    crow::Blueprint employee_bp("api/employee");

    // POST /api/employee
    CROW_BP_ROUTE(employee_bp, "").methods("POST"_method)
    ([this](const crow::request& req) {
        return this->create_employee(req);
    });

    // GET /api/employee/<id>
    CROW_BP_ROUTE(employee_bp, "/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->get_employee_by_id(req, id);
    });

    // GET /api/employee
    CROW_BP_ROUTE(employee_bp, "").methods("GET"_method)
    ([this](const crow::request& req) {
        return this->get_all_employees(req);
    });

    // PUT /api/employee/<id>
    CROW_BP_ROUTE(employee_bp, "/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->update_employee(req, id);
    });

    // DELETE /api/employee/<id>
    CROW_BP_ROUTE(employee_bp, "/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->delete_employee(req, id);
    });

    return employee_bp;
}
