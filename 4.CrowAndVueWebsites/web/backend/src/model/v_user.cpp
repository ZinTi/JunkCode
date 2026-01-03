#include "model/v_user.h"

VUser::VUser(int id, const std::string& username, const std::string& nickname, 
             const std::string& email, const std::string& sex) 
    : id_(id), username_(username), nickname_(nickname), email_(email), sex_(sex) {}

int VUser::get_id(void) const {
    return id_;
}

std::string VUser::get_username(void) const {
    return username_;
}

std::string VUser::get_nickname(void) const {
    return nickname_;
}

std::string VUser::get_email(void) const {
    return email_;
}

std::string VUser::get_sex(void) const {
    return sex_;
}
