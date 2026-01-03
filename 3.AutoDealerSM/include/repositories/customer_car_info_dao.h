/**
* @file customer_car_info_dao.h
 * @brief 客户车辆信息数据访问对象
 * @date 2025
 */

#ifndef CUSTOMER_CAR_INFO_DAO_H
#define CUSTOMER_CAR_INFO_DAO_H

#include "models/customer_car_info.h"
#include "repositories/base_dao.h"
#include <vector>

/**
 * @class CustomerCarInfoDAO
 * @brief 客户车辆信息数据访问对象类
 *
 * 提供客户车辆信息的CRUD操作接口
 */
class CustomerCarInfoDAO : public BaseDAO {
public:
    /**
     * @brief 插入客户车辆信息
     * @param car_info 客户车辆信息对象
     * @return bool 插入是否成功
     */
    bool insert(const CustomerCarInfo& car_info);

    /**
     * @brief 根据ID删除客户车辆信息
     * @param id 车辆ID
     * @return bool 删除是否成功
     */
    bool remove(const std::string& id);

    /**
     * @brief 更新客户车辆信息
     * @param car_info 客户车辆信息对象
     * @return bool 更新是否成功
     */
    bool update(const CustomerCarInfo& car_info);

    /**
     * @brief 根据ID查询客户车辆信息
     * @param id 车辆ID
     * @return CustomerCarInfo 客户车辆信息对象
     */
    CustomerCarInfo find_by_id(const std::string& id);

    /**
     * @brief 查询所有客户车辆信息
     * @return std::vector<CustomerCarInfo> 客户车辆信息列表
     */
    std::vector<CustomerCarInfo> find_all();

    /**
     * @brief 根据客户姓名查询车辆信息
     * @param cust_name 客户姓名
     * @return std::vector<CustomerCarInfo> 客户车辆信息列表
     */
    std::vector<CustomerCarInfo> find_by_cust_name(const std::string& cust_name);

    /**
     * @brief 根据车辆型号查询车辆信息
     * @param model 车辆型号
     * @return std::vector<CustomerCarInfo> 客户车辆信息列表
     */
    std::vector<CustomerCarInfo> find_by_model(const std::string& model);

    /**
     * @brief 根据车辆状况查询车辆信息
     * @param condition 车辆状况
     * @return std::vector<CustomerCarInfo> 客户车辆信息列表
     */
    std::vector<CustomerCarInfo> find_by_condition(const std::string& condition);

    /**
     * @brief 根据车辆颜色查询车辆信息
     * @param color 车辆颜色
     * @return std::vector<CustomerCarInfo> 客户车辆信息列表
     */
    std::vector<CustomerCarInfo> find_by_color(const std::string& color);
};

#endif // CUSTOMER_CAR_INFO_DAO_H
