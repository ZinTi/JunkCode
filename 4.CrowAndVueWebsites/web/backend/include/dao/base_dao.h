#ifndef BASE_DAO_H
#define BASE_DAO_H

#include <string>
#include <memory>

extern "C" {
    #include <sqlite3.h>
}

/**
 * @brief 基础数据访问对象类
 */
class BaseDao {
protected:
    std::shared_ptr<sqlite3> db_;

    /**
     * @brief 打开数据库连接
     * @param db_path 数据库文件路径
     * @return 成功返回true，失败返回false
     */
    bool open_database(const std::string& db_path);

    /**
     * @brief 执行SQL语句
     * @param sql SQL语句
     * @return 成功返回true，失败返回false
     */
    bool execute_sql(const std::string& sql);

    /**
     * @brief 初始化数据库表结构
     * @return 成功返回true，失败返回false
     */
    bool initialize_database(void);

public:
    BaseDao(void);
    virtual ~BaseDao(void) = default;
};

#endif // BASE_DAO_H
