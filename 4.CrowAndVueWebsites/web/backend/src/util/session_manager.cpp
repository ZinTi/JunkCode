#include "util/session_manager.h"
#include <random>
#include <algorithm>

SessionManager global_session_manager;  // 会话管理器全局实例

namespace {
    std::string generate_id() {
        static constexpr char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);
        
        std::string id;
        id.reserve(32);
        for (int i = 0; i < 32; ++i) {
            id += chars[dis(gen)];
        }
        return id;
    }
}

std::string SessionManager::create(const int user_id, const user_type_t user_type) {
    std::lock_guard lock(mutex_);
    std::string session_id = generate_id();
    sessions_.emplace(session_id, SessionInfo(user_id, user_type));
    return session_id;
}

void SessionManager::remove(const std::string& session_id) {
    std::lock_guard lock(mutex_);
    sessions_.erase(session_id);
}

int SessionManager::get_user_id(const std::string& session_id) const {
    std::lock_guard lock(mutex_);
    if (auto it = sessions_.find(session_id); it != sessions_.end()) {
        return it->second.user_id;
    }
    return -1;
}

user_type_t SessionManager::get_user_type(const std::string& session_id) const {
    std::lock_guard lock(mutex_);
    if (auto it = sessions_.find(session_id); it != sessions_.end()) {
        return it->second.user_type;
    }
    return user_type_t::INVALID;
}

std::shared_ptr<SessionInfo> SessionManager::get_session_info(const std::string& session_id) const {
    std::lock_guard lock(mutex_);
    if (auto it = sessions_.find(session_id); it != sessions_.end()) {
        return std::make_shared<SessionInfo>(it->second);
    }
    return nullptr;
}

void SessionManager::clear_all() {
    std::lock_guard lock(mutex_);
    sessions_.clear();
}

