/**
 * @file password_tool.h
 * @brief 密码工具类，提供密码哈希生成和验证功能
 * @version 1.0.0
 */

#ifndef PASSWORD_TOOL_H
#define PASSWORD_TOOL_H

#include <string>
#include <vector>
#include <optional>

/**
 * @class PasswordTool
 * @brief 密码工具类，提供基于 SHA256 的密码哈希和验证功能
 *
 * 该类提供了密码哈希生成和验证功能，使用 OpenSSL 库实现 SHA256 散列算法
 * 和安全的随机盐值生成。所有密码相关操作都遵循安全最佳实践。
 */
class PasswordTool {
public:
    /**
     * @brief 生成盐值和密码哈希
     *
     * 根据输入的明文密码生成随机盐值，并计算密码和盐值的 SHA256 散列值
     *
     * @param[in] plaintext_password 明文密码
     * @return std::optional<std::pair<std::string, std::string>> 
     *         如果成功，返回包含盐值和密码哈希的 pair；如果失败，返回 std::nullopt
     *         第一个元素是 base64 编码的盐值，第二个元素是 base64 编码的密码哈希
     */
    static std::optional<std::pair<std::string, std::string>> 
    generate_salt_and_hash(const std::string& plaintext_password);

    /**
     * @brief 验证密码是否匹配
     *
     * 验证输入的明文密码是否与存储的密码哈希匹配
     *
     * @param[in] plaintext_password_to_check 待验证的明文密码
     * @param[in] stored_hash 存储的密码哈希（base64 编码）
     * @param[in] salt 存储的盐值（base64 编码）
     * @return bool 如果密码匹配返回 true，否则返回 false
     */
    static bool verify_password(
        const std::string& plaintext_password_to_check,
        const std::string& stored_hash,
        const std::string& salt);

private:
    // 禁用默认构造函数和复制操作
    PasswordTool() = delete;
    PasswordTool(const PasswordTool&) = delete;
    PasswordTool& operator=(const PasswordTool&) = delete;

    /**
     * @brief 生成随机盐值
     *
     * 使用 OpenSSL 的加密安全随机数生成器生成盐值
     *
     * @param[in] salt_length 盐值长度（字节数）
     * @return std::optional<std::vector<unsigned char>> 
     *         如果成功，返回盐值字节数组；如果失败，返回 std::nullopt
     */
    static std::optional<std::vector<unsigned char>> 
    generate_salt(size_t salt_length = SALT_LENGTH_);

    /**
     * @brief 计算密码和盐值的 SHA256 散列值
     *
     * 将密码和盐值拼接后计算 SHA256 散列值
     *
     * @param[in] plaintext_password 明文密码
     * @param[in] salt 盐值字节数组
     * @return std::optional<std::vector<unsigned char>> 
     *         如果成功，返回哈希值字节数组；如果失败，返回 std::nullopt
     */
    static std::optional<std::vector<unsigned char>> 
    compute_sha256_hash(
        const std::string& plaintext_password,
        const std::vector<unsigned char>& salt);

    /**
     * @brief Base64 编码
     *
     * 将字节数组编码为 base64 字符串
     *
     * @param[in] data 要编码的字节数组
     * @return std::string base64 编码的字符串
     */
    static std::string base64_encode(const std::vector<unsigned char>& data);

    /**
     * @brief Base64 解码
     *
     * 将 base64 字符串解码为字节数组
     *
     * @param[in] encoded_string base64 编码的字符串
     * @return std::optional<std::vector<unsigned char>> 
     *         如果成功，返回解码后的字节数组；如果失败，返回 std::nullopt
     */
    static std::optional<std::vector<unsigned char>> 
    base64_decode(const std::string& encoded_string);

    /// @brief 默认盐值长度（字节）
    static const size_t SALT_LENGTH_ = 32;
    
    /// @brief SHA256 哈希值长度（字节）
    static const size_t HASH_LENGTH_ = 32;
};

#endif // PASSWORD_TOOL_H
