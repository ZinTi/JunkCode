/**
 * @file customer_info_dao.h
 * @brief 客户信息数据访问对象
 * @date 2025
 */

#ifndef CUSTOMER_INFO_DAO_H
#define CUSTOMER_INFO_DAO_H

#include "models/customer_info.h"
#include "repositories/base_dao.h"
#include <vector>

/**
 * @class CustomerInfoDAO
 * @brief 客户信息数据访问对象类
 *
 * 提供客户信息的CRUD操作接口
 */
class CustomerInfoDAO : public BaseDAO {
public:
    /**
     * @brief 插入客户信息
     * @param customer 客户信息对象
     * @return bool 插入是否成功
     */
    bool insert(const CustomerInfo& customer);

    /**
     * @brief 根据ID删除客户信息
     * @param id 客户ID
     * @return bool 删除是否成功
     */
    bool remove(const std::string& id);

    /**
     * @brief 更新客户信息
     * @param customer 客户信息对象
     * @return bool 更新是否成功
     */
    bool update(const CustomerInfo& customer);

    /**
     * @brief 根据ID查询客户信息
     * @param id 客户ID
     * @return CustomerInfo 客户信息对象
     */
    CustomerInfo find_by_id(const std::string& id);

    /**
     * @brief 查询所有客户信息
     * @return std::vector<CustomerInfo> 客户信息列表
     */
    std::vector<CustomerInfo> find_all();

    /**
     * @brief 根据姓名查询客户信息
     * @param name 客户姓名
     * @return std::vector<CustomerInfo> 客户信息列表
     */
    std::vector<CustomerInfo> find_by_name(const std::string& name);

    /**
     * @brief 根据性别查询客户信息
     * @param gender 客户性别
     * @return std::vector<CustomerInfo> 客户信息列表
     */
    std::vector<CustomerInfo> find_by_gender(char gender);

    /**
     * @brief 根据邮箱查询客户信息
     * @param email 客户邮箱
     * @return std::vector<CustomerInfo> 客户信息列表
     */
    std::vector<CustomerInfo> find_by_email(const std::string& email);

    /**
     * @brief 根据电话号码查询客户信息
     * @param phone 电话号码
     * @return std::vector<CustomerInfo> 客户信息列表
     */
    std::vector<CustomerInfo> find_by_phone(const std::string& phone);

    /**
     * @brief 根据车辆型号查询客户信息
     * @param car_model 车辆型号
     * @return std::vector<CustomerInfo> 客户信息列表
     */
    std::vector<CustomerInfo> find_by_car_model(const std::string& car_model);

    /**
     * @brief 根据车辆状况查询客户信息
     * @param car_condition 车辆状况
     * @return std::vector<CustomerInfo> 客户信息列表
     */
    std::vector<CustomerInfo> find_by_car_condition(const std::string& car_condition);

    /**
     * @brief 检查ID是否存在
     * @param id 要检查的客户ID
     * @return bool ID是否存在
     */
    bool id_exists(const std::string& id);
};

#endif // CUSTOMER_INFO_DAO_H
