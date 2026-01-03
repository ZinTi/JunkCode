/**
* @file sqlite_connection_pool.h
 * @brief SQLite数据库连接池管理类
 * @date 2025
 */

#ifndef SQLITE_CONNECTION_POOL_H
#define SQLITE_CONNECTION_POOL_H

extern "C" {
    #include <sqlite3.h>
}
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <string>

/**
 * @class SQLiteConnectionPool
 * @brief SQLite数据库连接池单例类
 *
 * 管理数据库连接的创建、分配和回收，确保线程安全和高性能访问
 */
class SQLiteConnectionPool {
public:
    /**
     * @brief 获取连接池单例实例
     * @return SQLiteConnectionPool& 连接池单例引用
     */
    static SQLiteConnectionPool& get_instance();

    /**
     * @brief 初始化连接池
     * @param dbPath 数据库文件路径
     * @param poolSize 连接池大小
     * @return bool 初始化是否成功
     */
    bool initialize(const std::string& dbPath, int poolSize = 10);

    /**
     * @brief 获取数据库连接
     * @return std::shared_ptr<sqlite3> 数据库连接智能指针
     */
    std::shared_ptr<sqlite3> get_connection();

    /**
     * @brief 释放连接池资源
     */
    void shutdown();

    // 删除拷贝构造函数和赋值运算符
    SQLiteConnectionPool(const SQLiteConnectionPool&) = delete;
    SQLiteConnectionPool& operator=(const SQLiteConnectionPool&) = delete;

private:
    /**
     * @brief 私有构造函数
     */
    SQLiteConnectionPool() = default;

    /**
     * @brief 创建新的数据库连接
     * @return sqlite3* 新创建的数据库连接
     */
    sqlite3* create_connection();

    std::queue<sqlite3*> m_connections;       ///< 连接队列
    std::mutex m_mutex;                       ///< 互斥锁
    std::condition_variable m_condition;      ///< 条件变量
    std::string m_dbPath;                     ///< 数据库文件路径
    int m_poolSize;                           ///< 连接池大小
    bool m_initialized = false;               ///< 初始化标志
};

#endif // SQLITE_CONNECTION_POOL_H
