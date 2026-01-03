#ifndef V_USER_H
#define V_USER_H

#include <string>

/**
 * @brief 用户视图模型类
 */
class VUser {
private:
    int id_;
    std::string username_;
    std::string nickname_;
    std::string email_;
    std::string sex_;

public:
    VUser(int id, const std::string& username, const std::string& nickname, 
          const std::string& email, const std::string& sex);
    
    int get_id(void) const;
    std::string get_username(void) const;
    std::string get_nickname(void) const;
    std::string get_email(void) const;
    std::string get_sex(void) const;
};

#endif // V_USER_H
