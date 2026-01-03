#ifndef GENDER_H
#define GENDER_H

#include <string>

/**
 * @brief 性别模型类
 */
class Gender {
private:
    int id_;
    std::string sex_;

public:
    Gender(int id, const std::string& sex);
    
    int get_id(void) const;
    std::string get_sex(void) const;
    
    void set_id(int id);
    void set_sex(const std::string& sex);
};

#endif // GENDER_H
