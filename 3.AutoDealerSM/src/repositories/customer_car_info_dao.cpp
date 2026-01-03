/**
 * @file customer_car_info_dao.cpp
 * @brief 客户车辆信息DAO实现
 */

#include "repositories/customer_car_info_dao.h"
#include <iostream>

extern "C" {
    #include <sqlite3.h>
}

// 查询回调函数
static int customer_car_info_callback(void* data, int argc, char** argv, char** col_names) {
    auto* car_infos = static_cast<std::vector<CustomerCarInfo>*>(data);

    if (argc == 5) { // id, condition, color, cust_name, model
        CustomerCarInfo car_info(argv[0] ? argv[0] : "",
                                argv[1] ? argv[1] : "",
                                argv[2] ? argv[2] : "",
                                argv[3] ? argv[3] : "",
                                argv[4] ? argv[4] : "");
        car_infos->push_back(car_info);
    }

    return 0;
}

bool CustomerCarInfoDAO::insert(const CustomerCarInfo& car_info) {
    std::string sql = "INSERT INTO customer_car_info (id, condition, color, cust_name, model) VALUES (?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        car_info.get_id(),
        car_info.get_condition(),
        car_info.get_color(),
        car_info.get_cust_name(),
        car_info.get_model()
    };

    return execute_update_with_params(sql, params);
}

bool CustomerCarInfoDAO::remove(const std::string& id) {
    std::string sql = "DELETE FROM customer_car_info WHERE id = ?";
    std::vector<std::string> params = {id};

    return execute_update_with_params(sql, params);
}

bool CustomerCarInfoDAO::update(const CustomerCarInfo& car_info) {
    std::string sql = "UPDATE customer_car_info SET condition = ?, color = ?, cust_name = ?, model = ? WHERE id = ?";
    std::vector<std::string> params = {
        car_info.get_condition(),
        car_info.get_color(),
        car_info.get_cust_name(),
        car_info.get_model(),
        car_info.get_id()
    };

    return execute_update_with_params(sql, params);
}

CustomerCarInfo CustomerCarInfoDAO::find_by_id(const std::string& id) {
    std::string sql = "SELECT * FROM customer_car_info WHERE id = '" + id + "'";
    std::vector<CustomerCarInfo> car_infos;

    execute_query(sql, customer_car_info_callback, &car_infos);

    if (car_infos.empty()) {
        return CustomerCarInfo(); // 返回默认对象
    }

    return car_infos[0];
}

std::vector<CustomerCarInfo> CustomerCarInfoDAO::find_all() {
    std::string sql = "SELECT * FROM customer_car_info";
    std::vector<CustomerCarInfo> car_infos;

    execute_query(sql, customer_car_info_callback, &car_infos);

    return car_infos;
}

std::vector<CustomerCarInfo> CustomerCarInfoDAO::find_by_cust_name(const std::string& cust_name) {
    std::string sql = "SELECT * FROM customer_car_info WHERE cust_name = '" + cust_name + "'";
    std::vector<CustomerCarInfo> car_infos;

    execute_query(sql, customer_car_info_callback, &car_infos);

    return car_infos;
}

std::vector<CustomerCarInfo> CustomerCarInfoDAO::find_by_model(const std::string& model) {
    std::string sql = "SELECT * FROM customer_car_info WHERE model = '" + model + "'";
    std::vector<CustomerCarInfo> car_infos;

    execute_query(sql, customer_car_info_callback, &car_infos);

    return car_infos;
}

std::vector<CustomerCarInfo> CustomerCarInfoDAO::find_by_condition(const std::string& condition) {
    std::string sql = "SELECT * FROM customer_car_info WHERE condition = '" + condition + "'";
    std::vector<CustomerCarInfo> car_infos;

    execute_query(sql, customer_car_info_callback, &car_infos);

    return car_infos;
}

std::vector<CustomerCarInfo> CustomerCarInfoDAO::find_by_color(const std::string& color) {
    std::string sql = "SELECT * FROM customer_car_info WHERE color = '" + color + "'";
    std::vector<CustomerCarInfo> car_infos;

    execute_query(sql, customer_car_info_callback, &car_infos);

    return car_infos;
}
