#ifndef EMPLOYEE_INFO_CONTROLLER_H
#define EMPLOYEE_INFO_CONTROLLER_H

#include <crow.h>
#include "models/employee_info.h"
#include "repositories/employee_info_dao.h"

class EmployeeInfoController {
private:
    EmployeeInfoDAO& employee_dao_;
public:
    explicit EmployeeInfoController(EmployeeInfoDAO& dao);

    // RESTful API 处理函数
    crow::response create_employee(const crow::request& req);
    crow::response get_employee_by_id(const crow::request& req, const std::string& id);
    crow::response get_all_employees(const crow::request& req);
    crow::response update_employee(const crow::request& req, const std::string& id);
    crow::response delete_employee(const crow::request& req, const std::string& id);
    
    crow::Blueprint get_blueprint();
};


#endif // EMPLOYEE_INFO_CONTROLLER_H
