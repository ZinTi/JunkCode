#include "utils/session_manager.h"
#include <random>
#include <algorithm>

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

std::string SessionManager::create(const std::string& user_id, const std::string& user_type) {
    std::lock_guard lock(mutex_);
    std::string session_id = generate_id();
    sessions_.emplace(session_id, SessionInfo(user_id, user_type));
    return session_id;
}

void SessionManager::remove(const std::string& session_id) {
    std::lock_guard lock(mutex_);
    sessions_.erase(session_id);
}

std::string SessionManager::get(const std::string& session_id) const {
    std::lock_guard lock(mutex_);
    if (auto it = sessions_.find(session_id); it != sessions_.end()) {
        return it->second.user_id;
    }
    return "";
}

std::string SessionManager::get_user_type(const std::string& session_id) const {
    std::lock_guard lock(mutex_);
    if (auto it = sessions_.find(session_id); it != sessions_.end()) {
        return it->second.user_type;
    }
    return "";
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
