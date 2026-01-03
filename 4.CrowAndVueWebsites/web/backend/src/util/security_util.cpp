#include "util/security_util.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

std::string SecurityUtil::generate_salt(void) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

std::string SecurityUtil::hash_password(const std::string& password, const std::string& salt) {
    std::string data = password + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), hash);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

bool SecurityUtil::verify_password(const std::string& password, const std::string& hashed_password, const std::string& salt) {
    return hash_password(password, salt) == hashed_password;
}
