/**
 * @file parts_info_dao.h
 * @brief 配件信息数据访问对象
 * @date 2025
 */

#ifndef PARTS_INFO_DAO_H
#define PARTS_INFO_DAO_H

#include "models/parts_info.h"
#include "repositories/base_dao.h"
#include <vector>

/**
 * @class PartsInfoDAO
 * @brief 配件信息数据访问对象类
 *
 * 提供配件信息的CRUD操作接口
 */
class PartsInfoDAO : public BaseDAO {
public:
    /**
     * @brief 插入配件信息
     * @param part 配件信息对象
     * @return bool 插入是否成功
     */
    bool insert(const PartsInfo& part);

    /**
     * @brief 根据ID删除配件信息
     * @param id 配件ID
     * @return bool 删除是否成功
     */
    bool remove(const std::string& id);

    /**
     * @brief 更新配件信息
     * @param part 配件信息对象
     * @return bool 更新是否成功
     */
    bool update(const PartsInfo& part);

    /**
     * @brief 根据ID查询配件信息
     * @param id 配件ID
     * @return PartsInfo 配件信息对象
     */
    PartsInfo find_by_id(const std::string& id);

    /**
     * @brief 查询所有配件信息
     * @return std::vector<PartsInfo> 配件信息列表
     */
    std::vector<PartsInfo> find_all();

    /**
     * @brief 根据型号查询配件信息
     * @param model 配件型号
     * @return std::vector<PartsInfo> 配件信息列表
     */
    std::vector<PartsInfo> find_by_model(const std::string& model);

    /**
     * @brief 根据名称查询配件信息
     * @param name 配件名称
     * @return std::vector<PartsInfo> 配件信息列表
     */
    std::vector<PartsInfo> find_by_name(const std::string& name);

    /**
     * @brief 根据分类查询配件信息
     * @param classification 配件分类
     * @return std::vector<PartsInfo> 配件信息列表
     */
    std::vector<PartsInfo> find_by_classification(const std::string& classification);

    /**
     * @brief 根据数量范围查询配件信息
     * @param min_number 最小数量
     * @param max_number 最大数量
     * @return std::vector<PartsInfo> 配件信息列表
     */
    std::vector<PartsInfo> find_by_number_range(int min_number, int max_number);

    /**
     * @brief 根据价格范围查询配件信息
     * @param min_price 最低价格
     * @param max_price 最高价格
     * @return std::vector<PartsInfo> 配件信息列表
     */
    std::vector<PartsInfo> find_by_price_range(double min_price, double max_price);

    /**
     * @brief 更新配件数量
     * @param id 配件ID
     * @param new_number 新的数量
     * @return bool 更新是否成功
     */
    bool update_number(const std::string& id, int new_number);

    /**
     * @brief 增加配件数量
     * @param id 配件ID
     * @param increment 增加的数量
     * @return bool 更新是否成功
     */
    bool increase_number(const std::string& id, int increment);

    /**
     * @brief 减少配件数量
     * @param id 配件ID
     * @param decrement 减少的数量
     * @return bool 更新是否成功
     */
    bool decrease_number(const std::string& id, int decrement);
};

#endif // PARTS_INFO_DAO_H
