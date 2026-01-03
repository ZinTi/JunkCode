#ifndef USER_DAO_H
#define USER_DAO_H

#include "dao/base_dao.h"
#include "model/user.h"
#include "model/v_user.h"
#include <vector>
#include <memory>

/**
 * @brief 用户数据访问对象类
 */
class UserDao : public BaseDao {
public:
    UserDao(void) = default;
    
    /**
     * @brief 用户注册
     * @param username 用户名
     * @param password 密码
     * @param email 邮箱
     * @param gender_id 性别ID
     * @param nickname 昵称（可选）
     * @return 成功返回true，失败返回false
     */
    bool register_user(const std::string& username, const std::string& password, const std::string& email, int gender_id, const std::string& nickname = "新用户");

    /**
     * @brief 验证用户名和用户密码是否正确
     * @param username 用户名
     * @param password 密码
     * @return 用户对象，如果失败返回 nullptr
     */
    std::unique_ptr<User> validate_by_username(const std::string& username, const std::string& password);

    /**
     * @brief 验证用户邮箱和用户密码是否正确
     * @param email 邮箱
     * @param password 密码
     * @return 用户对象，如果失败返回 nullptr
     */
    std::unique_ptr<User> validate_by_email(const std::string& email, const std::string& password);
    
    /**
     * @brief 根据用户名获取用户信息
     * @param username 用户名
     * @return 用户对象，如果不存在返回 nullptr
     */
    std::unique_ptr<User> get_user_by_username(const std::string& username);
    
    /**
     * @brief 根据用户ID获取用户信息
     * @param user_id 用户ID
     * @return 用户对象，如果不存在返回 nullptr
     */
    std::unique_ptr<User> get_user_by_id(int user_id);

    /**
     * @brief 根据邮箱获取用户信息
     * @param email 邮箱
     * @return 用户对象，如果不存在返回 nullptr
     */
    std::unique_ptr<User> get_user_by_email(const std::string& email);
    
    /**
     * @brief 获取所有用户视图信息
     * @return 用户视图对象向量
     */
    std::vector<VUser> get_all_vusers(void);

    /**
     * @brief 获取用户名
     * @param user_id 用户ID
     * @return 用户名
     */
    std::string get_username_by_id(int user_id);

    /**
     * @brief 获取用户名
     * @param email 邮箱
     * @return 用户名
     */
    std::string get_username_by_email(const std::string& email);

    /**
     * @brief 更新用户信息
     * @param user 用户对象
     * @return 成功返回true，失败返回false
     */
    bool update_user(const User& user);
    
    /**
     * @brief 强制重置密码
     * @param user_id 用户ID
     * @param new_password 新密码
     * @return 成功返回true，失败返回false
     */
    bool force_reset_password(int user_id, const std::string& new_password);
    
    /**
     * @brief 删除用户（注销账号）
     * @param user_id 用户ID
     * @return 成功返回true，失败返回false
     */
    bool delete_user(int user_id);
    
    /**
     * @brief 检查用户名是否存在
     * @param username 用户名
     * @return 存在返回true，不存在返回false
     */
    bool username_exists(const std::string& username);
};

#endif // USER_DAO_H
