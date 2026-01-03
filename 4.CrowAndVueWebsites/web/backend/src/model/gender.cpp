#include "model/gender.h"

Gender::Gender(int id, const std::string& sex) : id_(id), sex_(sex) {}

int Gender::get_id(void) const {
    return id_;
}

std::string Gender::get_sex(void) const {
    return sex_;
}

void Gender::set_id(int id) {
    id_ = id;
}

void Gender::set_sex(const std::string& sex) {
    sex_ = sex;
}
