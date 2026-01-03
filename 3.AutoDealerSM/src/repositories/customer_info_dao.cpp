/**
 * @file customer_info_dao.cpp
 * @brief 客户信息DAO实现
 */

#include "repositories/customer_info_dao.h"
#include <iostream>
#include <cstring>

extern "C" {
    #include <sqlite3.h>
}

// 查询回调函数
static int customer_info_callback(void* data, int argc, char** argv, char** col_names) {
    auto* customers = static_cast<std::vector<CustomerInfo>*>(data);

    if (argc == 13) { // 所有字段
        char gender = (argv[4] && strlen(argv[4]) > 0) ? argv[4][0] : 'M';
        CustomerInfo customer(argv[0] ? argv[0] : "",
                             argv[1] ? argv[1] : "",
                             argv[2] ? argv[2] : "",
                             argv[3] ? argv[3] : "",
                             gender,
                             argv[5] ? argv[5] : "",
                             argv[6] ? argv[6] : "",
                             argv[7] ? argv[7] : "",
                             argv[8] ? argv[8] : "",
                             argv[9] ? argv[9] : "",
                             argv[10] ? argv[10] : "",
                             argv[11] ? argv[11] : "");
        customers->push_back(customer);
    }

    return 0;
}

bool CustomerInfoDAO::insert(const CustomerInfo& customer) {
    std::string sql = "INSERT INTO customer_info (id, name, salt, pwd, gender, email, phone, address, zip_code, car_model, car_condition, car_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        customer.get_id(),
        customer.get_name(),
        customer.get_salt(),
        customer.get_pwd(),
        std::string(1, customer.get_gender()),
        customer.get_email(),
        customer.get_phone(),
        customer.get_address(),
        customer.get_zip_code(),
        customer.get_car_model(),
        customer.get_car_condition(),
        customer.get_car_id()
    };

    return execute_update_with_params(sql, params);
}

bool CustomerInfoDAO::remove(const std::string& id) {
    std::string sql = "DELETE FROM customer_info WHERE id = ?";
    std::vector<std::string> params = {id};

    return execute_update_with_params(sql, params);
}

bool CustomerInfoDAO::update(const CustomerInfo& customer) {
    std::string sql = "UPDATE customer_info SET name = ?, salt = ?, pwd = ?, gender = ?, email = ?, phone = ?, address = ?, zip_code = ?, car_model = ?, car_condition = ?, car_id = ? WHERE id = ?";
    std::vector<std::string> params = {
        customer.get_name(),
        customer.get_salt(),
        customer.get_pwd(),
        std::string(1, customer.get_gender()),
        customer.get_email(),
        customer.get_phone(),
        customer.get_address(),
        customer.get_zip_code(),
        customer.get_car_model(),
        customer.get_car_condition(),
        customer.get_car_id(),
        customer.get_id()
    };

    return execute_update_with_params(sql, params);
}

CustomerInfo CustomerInfoDAO::find_by_id(const std::string& id) {
    std::string sql = "SELECT * FROM customer_info WHERE id = '" + id + "'";
    std::vector<CustomerInfo> customers;

    execute_query(sql, customer_info_callback, &customers);

    if (customers.empty()) {
        return CustomerInfo(); // 返回默认对象
    }

    return customers[0];
}

std::vector<CustomerInfo> CustomerInfoDAO::find_all() {
    std::string sql = "SELECT * FROM customer_info";
    std::vector<CustomerInfo> customers;

    execute_query(sql, customer_info_callback, &customers);

    return customers;
}

std::vector<CustomerInfo> CustomerInfoDAO::find_by_name(const std::string& name) {
    std::string sql = "SELECT * FROM customer_info WHERE name = '" + name + "'";
    std::vector<CustomerInfo> customers;

    execute_query(sql, customer_info_callback, &customers);

    return customers;
}

std::vector<CustomerInfo> CustomerInfoDAO::find_by_gender(char gender) {
    std::string sql = "SELECT * FROM customer_info WHERE gender = '" + std::string(1, gender) + "'";
    std::vector<CustomerInfo> customers;

    execute_query(sql, customer_info_callback, &customers);

    return customers;
}

std::vector<CustomerInfo> CustomerInfoDAO::find_by_email(const std::string& email) {
    std::string sql = "SELECT * FROM customer_info WHERE email = '" + email + "'";
    std::vector<CustomerInfo> customers;

    execute_query(sql, customer_info_callback, &customers);

    return customers;
}

std::vector<CustomerInfo> CustomerInfoDAO::find_by_phone(const std::string& phone) {
    std::string sql = "SELECT * FROM customer_info WHERE phone = '" + phone + "'";
    std::vector<CustomerInfo> customers;

    execute_query(sql, customer_info_callback, &customers);

    return customers;
}

std::vector<CustomerInfo> CustomerInfoDAO::find_by_car_model(const std::string& car_model) {
    std::string sql = "SELECT * FROM customer_info WHERE car_model = '" + car_model + "'";
    std::vector<CustomerInfo> customers;

    execute_query(sql, customer_info_callback, &customers);

    return customers;
}

std::vector<CustomerInfo> CustomerInfoDAO::find_by_car_condition(const std::string& car_condition) {
    std::string sql = "SELECT * FROM customer_info WHERE car_condition = '" + car_condition + "'";
    std::vector<CustomerInfo> customers;

    execute_query(sql, customer_info_callback, &customers);

    return customers;
}

bool CustomerInfoDAO::id_exists(const std::string& id) {
    return exists("customer_info", "id", id);
}
