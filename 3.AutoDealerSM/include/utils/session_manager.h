#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

/**
 * @brief 会话信息结构体
 */
struct SessionInfo {
    std::string user_id;
    std::string user_type;
    
    SessionInfo(const std::string& id, const std::string& type) 
        : user_id(id), user_type(type) {}
};

/**
 * @brief 会话管理类，用于内存中会话的增删查清操作
 */
class SessionManager {
public:
    /**
     * @brief 创建新会话
     * @param user_id 用户唯一标识
     * @param user_type 用户类型
     * @return 新创建的会话ID（session_id）
     */
    std::string create(const std::string& user_id, const std::string& user_type);

    /**
     * @brief 删除指定会话（若存在）
     * @param session_id 要删除的会话ID
     */
    void remove(const std::string& session_id);

    /**
     * @brief 查询会话有效性
     * @param session_id 要查询的会话ID
     * @return 若会话存在（存在即代表有效）返回对应用户ID，否则返回空字符串
     */
    std::string get(const std::string& session_id) const;

    /**
     * @brief 查询用户类型
     * @param session_id 要查询的会话ID
     * @return 若会话存在返回用户类型，否则返回空字符串
     */
    std::string get_user_type(const std::string& session_id) const;

    /**
     * @brief 查询完整的会话信息
     * @param session_id 要查询的会话ID
     * @return 若会话存在返回SessionInfo，否则返回nullptr
     */
    std::shared_ptr<SessionInfo> get_session_info(const std::string& session_id) const;

    /// @brief 清空所有会话数据
    void clear_all();

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, SessionInfo> sessions_; // session_id -> SessionInfo
};

#endif
