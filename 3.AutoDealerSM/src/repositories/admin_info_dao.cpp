/**
 * @file admin_info_dao.cpp
 * @brief 管理员信息DAO实现
 */

#include "repositories/admin_info_dao.h"
#include <iostream>

extern "C" {
    #include <sqlite3.h>
}

// 查询回调函数
static int adminInfoCallback(void* data, int argc, char** argv, char** colNames) {
    auto* admins = static_cast<std::vector<AdminInfo>*>(data);

    if (argc == 4) { // id, name, salt, pwd
        AdminInfo admin(argv[0] ? argv[0] : "",
                       argv[1] ? argv[1] : "",
                       argv[2] ? argv[2] : "",
                       argv[3] ? argv[3] : "");
        admins->push_back(admin);
    }

    return 0;
}

bool AdminInfoDAO::insert(const AdminInfo& admin) {
    std::string sql = "INSERT INTO admin_info (id, name, salt, pwd) VALUES (?, ?, ?, ?)";
    std::vector<std::string> params = {
        admin.get_id(),
        admin.get_name(),
        admin.get_salt(),
        admin.get_pwd()
    };

    return  execute_update_with_params(sql, params);
}

bool AdminInfoDAO::remove(const std::string& id) {
    std::string sql = "DELETE FROM admin_info WHERE id = ?";
    std::vector<std::string> params = {id};

    return  execute_update_with_params(sql, params);
}

bool AdminInfoDAO::update(const AdminInfo& admin) {
    std::string sql = "UPDATE admin_info SET name = ?, salt = ?, pwd = ? WHERE id = ?";
    std::vector<std::string> params = {
        admin.get_name(),
        admin.get_salt(),
        admin.get_pwd(),
        admin.get_id()
    };

    return  execute_update_with_params(sql, params);
}

AdminInfo AdminInfoDAO::find_by_id(const std::string& id) {
    std::string sql = "SELECT * FROM admin_info WHERE id = '" + id + "'";
    std::vector<AdminInfo> admins;

    execute_query(sql, adminInfoCallback, &admins);

    if (admins.empty()) {
        return AdminInfo(); // 返回默认对象
    }

    return admins[0];
}

std::vector<AdminInfo> AdminInfoDAO::find_all() {
    std::string sql = "SELECT * FROM admin_info";
    std::vector<AdminInfo> admins;

    execute_query(sql, adminInfoCallback, &admins);

    return admins;
}

std::vector<AdminInfo> AdminInfoDAO::find_by_name(const std::string& name) {
    std::string sql = "SELECT * FROM admin_info WHERE name = '" + name + "'";
    std::vector<AdminInfo> admins;

    execute_query(sql, adminInfoCallback, &admins);

    return admins;
}

bool AdminInfoDAO::id_exists(const std::string& id) {
    return exists("admin_info", "id", id);
}
