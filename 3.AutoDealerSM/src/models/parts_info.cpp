#include "models/parts_info.h"
#include <sstream>
#include <iomanip>

PartsInfo::PartsInfo()
    : id_("(无)"), model_("(无)"), name_(""), number_(0),
      classification_(""), price_(0.0), remarks_("") {
}

PartsInfo::PartsInfo(const std::string& id, const std::string& model,
                     const std::string& name, int number,
                     const std::string& classification, double price,
                     const std::string& remarks)
    : id_(id), model_(model), name_(name), number_(number),
      classification_(classification), price_(price), remarks_(remarks) {
}

// Getter implementations
std::string PartsInfo::get_id() const { return id_; }
std::string PartsInfo::get_model() const { return model_; }
std::string PartsInfo::get_name() const { return name_; }
int PartsInfo::get_number() const { return number_; }
std::string PartsInfo::get_classification() const { return classification_; }
double PartsInfo::get_price() const { return price_; }
std::string PartsInfo::get_remarks() const { return remarks_; }

// Setter implementations
void PartsInfo::set_id(const std::string& id) { id_ = id; }
void PartsInfo::set_model(const std::string& model) { model_ = model; }
void PartsInfo::set_name(const std::string& name) { name_ = name; }
void PartsInfo::set_number(int number) { number_ = number; }
void PartsInfo::set_classification(const std::string& classification) { classification_ = classification; }
void PartsInfo::set_price(double price) { price_ = price; }
void PartsInfo::set_remarks(const std::string& remarks) { remarks_ = remarks; }

std::string PartsInfo::to_json() const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "{";
    ss << "\"id\":\"" << id_ << "\",";
    ss << "\"model\":\"" << model_ << "\",";
    ss << "\"name\":\"" << name_ << "\",";
    ss << "\"number\":" << number_ << ",";
    ss << "\"classification\":\"" << classification_ << "\",";
    ss << "\"price\":" << price_ << ",";
    ss << "\"remarks\":\"" << remarks_ << "\"";
    ss << "}";
    return ss.str();
}
