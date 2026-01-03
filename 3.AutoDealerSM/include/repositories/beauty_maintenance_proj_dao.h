/**
 * @file beauty_maintenance_proj_dao.h
 * @brief 美容保养项目数据访问对象
 * @date 2025
 */

#ifndef BEAUTY_MAINTENANCE_PROJ_DAO_H
#define BEAUTY_MAINTENANCE_PROJ_DAO_H

#include "models/beauty_maintenance_proj.h"
#include "repositories/base_dao.h"
#include <vector>

/**
 * @class BeautyMaintenanceProjDAO
 * @brief 美容保养项目数据访问对象类
 *
 * 提供美容保养项目的CRUD操作接口
 */
class BeautyMaintenanceProjDAO : public BaseDAO {
public:
    /**
     * @brief 插入美容保养项目信息
     * @param proj 美容保养项目对象
     * @return bool 插入是否成功
     */
    bool insert(const BeautyMaintenanceProj& proj);

    /**
     * @brief 根据ID删除美容保养项目信息
     * @param id 项目ID
     * @return bool 删除是否成功
     */
    bool remove(const std::string& id);

    /**
     * @brief 更新美容保养项目信息
     * @param proj 美容保养项目对象
     * @return bool 更新是否成功
     */
    bool update(const BeautyMaintenanceProj& proj);

    /**
     * @brief 根据ID查询美容保养项目信息
     * @param id 项目ID
     * @return BeautyMaintenanceProj 美容保养项目对象
     */
    BeautyMaintenanceProj find_by_id(const std::string& id);

    /**
     * @brief 查询所有美容保养项目信息
     * @return std::vector<BeautyMaintenanceProj> 美容保养项目列表
     */
    std::vector<BeautyMaintenanceProj> find_all();

    /**
     * @brief 根据班组ID查询美容保养项目信息
     * @param group_id 班组ID
     * @return std::vector<BeautyMaintenanceProj> 美容保养项目列表
     */
    std::vector<BeautyMaintenanceProj> find_by_group_id(const std::string& group_id);

    /**
     * @brief 根据项目名称查询美容保养项目信息
     * @param proj_name 项目名称
     * @return std::vector<BeautyMaintenanceProj> 美容保养项目列表
     */
    std::vector<BeautyMaintenanceProj> find_by_proj_name(const std::string& proj_name);

    /**
     * @brief 根据价格范围查询美容保养项目信息
     * @param min_price 最低价格
     * @param max_price 最高价格
     * @return std::vector<BeautyMaintenanceProj> 美容保养项目列表
     */
    std::vector<BeautyMaintenanceProj> find_by_price_range(double min_price, double max_price);
};

#endif // BEAUTY_MAINTENANCE_PROJ_DAO_H
