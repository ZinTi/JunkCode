/**
 * @file password_tool.cpp
 * @brief 密码工具类的实现
 */

#include "utils/password_tool.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cstring>
#include <iostream>

// Base64 编码表
static const char BASE64_CHARS[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::optional<std::pair<std::string, std::string>> 
PasswordTool::generate_salt_and_hash(const std::string& plaintext_password) {
    try {
        // 生成盐值
        auto salt_opt = generate_salt(SALT_LENGTH_);
        if (!salt_opt.has_value()) {
            return std::nullopt;
        }
        auto salt = std::move(salt_opt.value());

        // 计算哈希值
        auto hash_opt = compute_sha256_hash(plaintext_password, salt);
        if (!hash_opt.has_value()) {
            return std::nullopt;
        }
        auto hash = std::move(hash_opt.value());

        // 编码为 base64
        std::string salt_base64 = base64_encode(salt);
        std::string hash_base64 = base64_encode(hash);

        return std::make_pair(salt_base64, hash_base64);
    } catch (const std::exception& e) {
        // 异常安全：不泄露任何信息
        return std::nullopt;
    }
}

bool PasswordTool::verify_password(
    const std::string& plaintext_password_to_check,
    const std::string& stored_hash,
    const std::string& salt) {
    
    // 解码盐值
    auto salt_bytes_opt = base64_decode(salt);
    if (!salt_bytes_opt.has_value()) {
        return false;
    }
    
    // 解码存储的哈希值
    auto stored_hash_bytes_opt = base64_decode(stored_hash);
    if (!stored_hash_bytes_opt.has_value()) {
        return false;
    }

    // 计算待验证密码的哈希值
    auto computed_hash_opt = compute_sha256_hash(
        plaintext_password_to_check, 
        salt_bytes_opt.value());
    
    if (!computed_hash_opt.has_value()) {
        return false;
    }

    // 比较哈希值（使用恒定时间比较以防止时序攻击）
    const auto& computed_hash = computed_hash_opt.value();
    const auto& stored_hash_bytes = stored_hash_bytes_opt.value();
    
    // std::cout << "computed_hash: " ;
    // for(const auto ch : computed_hash){
    //     std::cout << ch;
    // }
    // std::cout << std::endl;

    // std::cout << "stored_hash_bytes: " ;
    // for(const auto ch : stored_hash_bytes){
    //     std::cout << ch;
    // }
    // std::cout << std::endl;

    if (computed_hash.size() != stored_hash_bytes.size()) {
        return false;
    }

    // 使用恒定时间比较
    unsigned char result = 0;
    for (size_t i = 0; i < computed_hash.size(); ++i) {
        result |= computed_hash[i] ^ stored_hash_bytes[i];
    }

    return result == 0;
}

std::optional<std::vector<unsigned char>> 
PasswordTool::generate_salt(size_t salt_length) {
    std::vector<unsigned char> salt(salt_length);
    
    // 使用 OpenSSL 的加密安全随机数生成器
    if (RAND_bytes(salt.data(), static_cast<int>(salt_length)) != 1) {
        return std::nullopt;
    }
    
    return salt;
}

std::optional<std::vector<unsigned char>> 
PasswordTool::compute_sha256_hash(
    const std::string& plaintext_password,
    const std::vector<unsigned char>& salt) {
    
    std::vector<unsigned char> hash(SHA256_DIGEST_LENGTH);
    
    // 创建 SHA256 上下文
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (context == nullptr) {
        return std::nullopt;
    }

    // 初始化 SHA256 哈希计算
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(context);
        return std::nullopt;
    }

    // 修复：确保密码和盐值的处理顺序一致
    // 先处理盐值，再处理密码，保持生成和验证时的一致性
    if (EVP_DigestUpdate(context, salt.data(), salt.size()) != 1) {
        EVP_MD_CTX_free(context);
        return std::nullopt;
    }

    if (EVP_DigestUpdate(context, 
                        plaintext_password.c_str(), 
                        plaintext_password.length()) != 1) {
        EVP_MD_CTX_free(context);
        return std::nullopt;
    }

    // 完成哈希计算
    unsigned int length = 0;
    if (EVP_DigestFinal_ex(context, hash.data(), &length) != 1 || 
        length != SHA256_DIGEST_LENGTH) {
        EVP_MD_CTX_free(context);
        return std::nullopt;
    }

    EVP_MD_CTX_free(context);
    return hash;
}

