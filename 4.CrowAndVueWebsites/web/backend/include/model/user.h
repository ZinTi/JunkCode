#ifndef USER_H
#define USER_H

#include <string>

/**
 * @brief 用户模型类
 */
class User {
private:
    int id_;
    std::string username_;
    std::string nickname_;
    std::string salt_;
    std::string passwd_;
    std::string email_;
    int gender_id_;

public:
    User(int id, const std::string& username, const std::string& nickname, 
         const std::string& salt, const std::string& passwd, 
         const std::string& email, int gender_id);
    
    // Getter方法
    int get_id(void) const;
    std::string get_username(void) const;
    std::string get_nickname(void) const;
    std::string get_salt(void) const;
    std::string get_passwd(void) const;
    std::string get_email(void) const;
    int get_gender_id(void) const;
    
    // Setter方法
    void set_id(int id);
    void set_username(const std::string& username);
    void set_nickname(const std::string& nickname);
    void set_salt(const std::string& salt);
    void set_passwd(const std::string& passwd);
    void set_email(const std::string& email);
    void set_gender_id(int gender_id);

    // 清空数据
    void set_empty();
};

#endif // USER_H
