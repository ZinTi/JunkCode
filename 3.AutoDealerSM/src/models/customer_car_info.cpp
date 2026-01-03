#include "models/customer_car_info.h"
#include <sstream>

CustomerCarInfo::CustomerCarInfo()
    : id_("(无)"), condition_("(无)"), color_("(无)"),
      cust_name_("(无)"), model_("(无)") {
}

CustomerCarInfo::CustomerCarInfo(const std::string& id, const std::string& condition, const std::string& color, const std::string& cust_name, const std::string& model)
    : id_(id), condition_(condition), color_(color),
      cust_name_(cust_name), model_(model) {
}

// Getter implementations
std::string CustomerCarInfo::get_id() const { return id_; }
std::string CustomerCarInfo::get_condition() const { return condition_; }
std::string CustomerCarInfo::get_color() const { return color_; }
std::string CustomerCarInfo::get_cust_name() const { return cust_name_; }
std::string CustomerCarInfo::get_model() const { return model_; }

// Setter implementations
void CustomerCarInfo::set_id(const std::string& id) { id_ = id; }
void CustomerCarInfo::set_condition(const std::string& condition) { condition_ = condition; }
void CustomerCarInfo::set_color(const std::string& color) { color_ = color; }
void CustomerCarInfo::set_cust_name(const std::string& cust_name) { cust_name_ = cust_name; }
void CustomerCarInfo::set_model(const std::string& model) { model_ = model; }

std::string CustomerCarInfo::to_json() const {
    std::stringstream ss;
    ss << "{";
    ss << "\"id\":\"" << id_ << "\",";
    ss << "\"condition\":\"" << condition_ << "\",";
    ss << "\"color\":\"" << color_ << "\",";
    ss << "\"cust_name\":\"" << cust_name_ << "\",";
    ss << "\"model\":\"" << model_ << "\"";
    ss << "}";
    return ss.str();
}


