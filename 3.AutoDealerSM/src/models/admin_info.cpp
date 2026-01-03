#include "models/admin_info.h"
#include <sstream>

AdminInfo::AdminInfo()
    : id_("(无)"), name_("(无)"), salt_("(无)"), pwd_("(无)") {
}

AdminInfo::AdminInfo(const std::string& id, const std::string& name,
                     const std::string& salt, const std::string& pwd)
    : id_(id), name_(name), salt_(salt), pwd_(pwd) {
}

// Getter implementations
std::string AdminInfo::get_id() const { return id_; }
std::string AdminInfo::get_name() const { return name_; }
std::string AdminInfo::get_salt() const { return salt_; }
std::string AdminInfo::get_pwd() const { return pwd_; }

// Setter implementations
void AdminInfo::set_id(const std::string& id) { id_ = id; }
void AdminInfo::set_name(const std::string& name) { name_ = name; }
void AdminInfo::set_salt(const std::string& salt) { salt_ = salt; }
void AdminInfo::set_pwd(const std::string& pwd) { pwd_ = pwd; }

std::string AdminInfo::to_json() const {
    std::stringstream ss;
    ss << "{";
    ss << "\"id\":\"" << id_ << "\",";
    ss << "\"name\":\"" << name_ << "\",";
    ss << "\"salt\":\"" << salt_ << "\",";
    ss << "\"pwd\":\"" << pwd_ << "\"";
    ss << "}";
    return ss.str();
}
