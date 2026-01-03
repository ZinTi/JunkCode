/**
 * @file parts_info_dao.cpp
 * @brief 配件信息DAO实现
 */

#include "repositories/parts_info_dao.h"
#include <iostream>

extern "C" {
    #include <sqlite3.h>
}

// 查询回调函数
static int parts_info_callback(void* data, int argc, char** argv, char** col_names) {
    auto* parts = static_cast<std::vector<PartsInfo>*>(data);

    if (argc == 7) { // 所有字段
        int number = argv[3] ? std::stoi(argv[3]) : 0;
        double price = argv[5] ? std::stod(argv[5]) : 0.0;

        PartsInfo part(argv[0] ? argv[0] : "",
                      argv[1] ? argv[1] : "",
                      argv[2] ? argv[2] : "",
                      number,
                      argv[4] ? argv[4] : "",
                      price,
                      argv[6] ? argv[6] : "");
        parts->push_back(part);
    }

    return 0;
}

bool PartsInfoDAO::insert(const PartsInfo& part) {
    std::string sql = "INSERT INTO parts_info (id, model, name, number, classification, price, remarks) VALUES (?, ?, ?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        part.get_id(),
        part.get_model(),
        part.get_name(),
        std::to_string(part.get_number()),
        part.get_classification(),
        std::to_string(part.get_price()),
        part.get_remarks()
    };

    return execute_update_with_params(sql, params);
}

bool PartsInfoDAO::remove(const std::string& id) {
    std::string sql = "DELETE FROM parts_info WHERE id = ?";
    std::vector<std::string> params = {id};

    return execute_update_with_params(sql, params);
}

bool PartsInfoDAO::update(const PartsInfo& part) {
    std::string sql = "UPDATE parts_info SET model = ?, name = ?, number = ?, classification = ?, price = ?, remarks = ? WHERE id = ?";
    std::vector<std::string> params = {
        part.get_model(),
        part.get_name(),
        std::to_string(part.get_number()),
        part.get_classification(),
        std::to_string(part.get_price()),
        part.get_remarks(),
        part.get_id()
    };

    return execute_update_with_params(sql, params);
}

PartsInfo PartsInfoDAO::find_by_id(const std::string& id) {
    std::string sql = "SELECT * FROM parts_info WHERE id = '" + id + "'";
    std::vector<PartsInfo> parts;

    execute_query(sql, parts_info_callback, &parts);

    if (parts.empty()) {
        return PartsInfo(); // 返回默认对象
    }

    return parts[0];
}

std::vector<PartsInfo> PartsInfoDAO::find_all() {
    std::string sql = "SELECT * FROM parts_info";
    std::vector<PartsInfo> parts;

    execute_query(sql, parts_info_callback, &parts);

    return parts;
}

std::vector<PartsInfo> PartsInfoDAO::find_by_model(const std::string& model) {
    std::string sql = "SELECT * FROM parts_info WHERE model = '" + model + "'";
    std::vector<PartsInfo> parts;

    execute_query(sql, parts_info_callback, &parts);

    return parts;
}

std::vector<PartsInfo> PartsInfoDAO::find_by_name(const std::string& name) {
    std::string sql = "SELECT * FROM parts_info WHERE name = '" + name + "'";
    std::vector<PartsInfo> parts;

    execute_query(sql, parts_info_callback, &parts);

    return parts;
}

std::vector<PartsInfo> PartsInfoDAO::find_by_classification(const std::string& classification) {
    std::string sql = "SELECT * FROM parts_info WHERE classification = '" + classification + "'";
    std::vector<PartsInfo> parts;

    execute_query(sql, parts_info_callback, &parts);

    return parts;
}

std::vector<PartsInfo> PartsInfoDAO::find_by_number_range(int min_number, int max_number) {
    std::string sql = "SELECT * FROM parts_info WHERE number BETWEEN " +
                     std::to_string(min_number) + " AND " + std::to_string(max_number);
    std::vector<PartsInfo> parts;

    execute_query(sql, parts_info_callback, &parts);

    return parts;
}

std::vector<PartsInfo> PartsInfoDAO::find_by_price_range(double min_price, double max_price) {
    std::string sql = "SELECT * FROM parts_info WHERE price BETWEEN " +
                     std::to_string(min_price) + " AND " + std::to_string(max_price);
    std::vector<PartsInfo> parts;

    execute_query(sql, parts_info_callback, &parts);

    return parts;
}

bool PartsInfoDAO::update_number(const std::string& id, int new_number) {
    std::string sql = "UPDATE parts_info SET number = ? WHERE id = ?";
    std::vector<std::string> params = {
        std::to_string(new_number),
        id
    };

    return execute_update_with_params(sql, params);
}

bool PartsInfoDAO::increase_number(const std::string& id, int increment) {
    std::string sql = "UPDATE parts_info SET number = number + ? WHERE id = ?";
    std::vector<std::string> params = {
        std::to_string(increment),
        id
    };

    return execute_update_with_params(sql, params);
}

bool PartsInfoDAO::decrease_number(const std::string& id, int decrement) {
    std::string sql = "UPDATE parts_info SET number = number - ? WHERE id = ? AND number >= ?";
    std::vector<std::string> params = {
        std::to_string(decrement),
        id,
        std::to_string(decrement)
    };

    return execute_update_with_params(sql, params);
}
