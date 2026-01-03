#include "model/user.h"

User::User(int id, const std::string& username, const std::string& nickname, 
           const std::string& salt, const std::string& passwd, 
           const std::string& email, int gender_id) 
    : id_(id), username_(username), nickname_(nickname), salt_(salt), 
      passwd_(passwd), email_(email), gender_id_(gender_id) {}

int User::get_id(void) const {
    return id_;
}

std::string User::get_username(void) const {
    return username_;
}

std::string User::get_nickname(void) const {
    return nickname_;
}

std::string User::get_salt(void) const {
    return salt_;
}

std::string User::get_passwd(void) const {
    return passwd_;
}

std::string User::get_email(void) const {
    return email_;
}

int User::get_gender_id(void) const {
    return gender_id_;
}

void User::set_id(int id) {
    id_ = id;
}

void User::set_username(const std::string& username) {
    username_ = username;
}

void User::set_nickname(const std::string& nickname) {
    nickname_ = nickname;
}

void User::set_salt(const std::string& salt) {
    salt_ = salt;
}

void User::set_passwd(const std::string& passwd) {
    passwd_ = passwd;
}

void User::set_email(const std::string& email) {
    email_ = email;
}

void User::set_gender_id(int gender_id) {
    gender_id_ = gender_id;
}

void User::set_empty(){
    id_ = 0;
    username_ = "";
    nickname_ = "";
    salt_ = "";
    passwd_ = "";
    email_ = "";
    gender_id_ = 0;
}