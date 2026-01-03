#include "models/beauty_maintenance_proj.h"
#include <sstream>
#include <iomanip>

BeautyMaintenanceProj::BeautyMaintenanceProj()
    : id_("(无)"), proj_name_("(无)"), price_(0.0), group_id_("(无)") {
}

BeautyMaintenanceProj::BeautyMaintenanceProj(const std::string& id, const std::string& proj_name,
                                             double price, const std::string& group_id)
    : id_(id), proj_name_(proj_name), price_(price), group_id_(group_id) {
}

// Getter implementations
std::string BeautyMaintenanceProj::get_id() const { return id_; }
std::string BeautyMaintenanceProj::get_proj_name() const { return proj_name_; }
double BeautyMaintenanceProj::get_price() const { return price_; }
std::string BeautyMaintenanceProj::get_group_id() const { return group_id_; }

// Setter implementations
void BeautyMaintenanceProj::set_id(const std::string& id) { id_ = id; }
void BeautyMaintenanceProj::set_proj_name(const std::string& proj_name) { proj_name_ = proj_name; }
void BeautyMaintenanceProj::set_price(double price) { price_ = price; }
void BeautyMaintenanceProj::set_group_id(const std::string& group_id) { group_id_ = group_id; }

std::string BeautyMaintenanceProj::to_json() const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "{";
    ss << "\"id\":\"" << id_ << "\",";
    ss << "\"proj_name\":\"" << proj_name_ << "\",";
    ss << "\"price\":" << price_ << ",";
    ss << "\"group_id\":\"" << group_id_ << "\"";
    ss << "}";
    return ss.str();
}
