/**
 * @file beauty_maintenance_proj_dao.cpp
 * @brief 美容保养项目DAO实现
 */

#include "repositories/beauty_maintenance_proj_dao.h"
#include <iostream>

extern "C" {
    #include <sqlite3.h>
}

// 查询回调函数
static int beauty_maintenance_proj_callback(void* data, int argc, char** argv, char** col_names) {
    auto* projects = static_cast<std::vector<BeautyMaintenanceProj>*>(data);

    if (argc == 4) { // id, proj_name, price, group_id
        double price = argv[2] ? std::stod(argv[2]) : 0.0;
        BeautyMaintenanceProj proj(argv[0] ? argv[0] : "",
                                  argv[1] ? argv[1] : "",
                                  price,
                                  argv[3] ? argv[3] : "");
        projects->push_back(proj);
    }

    return 0;
}

bool BeautyMaintenanceProjDAO::insert(const BeautyMaintenanceProj& proj) {
    std::string sql = "INSERT INTO beauty_maintenance_proj (id, proj_name, price, group_id) VALUES (?, ?, ?, ?)";
    std::vector<std::string> params = {
        proj.get_id(),
        proj.get_proj_name(),
        std::to_string(proj.get_price()),
        proj.get_group_id()
    };

    return execute_update_with_params(sql, params);
}

bool BeautyMaintenanceProjDAO::remove(const std::string& id) {
    std::string sql = "DELETE FROM beauty_maintenance_proj WHERE id = ?";
    std::vector<std::string> params = {id};

    return execute_update_with_params(sql, params);
}

bool BeautyMaintenanceProjDAO::update(const BeautyMaintenanceProj& proj) {
    std::string sql = "UPDATE beauty_maintenance_proj SET proj_name = ?, price = ?, group_id = ? WHERE id = ?";
    std::vector<std::string> params = {
        proj.get_proj_name(),
        std::to_string(proj.get_price()),
        proj.get_group_id(),
        proj.get_id()
    };

    return execute_update_with_params(sql, params);
}

BeautyMaintenanceProj BeautyMaintenanceProjDAO::find_by_id(const std::string& id) {
    std::string sql = "SELECT * FROM beauty_maintenance_proj WHERE id = '" + id + "'";
    std::vector<BeautyMaintenanceProj> projects;

    execute_query(sql, beauty_maintenance_proj_callback, &projects);

    if (projects.empty()) {
        return BeautyMaintenanceProj(); // 返回默认对象
    }

    return projects[0];
}

std::vector<BeautyMaintenanceProj> BeautyMaintenanceProjDAO::find_all() {
    std::string sql = "SELECT * FROM beauty_maintenance_proj";
    std::vector<BeautyMaintenanceProj> projects;

    execute_query(sql, beauty_maintenance_proj_callback, &projects);

    return projects;
}

std::vector<BeautyMaintenanceProj> BeautyMaintenanceProjDAO::find_by_group_id(const std::string& group_id) {
    std::string sql = "SELECT * FROM beauty_maintenance_proj WHERE group_id = '" + group_id + "'";
    std::vector<BeautyMaintenanceProj> projects;

    execute_query(sql, beauty_maintenance_proj_callback, &projects);

    return projects;
}

std::vector<BeautyMaintenanceProj> BeautyMaintenanceProjDAO::find_by_proj_name(const std::string& proj_name) {
    std::string sql = "SELECT * FROM beauty_maintenance_proj WHERE proj_name = '" + proj_name + "'";
    std::vector<BeautyMaintenanceProj> projects;

    execute_query(sql, beauty_maintenance_proj_callback, &projects);

    return projects;
}

std::vector<BeautyMaintenanceProj> BeautyMaintenanceProjDAO::find_by_price_range(double min_price, double max_price) {
    std::string sql = "SELECT * FROM beauty_maintenance_proj WHERE price BETWEEN " +
                     std::to_string(min_price) + " AND " + std::to_string(max_price);
    std::vector<BeautyMaintenanceProj> projects;

    execute_query(sql, beauty_maintenance_proj_callback, &projects);

    return projects;
}
