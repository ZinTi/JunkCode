/**
* @file base_dao.cpp
 * @brief 基础DAO实现
 */

#include "repositories/base_dao.h"
#include <iostream>

extern "C" {
#include <sqlite3.h>
}

std::shared_ptr<sqlite3> BaseDAO::get_connection() {
    return SQLiteConnectionPool::get_instance().get_connection();
}

int BaseDAO::execute_query(const std::string& sql, int (*callback)(void*, int, char**, char**), void* data) {
    auto connection = get_connection();
    char* error_msg = nullptr;

    int result = sqlite3_exec(connection.get(), sql.c_str(), callback, data, &error_msg);

    if (result != SQLITE_OK) {
        std::cerr << "SQL error: " << error_msg << std::endl;
        sqlite3_free(error_msg);
    }

    return result;
}

bool BaseDAO::execute_update(const std::string& sql) {
    auto connection = get_connection();
    char* error_msg = nullptr;

    int result = sqlite3_exec(connection.get(), sql.c_str(), nullptr, nullptr, &error_msg);

    if (result != SQLITE_OK) {
        std::cerr << "SQL error: " << error_msg << std::endl;
        sqlite3_free(error_msg);
        return false;
    }

    return true;
}

bool BaseDAO::execute_update_with_params(const std::string& sql, const std::vector<std::string>& params) {
    auto connection = get_connection();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(connection.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
    }

    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return result;
}

bool BaseDAO::exists(const std::string& table, const std::string& column, const std::string& value) {
    std::string sql = "SELECT COUNT(*) FROM " + table + " WHERE " + column + " = ?";
    auto connection = get_connection();
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(connection.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC);
    
    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = (sqlite3_column_int(stmt, 0) > 0);
    }
    
    sqlite3_finalize(stmt);
    return exists;
}
