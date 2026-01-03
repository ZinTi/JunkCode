/**
 * @file employee_info_dao.cpp
 * @brief 职员信息DAO实现
 */

#include "repositories/employee_info_dao.h"
#include <iostream>
#include <cstring>

extern "C" {
    #include <sqlite3.h>
}

// 查询回调函数
static int employee_info_callback(void* data, int argc, char** argv, char** col_names) {
    auto* employees = static_cast<std::vector<EmployeeInfo>*>(data);

    if (argc == 10) { // 所有字段
        char gender = (argv[2] && strlen(argv[2]) > 0) ? argv[2][0] : 'M';
        double monthly_working_hours = argv[8] ? std::stod(argv[8]) : 0.0;
        double monthly_salary = argv[9] ? std::stod(argv[9]) : 0.0;

        EmployeeInfo employee(argv[0] ? argv[0] : "",
                             argv[1] ? argv[1] : "",
                             gender,
                             argv[3] ? argv[3] : "",
                             argv[4] ? argv[4] : "",
                             argv[5] ? argv[5] : "",
                             argv[6] ? argv[6] : "",
                             argv[7] ? argv[7] : "",
                             monthly_working_hours,
                             monthly_salary);
        employees->push_back(employee);
    }

    return 0;
}

bool EmployeeInfoDAO::insert(const EmployeeInfo& employee) {
    std::string sql = "INSERT INTO employee_info (id, name, gender, email, phone, address, zip_code, group_id, monthly_working_hours, monthly_salary) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        employee.get_id(),
        employee.get_name(),
        std::string(1, employee.get_gender()),
        employee.get_email(),
        employee.get_phone(),
        employee.get_address(),
        employee.get_zip_code(),
        employee.get_group_id(),
        std::to_string(employee.get_monthly_working_hours()),
        std::to_string(employee.get_monthly_salary())
    };

    return execute_update_with_params(sql, params);
}

bool EmployeeInfoDAO::remove(const std::string& id) {
    std::string sql = "DELETE FROM employee_info WHERE id = ?";
    std::vector<std::string> params = {id};

    return execute_update_with_params(sql, params);
}

bool EmployeeInfoDAO::update(const EmployeeInfo& employee) {
    std::string sql = "UPDATE employee_info SET name = ?, gender = ?, email = ?, phone = ?, address = ?, zip_code = ?, group_id = ?, monthly_working_hours = ?, monthly_salary = ? WHERE id = ?";
    std::vector<std::string> params = {
        employee.get_name(),
        std::string(1, employee.get_gender()),
        employee.get_email(),
        employee.get_phone(),
        employee.get_address(),
        employee.get_zip_code(),
        employee.get_group_id(),
        std::to_string(employee.get_monthly_working_hours()),
        std::to_string(employee.get_monthly_salary()),
        employee.get_id()
    };

    return execute_update_with_params(sql, params);
}

EmployeeInfo EmployeeInfoDAO::find_by_id(const std::string& id) {
    std::string sql = "SELECT * FROM employee_info WHERE id = '" + id + "'";
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    if (employees.empty()) {
        return EmployeeInfo(); // 返回默认对象
    }

    return employees[0];
}

std::vector<EmployeeInfo> EmployeeInfoDAO::find_all() {
    std::string sql = "SELECT * FROM employee_info";
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    return employees;
}

std::vector<EmployeeInfo> EmployeeInfoDAO::find_by_name(const std::string& name) {
    std::string sql = "SELECT * FROM employee_info WHERE name = '" + name + "'";
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    return employees;
}

std::vector<EmployeeInfo> EmployeeInfoDAO::find_by_gender(char gender) {
    std::string sql = "SELECT * FROM employee_info WHERE gender = '" + std::string(1, gender) + "'";
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    return employees;
}

std::vector<EmployeeInfo> EmployeeInfoDAO::find_by_group_id(const std::string& group_id) {
    std::string sql = "SELECT * FROM employee_info WHERE group_id = '" + group_id + "'";
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    return employees;
}

std::vector<EmployeeInfo> EmployeeInfoDAO::find_by_salary_range(double min_salary, double max_salary) {
    std::string sql = "SELECT * FROM employee_info WHERE monthly_salary BETWEEN " +
                     std::to_string(min_salary) + " AND " + std::to_string(max_salary);
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    return employees;
}

std::vector<EmployeeInfo> EmployeeInfoDAO::find_by_hours_range(double min_hours, double max_hours) {
    std::string sql = "SELECT * FROM employee_info WHERE monthly_working_hours BETWEEN " +
                     std::to_string(min_hours) + " AND " + std::to_string(max_hours);
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    return employees;
}

std::vector<EmployeeInfo> EmployeeInfoDAO::find_by_email(const std::string& email) {
    std::string sql = "SELECT * FROM employee_info WHERE email = '" + email + "'";
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    return employees;
}

std::vector<EmployeeInfo> EmployeeInfoDAO::find_by_phone(const std::string& phone) {
    std::string sql = "SELECT * FROM employee_info WHERE phone = '" + phone + "'";
    std::vector<EmployeeInfo> employees;

    execute_query(sql, employee_info_callback, &employees);

    return employees;
}
