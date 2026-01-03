/**
* @file base_dao.h
 * @brief 基础DAO接口类
 * @date 2025
 */

#ifndef BASE_DAO_H
#define BASE_DAO_H

extern "C" {
#include <sqlite3.h>
}
#include <memory>
#include <string>
#include <vector>
#include "repositories/sqlite_connection_pool.h"

/**
 * @class BaseDAO
 * @brief 基础数据访问对象接口
 *
 * 提供通用的数据库操作接口，所有具体DAO类都应继承此类
 */
class BaseDAO {
protected:
    /**
     * @brief 获取数据库连接
     * @return std::shared_ptr<sqlite3> 数据库连接
     */
    std::shared_ptr<sqlite3> get_connection();

    /**
     * @brief 执行SQL查询语句
     * @param sql SQL语句
     * @param callback 回调函数
     * @param data 回调数据
     * @return int 执行结果
     */
    int execute_query(const std::string& sql,
                     int (*callback)(void*, int, char**, char**),
                     void* data);

    /**
     * @brief 执行SQL更新语句
     * @param sql SQL语句
     * @return bool 执行是否成功
     */
    bool execute_update(const std::string& sql);

    /**
     * @brief 执行带参数的SQL更新语句
     * @param sql SQL语句
     * @param params 参数向量
     * @return bool 执行是否成功
     */
    bool execute_update_with_params(const std::string& sql, const std::vector<std::string>& params);

    /**
     * @brief 检查记录是否存在
     * @param table 表名
     * @param column 列名
     * @param value 要检查的值
     * @return bool 是否存在
     */
    bool exists(const std::string& table, const std::string& column, const std::string& value);
};

#endif // BASE_DAO_H
