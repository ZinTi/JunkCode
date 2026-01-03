/**
 * @file input_validator.h
 * @brief 输入数据验证类头文件
 */

#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <string>
#include <string_view>

/**
 * @class InputValidator
 * @brief 输入数据验证工具类
 * 
 * 提供密码、邮箱、手机号等常见输入数据的验证功能
 */
class InputValidator {
public:
    /**
     * @brief 验证密码是否合法
     * 
     * 密码要求：
     * - 长度在6-18位之间
     * - 至少包含以下四种字符类型中的至少两种：
     *   1. 数字 (0-9)
     *   2. 小写字母 (a-z)
     *   3. 大写字母 (A-Z)
     *   4. 其他字符（标点符号、特殊符号等）
     * 
     * @param password 待验证的密码字符串
     * @return true 密码合法
     * @return false 密码不合法
     */
    static bool validate_password(const std::string_view password);

    /**
     * @brief 简单验证邮箱格式是否合法
     * 
     * 验证规则：
     * - 必须包含且仅包含一个 @ 符号
     * - @ 符号前必须有至少一个字符（本地部分）
     * - @ 符号后必须有一个点号 .
     * - 点号后必须至少有一个字符（顶级域名）
     * - @ 和点号之间必须有至少一个字符（域名部分）
     * 
     * @param email 待验证的邮箱字符串
     * @return true 邮箱格式合法
     * @return false 邮箱格式不合法
     */
    static bool validate_email(const std::string_view email);

    /**
     * @brief 验证手机号是否合法
     * 
     * 验证规则：
     * - 必须为11位数字
     * - 第一位必须为1
     * 
     * @param phone_number 待验证的手机号字符串
     * @return true 手机号格式合法
     * @return false 手机号格式不合法
     */
    static bool validate_phone_number(const std::string_view phone_number);

private:
    /**
     * @brief 检查字符是否为数字
     * 
     * @param ch 待检查的字符
     * @return true 字符是数字
     * @return false 字符不是数字
     */
    static bool is_digit(char ch);

    /**
     * @brief 检查字符是否为小写字母
     * 
     * @param ch 待检查的字符
     * @return true 字符是小写字母
     * @return false 字符不是小写字母
     */
    static bool is_lowercase(char ch);

    /**
     * @brief 检查字符是否为大写字母
     * 
     * @param ch 待检查的字符
     * @return true 字符是大写字母
     * @return false 字符不是大写字母
     */
    static bool is_uppercase(char ch);

    /**
     * @brief 检查字符是否为其他字符（非数字、非字母）
     * 
     * @param ch 待检查的字符
     * @return true 字符是其他字符
     * @return false 字符不是其他字符
     */
    static bool is_other_char(char ch);
};

#endif // INPUT_VALIDATOR_H
