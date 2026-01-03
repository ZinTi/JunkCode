#include "dao/base_dao.h"
#include <iostream>

#define DATABASE_FILENAME       "web_backend.db"

// SQLite3删除器用于智能指针
struct Sqlite3Deleter {
    void operator()(sqlite3* db) const {
        if (db) {
            sqlite3_close(db);
        }
    }
};

BaseDao::BaseDao(void) {
    open_database(DATABASE_FILENAME);
}

bool BaseDao::open_database(const std::string& db_path) {
    sqlite3* db = nullptr;
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    db_ = std::shared_ptr<sqlite3>(db, Sqlite3Deleter());
    
    // 启用外键约束
    execute_sql("PRAGMA foreign_keys = ON;");
    
    // 初始化数据库
    return initialize_database();
}

bool BaseDao::execute_sql(const std::string& sql) {
    char* error_msg = nullptr;
    if (sqlite3_exec(db_.get(), sql.c_str(), nullptr, nullptr, &error_msg) != SQLITE_OK) {
        std::cerr << "SQL错误: " << error_msg << std::endl;
        sqlite3_free(error_msg);
        return false;
    }
    return true;
}

bool BaseDao::initialize_database(void) {
    // 创建gender表
    std::string create_gender_table = R"(
        CREATE TABLE IF NOT EXISTS gender (
            id INTEGER PRIMARY KEY,
            sex TEXT
        );
    )";
    
    if (!execute_sql(create_gender_table)) {
        return false;
    }
    
    // 插入性别数据
    std::string insert_gender = R"(
        INSERT OR IGNORE INTO gender VALUES
        (0, '未知'),
        (1, '男'),
        (2, '女');
    )";
    
    if (!execute_sql(insert_gender)) {
        return false;
    }
    
    // 创建users表
    std::string create_users_table = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            nickname TEXT NOT NULL DEFAULT '新用户',
            salt TEXT NOT NULL,
            passwd TEXT NOT NULL,
            email TEXT NOT NULL UNIQUE,
            gender_id INTEGER NOT NULL,
            FOREIGN KEY (gender_id) REFERENCES gender(id)
        );
    )";
    
    if (!execute_sql(create_users_table)) {
        return false;
    }
    
    // 插入默认管理员用户（如果不存在）
    std::string insert_admin = R"(
        INSERT OR IGNORE INTO users (id, username, nickname, salt, passwd, email, gender_id)
        VALUES (1, 'root', '超级管理员', '40c3b8e28e587f89', 
                '34e47279d89702f8403ec5d0152c6aa7f2f16ce08fe91676dd1e1a4fd34a7d20', 'root@email.com', 
                (SELECT id FROM gender WHERE sex = '未知'));
    )";
    
    if (!execute_sql(insert_admin)) {
        return false;
    }
    
    // 创建视图
    std::string create_view = R"(
        CREATE VIEW IF NOT EXISTS v_users AS
        SELECT users.id, users.username, users.nickname,
        users.email, gender.sex FROM users LEFT JOIN gender 
        ON users.gender_id = gender.id;
    )";
    
    return execute_sql(create_view);
}
