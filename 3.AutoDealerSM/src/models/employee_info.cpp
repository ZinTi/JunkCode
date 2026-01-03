#include "models/employee_info.h"
#include <sstream>
#include <iomanip>

EmployeeInfo::EmployeeInfo()
    : id_("(无)"), name_("(无)"), gender_('M'), email_(""), phone_(""),
      address_(""), zip_code_(""), group_id_(""), monthly_working_hours_(0.0),
      monthly_salary_(0.0) {
}

EmployeeInfo::EmployeeInfo(const std::string& id, const std::string& name,
                           char gender, const std::string& email,
                           const std::string& phone, const std::string& address,
                           const std::string& zip_code, const std::string& group_id,
                           double monthly_working_hours, double monthly_salary)
    : id_(id), name_(name), gender_(gender), email_(email), phone_(phone),
      address_(address), zip_code_(zip_code), group_id_(group_id),
      monthly_working_hours_(monthly_working_hours), monthly_salary_(monthly_salary) {
}

// Getter implementations
std::string EmployeeInfo::get_id() const { return id_; }
std::string EmployeeInfo::get_name() const { return name_; }
char EmployeeInfo::get_gender() const { return gender_; }
std::string EmployeeInfo::get_email() const { return email_; }
std::string EmployeeInfo::get_phone() const { return phone_; }
std::string EmployeeInfo::get_address() const { return address_; }
std::string EmployeeInfo::get_zip_code() const { return zip_code_; }
std::string EmployeeInfo::get_group_id() const { return group_id_; }
double EmployeeInfo::get_monthly_working_hours() const { return monthly_working_hours_; }
double EmployeeInfo::get_monthly_salary() const { return monthly_salary_; }

// Setter implementations
void EmployeeInfo::set_id(const std::string& id) { id_ = id; }
void EmployeeInfo::set_name(const std::string& name) { name_ = name; }
void EmployeeInfo::set_gender(char gender) { gender_ = gender; }
void EmployeeInfo::set_email(const std::string& email) { email_ = email; }
void EmployeeInfo::set_phone(const std::string& phone) { phone_ = phone; }
void EmployeeInfo::set_address(const std::string& address) { address_ = address; }
void EmployeeInfo::set_zip_code(const std::string& zip_code) { zip_code_ = zip_code; }
void EmployeeInfo::set_group_id(const std::string& group_id) { group_id_ = group_id; }
void EmployeeInfo::set_monthly_working_hours(double monthly_working_hours) { monthly_working_hours_ = monthly_working_hours; }
void EmployeeInfo::set_monthly_salary(double monthly_salary) { monthly_salary_ = monthly_salary; }

std::string EmployeeInfo::to_json() const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "{";
    ss << "\"id\":\"" << id_ << "\",";
    ss << "\"name\":\"" << name_ << "\",";
    ss << "\"gender\":\"" << gender_ << "\",";
    ss << "\"email\":\"" << email_ << "\",";
    ss << "\"phone\":\"" << phone_ << "\",";
    ss << "\"address\":\"" << address_ << "\",";
    ss << "\"zip_code\":\"" << zip_code_ << "\",";
    ss << "\"group_id\":\"" << group_id_ << "\",";
    ss << "\"monthly_working_hours\":" << monthly_working_hours_ << ",";
    ss << "\"monthly_salary\":" << monthly_salary_;
    ss << "}";
    return ss.str();
}
