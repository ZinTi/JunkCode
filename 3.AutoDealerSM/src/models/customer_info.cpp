#include "models/customer_info.h"
#include <sstream>

CustomerInfo::CustomerInfo()
    : id_("(无)"), name_("(无)"), salt_("(无)"), pwd_("(无)"),
      gender_('M'), email_(""), phone_(""), address_(""),
      zip_code_(""), car_model_(""), car_condition_(""), car_id_("") {
}

CustomerInfo::CustomerInfo(const std::string& id, const std::string& name,
                           const std::string& salt, const std::string& pwd,
                           char gender, const std::string& email,
                           const std::string& phone, const std::string& address,
                           const std::string& zip_code, const std::string& car_model,
                           const std::string& car_condition, const std::string& car_id)
    : id_(id), name_(name), salt_(salt), pwd_(pwd), gender_(gender),
      email_(email), phone_(phone), address_(address), zip_code_(zip_code),
      car_model_(car_model), car_condition_(car_condition), car_id_(car_id) {
}

// Getter implementations
std::string CustomerInfo::get_id() const { return id_; }
std::string CustomerInfo::get_name() const { return name_; }
std::string CustomerInfo::get_salt() const { return salt_; }
std::string CustomerInfo::get_pwd() const { return pwd_; }
char CustomerInfo::get_gender() const { return gender_; }
std::string CustomerInfo::get_email() const { return email_; }
std::string CustomerInfo::get_phone() const { return phone_; }
std::string CustomerInfo::get_address() const { return address_; }
std::string CustomerInfo::get_zip_code() const { return zip_code_; }
std::string CustomerInfo::get_car_model() const { return car_model_; }
std::string CustomerInfo::get_car_condition() const { return car_condition_; }
std::string CustomerInfo::get_car_id() const { return car_id_; }

// Setter implementations
void CustomerInfo::set_id(const std::string& id) { id_ = id; }
void CustomerInfo::set_name(const std::string& name) { name_ = name; }
void CustomerInfo::set_salt(const std::string& salt) { salt_ = salt; }
void CustomerInfo::set_pwd(const std::string& pwd) { pwd_ = pwd; }
void CustomerInfo::set_gender(char gender) { gender_ = gender; }
void CustomerInfo::set_email(const std::string& email) { email_ = email; }
void CustomerInfo::set_phone(const std::string& phone) { phone_ = phone; }
void CustomerInfo::set_address(const std::string& address) { address_ = address; }
void CustomerInfo::set_zip_code(const std::string& zip_code) { zip_code_ = zip_code; }
void CustomerInfo::set_car_model(const std::string& car_model) { car_model_ = car_model; }
void CustomerInfo::set_car_condition(const std::string& car_condition) { car_condition_ = car_condition; }
void CustomerInfo::set_car_id(const std::string& car_id) { car_id_ = car_id; }

std::string CustomerInfo::to_json() const {
    std::stringstream ss;
    ss << "{";
    ss << "\"id\":\"" << id_ << "\",";
    ss << "\"name\":\"" << name_ << "\",";
    ss << "\"salt\":\"" << salt_ << "\",";
    ss << "\"pwd\":\"" << pwd_ << "\",";
    ss << "\"gender\":\"" << gender_ << "\",";
    ss << "\"email\":\"" << email_ << "\",";
    ss << "\"phone\":\"" << phone_ << "\",";
    ss << "\"address\":\"" << address_ << "\",";
    ss << "\"zip_code\":\"" << zip_code_ << "\",";
    ss << "\"car_model\":\"" << car_model_ << "\",";
    ss << "\"car_condition\":\"" << car_condition_ << "\",";
    ss << "\"car_id\":\"" << car_id_ << "\"";
    ss << "}";
    return ss.str();
}
