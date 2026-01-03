#include "dao/user_dao.h"
#include "util/security_util.h"
#include <iostream>

bool UserDao::register_user(const std::string& username, const std::string& password, 
                           const std::string& email, int gender_id, 
                           const std::string& nickname) {
    if (username_exists(username)) {
        std::cerr << "用户名已存在: " << username << std::endl;
        return false;
    }
    
    std::string salt = SecurityUtil::generate_salt();
    std::string hashed_password = SecurityUtil::hash_password(password, salt);
    
    std::string sql = R"(
        INSERT INTO users (username, nickname, salt, passwd, email, gender_id)
        VALUES (?, ?, ?, ?, ?, ?);
    )";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, nickname.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, salt.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, hashed_password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, gender_id);
    
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    
    return success;
}

std::unique_ptr<User> UserDao::validate_by_username(const std::string& username, const std::string& password) {
    // 1. 通过用户名获取用户完整信息
    auto user = get_user_by_username(username);
    if (user) { // 2. 验证用户密码是否正确
        if (!SecurityUtil::verify_password(password, user->get_passwd(), user->get_salt())) {
            return nullptr; // 验证失败，返回空指针
        }
    }
    return user;
}

std::unique_ptr<User> UserDao::validate_by_email(const std::string& email, const std::string& password) {
    // 1. 通过邮箱获取用户完整信息
    auto user = get_user_by_email(email);
    if (user) { // 2. 验证用户密码是否正确
        if (!SecurityUtil::verify_password(password, user->get_passwd(), user->get_salt())) {
            return nullptr; // 验证失败，返回空指针
        }
    }
    return user;
}

std::unique_ptr<User> UserDao::get_user_by_username(const std::string& username) {
    std::string sql = "SELECT id, username, nickname, salt, passwd, email, gender_id FROM users WHERE username = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return nullptr;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    
    std::unique_ptr<User> user = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        const unsigned char* nickname = sqlite3_column_text(stmt, 2);
        const unsigned char* salt = sqlite3_column_text(stmt, 3);
        const unsigned char* passwd = sqlite3_column_text(stmt, 4);
        const unsigned char* email = sqlite3_column_text(stmt, 5);
        int gender_id = sqlite3_column_int(stmt, 6);
        
        user = std::make_unique<User>(id, 
            std::string(reinterpret_cast<const char*>(username)),
            std::string(reinterpret_cast<const char*>(nickname)),
            std::string(reinterpret_cast<const char*>(salt)),
            std::string(reinterpret_cast<const char*>(passwd)),
            std::string(reinterpret_cast<const char*>(email)),
            gender_id);
    }
    
    sqlite3_finalize(stmt);
    return user;
}

std::unique_ptr<User> UserDao::get_user_by_id(int user_id) {
    std::string sql = "SELECT id, username, nickname, salt, passwd, email, gender_id FROM users WHERE id = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return nullptr;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    std::unique_ptr<User> user = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        const unsigned char* nickname = sqlite3_column_text(stmt, 2);
        const unsigned char* salt = sqlite3_column_text(stmt, 3);
        const unsigned char* passwd = sqlite3_column_text(stmt, 4);
        const unsigned char* email = sqlite3_column_text(stmt, 5);
        int gender_id = sqlite3_column_int(stmt, 6);
        
        user = std::make_unique<User>(id, 
            std::string(reinterpret_cast<const char*>(username)),
            std::string(reinterpret_cast<const char*>(nickname)),
            std::string(reinterpret_cast<const char*>(salt)),
            std::string(reinterpret_cast<const char*>(passwd)),
            std::string(reinterpret_cast<const char*>(email)),
            gender_id);
    }
    
    sqlite3_finalize(stmt);
    return user;
}

std::unique_ptr<User> UserDao::get_user_by_email(const std::string& email) {
    std::string sql = "SELECT id, username, nickname, salt, passwd, email, gender_id FROM users WHERE email = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return nullptr;
    }
    
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    
    std::unique_ptr<User> user = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        const unsigned char* nickname = sqlite3_column_text(stmt, 2);
        const unsigned char* salt = sqlite3_column_text(stmt, 3);
        const unsigned char* passwd = sqlite3_column_text(stmt, 4);
        const unsigned char* email = sqlite3_column_text(stmt, 5);
        int gender_id = sqlite3_column_int(stmt, 6);

        user = std::make_unique<User>(id, 
            std::string(reinterpret_cast<const char*>(username)),
            std::string(reinterpret_cast<const char*>(nickname)),
            std::string(reinterpret_cast<const char*>(salt)),
            std::string(reinterpret_cast<const char*>(passwd)),
            std::string(reinterpret_cast<const char*>(email)),
            gender_id);
    }
    
    sqlite3_finalize(stmt);
    return user;
}

std::vector<VUser> UserDao::get_all_vusers(void) {
    std::vector<VUser> vusers;
    std::string sql = "SELECT id, username, nickname, email, sex FROM v_users ORDER BY id;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return vusers;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        const unsigned char* nickname = sqlite3_column_text(stmt, 2);
        const unsigned char* email = sqlite3_column_text(stmt, 3);
        const unsigned char* sex = sqlite3_column_text(stmt, 4);
        
        vusers.emplace_back(id,
            std::string(reinterpret_cast<const char*>(username)),
            std::string(reinterpret_cast<const char*>(nickname)),
            std::string(reinterpret_cast<const char*>(email)),
            std::string(reinterpret_cast<const char*>(sex)));
    }
    
    sqlite3_finalize(stmt);
    return vusers;
}

std::string UserDao::get_username_by_id(const int user_id){
    std::string username;
    std::string sql = "SELECT username FROM users WHERE id = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return nullptr;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        username = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);
    return username;
}

std::string UserDao::get_username_by_email(const std::string& email){
    std::string username;
    std::string sql = "SELECT username FROM users WHERE email = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return nullptr;
    }
    
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        username = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);
    return username;
}

bool UserDao::update_user(const User& user) {
    std::string sql = R"(
        UPDATE users 
        SET username = ?, nickname = ?, email = ?, gender_id = ?
        WHERE id = ?;
    )";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, user.get_username().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.get_nickname().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.get_email().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, user.get_gender_id());
    sqlite3_bind_int(stmt, 5, user.get_id());
    
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    
    return success;
}

bool UserDao::force_reset_password(int user_id, const std::string& new_password) {
    std::string salt = SecurityUtil::generate_salt();
    std::string hashed_password = SecurityUtil::hash_password(new_password, salt);
    
    std::string sql = "UPDATE users SET salt = ?, passwd = ? WHERE id = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, salt.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, hashed_password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, user_id);
    
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    
    return success;
}

bool UserDao::delete_user(int user_id) {
    std::string sql = "DELETE FROM users WHERE id = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    
    return success;
}

bool UserDao::username_exists(const std::string& username) {
    std::string sql = "SELECT COUNT(*) FROM users WHERE username = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "准备SQL语句失败: " << sqlite3_errmsg(db_.get()) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    
    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = (sqlite3_column_int(stmt, 0) > 0);
    }
    
    sqlite3_finalize(stmt);
    return exists;
}
