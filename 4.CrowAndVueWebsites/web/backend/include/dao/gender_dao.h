#ifndef GENDER_DAO_H
#define GENDER_DAO_H

#include "dao/base_dao.h"
#include "model/gender.h"
#include <vector>
#include <memory>

/**
 * @brief 性别数据访问对象类
 */
class GenderDao : public BaseDao {
public:
    GenderDao(void) = default;
    
    /**
     * @brief 获取所有性别信息
     * @return 性别对象向量
     */
    std::vector<Gender> get_all_genders(void);
    
    /**
     * @brief 根据ID获取性别信息
     * @param id 性别ID
     * @return 性别对象，如果不存在返回空对象
     */
    std::unique_ptr<Gender> get_gender_by_id(int id);
    
    /**
     * @brief 根据性别描述获取性别信息
     * @param sex 性别描述
     * @return 性别对象，如果不存在返回空对象
     */
    std::unique_ptr<Gender> get_gender_by_sex(const std::string& sex);
};

#endif // GENDER_DAO_H
