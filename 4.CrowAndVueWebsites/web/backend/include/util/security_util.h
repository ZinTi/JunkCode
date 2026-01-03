#ifndef SECURITY_UTIL_H
#define SECURITY_UTIL_H

#include <string>
#include <random>

/**
 * @brief 安全工具类，提供密码加密和验证功能
 */
class SecurityUtil {
public:
    /**
     * @brief 生成随机盐值
     * @return 返回16字符的随机盐值
     */
    static std::string generate_salt(void);

    /**
     * @brief 使用盐值对密码进行哈希
     * @param password 明文密码
     * @param salt 盐值
     * @return 返回哈希后的密码
     */
    static std::string hash_password(const std::string& password, const std::string& salt);

    /**
     * @brief 验证密码是否匹配
     * @param password 待验证的明文密码
     * @param hashed_password 已哈希的密码
     * @param salt 盐值
     * @return 如果匹配返回true，否则返回false
     */
    static bool verify_password(const std::string& password, const std::string& hashed_password, const std::string& salt);
};

#endif // SECURITY_UTIL_H
