/**
 * @file sqlite_connection_pool.cpp
 * @brief SQLite数据库连接池实现
 */

#include "repositories/sqlite_connection_pool.h"
#include <iostream>
#include <stdexcept>

extern "C" {
    #include <sqlite3.h>
}

SQLiteConnectionPool& SQLiteConnectionPool::get_instance() {
    static SQLiteConnectionPool instance;
    return instance;
}

bool SQLiteConnectionPool::initialize(const std::string& dbPath, int poolSize) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_initialized) {
        return true;
    }

    m_dbPath = dbPath;
    m_poolSize = poolSize;

    try {
        for (int i = 0; i < m_poolSize; ++i) {
            sqlite3* connection = create_connection();
            if (connection) {
                m_connections.push(connection);
            } else {
                throw std::runtime_error("Failed to create database connection");
            }
        }
        m_initialized = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Connection pool initialization failed: " << e.what() << std::endl;
        // 清理已创建的连接
        while (!m_connections.empty()) {
            sqlite3* conn = m_connections.front();
            sqlite3_close(conn);
            m_connections.pop();
        }
        return false;
    }
}

std::shared_ptr<sqlite3> SQLiteConnectionPool::get_connection() {
    std::unique_lock<std::mutex> lock(m_mutex);

    while (m_connections.empty()) {
        m_condition.wait(lock);
    }

    sqlite3* connection = m_connections.front();
    m_connections.pop();

    // 自定义删除器，将连接返回到池中
    auto deleter = [this](sqlite3* conn) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.push(conn);
        m_condition.notify_one();
    };

    return std::shared_ptr<sqlite3>(connection, deleter);
}

void SQLiteConnectionPool::shutdown() {
    std::lock_guard<std::mutex> lock(m_mutex);

    while (!m_connections.empty()) {
        sqlite3* connection = m_connections.front();
        sqlite3_close(connection);
        m_connections.pop();
    }

    m_initialized = false;
}

sqlite3* SQLiteConnectionPool::create_connection() {
    sqlite3* connection = nullptr;
    int result = sqlite3_open(m_dbPath.c_str(), &connection);

    if (result != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(connection) << std::endl;
        return nullptr;
    }

    // 设置超时时间
    sqlite3_busy_timeout(connection, 3000);
    return connection;
}
