/**
 * @file input_validator.cpp
 * @brief 输入数据验证类实现文件
 */

#include "util/input_validator.h"
#include <string_view>
#include <algorithm>
#include <cctype>

bool InputValidator::validate_password(const std::string_view password) {
    // 检查长度
    const size_t length = password.length();
    if (length < 6 || length > 18) {
        return false;
    }

    // 统计字符类型
    bool has_digit = false;
    bool has_lowercase = false;
    bool has_uppercase = false;
    bool has_other = false;

    for (char ch : password) {
        if (is_digit(ch)) {
            has_digit = true;
        } else if (is_lowercase(ch)) {
            has_lowercase = true;
        } else if (is_uppercase(ch)) {
            has_uppercase = true;
        } else if (is_other_char(ch)) {
            has_other = true;
        }
    }

    // 计算包含的字符类型数量
    int type_count = 0;
    if (has_digit) type_count++;
    if (has_lowercase) type_count++;
    if (has_uppercase) type_count++;
    if (has_other) type_count++;

    // 至少需要包含两种字符类型
    return type_count >= 2;
}

bool InputValidator::validate_email(const std::string_view email) {
    // 检查是否为空
    if (email.empty()) {
        return false;
    }

    // 查找 @ 符号
    size_t at_pos = email.find('@');
    
    // 检查是否只有一个 @ 符号
    if (at_pos == std::string_view::npos || 
        email.find('@', at_pos + 1) != std::string_view::npos) {
        return false;
    }

    // 检查 @ 前是否有字符（本地部分）
    if (at_pos == 0) {
        return false;
    }

    // 检查 @ 后是否有字符
    if (at_pos == email.length() - 1) {
        return false;
    }

    // 查找点号
    size_t dot_pos = email.find('.', at_pos);
    
    // 检查点号是否存在且在 @ 之后
    if (dot_pos == std::string_view::npos || dot_pos <= at_pos + 1) {
        return false;
    }

    // 检查点号后是否还有字符（顶级域名）
    if (dot_pos == email.length() - 1) {
        return false;
    }

    // 检查点号和 @ 之间是否有字符（域名部分）
    if (dot_pos == at_pos + 1) {
        return false;
    }

    return true;
}

bool InputValidator::validate_phone_number(const std::string_view phone_number) {
    // 检查长度
    if (phone_number.length() != 11) {
        return false;
    }

    // 检查第一位是否为1
    if (phone_number[0] != '1') {
        return false;
    }

    // 检查所有字符是否为数字
    return std::all_of(phone_number.begin(), phone_number.end(), 
                      [](char ch) { return is_digit(ch); });
}

bool InputValidator::is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool InputValidator::is_lowercase(char ch) {
    return ch >= 'a' && ch <= 'z';
}

bool InputValidator::is_uppercase(char ch) {
    return ch >= 'A' && ch <= 'Z';
}

bool InputValidator::is_other_char(char ch) {
    // 不是数字，不是字母的字符
    return !is_digit(ch) && !is_lowercase(ch) && !is_uppercase(ch);
}