std::string PasswordTool::base64_encode(const std::vector<unsigned char>& data) {
    std::string result;
    size_t input_length = data.size();
    
    // Base64 编码：每 3 个字节转换为 4 个字符
    for (size_t i = 0; i < input_length; i += 3) {
        uint32_t octet_a = i < input_length ? data[i] : 0;
        uint32_t octet_b = i + 1 < input_length ? data[i + 1] : 0;
        uint32_t octet_c = i + 2 < input_length ? data[i + 2] : 0;
        
        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;
        
        result.push_back(BASE64_CHARS[(triple >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 12) & 0x3F]);
        
        // 修复：正确处理填充
        if (i + 1 < input_length) {
            result.push_back(BASE64_CHARS[(triple >> 6) & 0x3F]);
        } else {
            result.push_back('=');
        }
        
        if (i + 2 < input_length) {
            result.push_back(BASE64_CHARS[triple & 0x3F]);
        } else {
            result.push_back('=');
        }
    }
    
    return result;
}

std::optional<std::vector<unsigned char>> 
PasswordTool::base64_decode(const std::string& encoded_string) {
    std::vector<unsigned char> result;
    size_t input_length = encoded_string.length();
    
    // 检查字符串长度是否合法
    if (input_length % 4 != 0) {
        return std::nullopt;
    }
    
    // 计算填充字符数量
    size_t padding_count = 0;
    if (input_length > 0) {
        if (encoded_string[input_length - 1] == '=') padding_count++;
        if (input_length > 1 && encoded_string[input_length - 2] == '=') padding_count++;
    }
    
    // 准备解码表 - 修复：正确初始化解码表
    int decoding_table[256];
    std::memset(decoding_table, -1, sizeof(decoding_table));
    for (int i = 0; i < 64; ++i) {
        decoding_table[static_cast<unsigned char>(BASE64_CHARS[i])] = i;
    }
    // 修复：不要将'='映射到0，保持为-1
    // 在解码逻辑中单独处理'='字符
    
    // Base64 解码：每 4 个字符转换为 3 个字节
    for (size_t i = 0; i < input_length; i += 4) {
        // 修复：正确处理填充字符
        int sextet_a = (encoded_string[i] == '=') ? 0 : decoding_table[static_cast<unsigned char>(encoded_string[i])];
        int sextet_b = (encoded_string[i + 1] == '=') ? 0 : decoding_table[static_cast<unsigned char>(encoded_string[i + 1])];
        int sextet_c = (encoded_string[i + 2] == '=') ? 0 : decoding_table[static_cast<unsigned char>(encoded_string[i + 2])];
        int sextet_d = (encoded_string[i + 3] == '=') ? 0 : decoding_table[static_cast<unsigned char>(encoded_string[i + 3])];
        
        // 检查是否有无效字符（除了填充字符）
        if ((sextet_a == -1 && encoded_string[i] != '=') ||
            (sextet_b == -1 && encoded_string[i + 1] != '=') ||
            (sextet_c == -1 && encoded_string[i + 2] != '=') ||
            (sextet_d == -1 && encoded_string[i + 3] != '=')) {
            return std::nullopt;
        }
        
        uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;
        
        // 修复：正确输出解码结果，考虑填充
        result.push_back(static_cast<unsigned char>((triple >> 16) & 0xFF));
        if (encoded_string[i + 2] != '=') {
            result.push_back(static_cast<unsigned char>((triple >> 8) & 0xFF));
        }
        if (encoded_string[i + 3] != '=') {
            result.push_back(static_cast<unsigned char>(triple & 0xFF));
        }
    }
    
    // 修复：正确计算输出长度，考虑填充
    size_t expected_length = (input_length / 4) * 3 - padding_count;
    if (result.size() != expected_length) {
        return std::nullopt;
    }
    
    return result;
}
