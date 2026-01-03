/**
* @file admin_info_dao.h
 * @brief 管理员信息数据访问对象
 * @date 2025
 */

#ifndef ADMIN_INFO_DAO_H
#define ADMIN_INFO_DAO_H

#include "models/admin_info.h"
#include "repositories/base_dao.h"
#include <vector>

/**
 * @class AdminInfoDAO
 * @brief 管理员信息数据访问对象类
 *
 * 提供管理员信息的CRUD操作接口
 */
class AdminInfoDAO : public BaseDAO {
public:
    /**
     * @brief 插入管理员信息
     * @param admin 管理员信息对象
     * @return bool 插入是否成功
     */
    bool insert(const AdminInfo& admin);

    /**
     * @brief 根据ID删除管理员信息
     * @param id 管理员ID
     * @return bool 删除是否成功
     */
    bool remove(const std::string& id);

    /**
     * @brief 更新管理员信息
     * @param admin 管理员信息对象
     * @return bool 更新是否成功
     */
    bool update(const AdminInfo& admin);

    /**
     * @brief 根据ID查询管理员信息
     * @param id 管理员ID
     * @return AdminInfo 管理员信息对象
     */
    AdminInfo find_by_id(const std::string& id);

    /**
     * @brief 查询所有管理员信息
     * @return std::vector<AdminInfo> 管理员信息列表
     */
    std::vector<AdminInfo> find_all();

    /**
     * @brief 根据姓名查询管理员信息
     * @param name 管理员姓名
     * @return std::vector<AdminInfo> 管理员信息列表
     */
    std::vector<AdminInfo> find_by_name(const std::string& name);

    /**
     * @brief 检查ID是否存在
     * @param id 要检查的管理员ID
     * @return bool ID是否存在
     */
    bool id_exists(const std::string& id);
};

#endif // ADMIN_INFO_DAO_H
