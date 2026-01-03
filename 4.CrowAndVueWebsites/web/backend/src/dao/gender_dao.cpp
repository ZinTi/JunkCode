#include "dao/gender_dao.h"
#include <iostream>

std::vector<Gender> GenderDao::get_all_genders(void) {
    std::vector<Gender> genders;
    std::string sql = "SELECT id, sex FROM gender ORDER BY id;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return genders;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* sex = sqlite3_column_text(stmt, 1);
        
        genders.emplace_back(id, std::string(reinterpret_cast<const char*>(sex)));
    }
    
    sqlite3_finalize(stmt);
    return genders;
}

std::unique_ptr<Gender> GenderDao::get_gender_by_id(int id) {
    std::string sql = "SELECT id, sex FROM gender WHERE id = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return nullptr;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    std::unique_ptr<Gender> gender = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int gender_id = sqlite3_column_int(stmt, 0);
        const unsigned char* sex = sqlite3_column_text(stmt, 1);
        
        gender = std::make_unique<Gender>(gender_id, std::string(reinterpret_cast<const char*>(sex)));
    }
    
    sqlite3_finalize(stmt);
    return gender;
}

std::unique_ptr<Gender> GenderDao::get_gender_by_sex(const std::string& sex) {
    std::string sql = "SELECT id, sex FROM gender WHERE sex = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return nullptr;
    }
    
    sqlite3_bind_text(stmt, 1, sex.c_str(), -1, SQLITE_TRANSIENT);
    
    std::unique_ptr<Gender> gender = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* gender_sex = sqlite3_column_text(stmt, 1);
        
        gender = std::make_unique<Gender>(id, std::string(reinterpret_cast<const char*>(gender_sex)));
    }
    
    sqlite3_finalize(stmt);
    return gender;
}
