#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <crow.h>
#include "util/session_manager.h"
#include "model/user_type_t.h"

// 宏常量
#define AUTH_BOOL           "authmw-auth"
#define AUTH_USER_ID        "authmw-auth_user_id"
#define AUTH_USER_TYPE      "authmw-auth_user_type"

// 认证中间件
// 1. 非API路径（不以/api/开头）全部跳过认证。
// 2. API路径（以/api/开头）则根据白名单规则进行认证。
struct AuthMW {
    // 白名单配置（路径 + 方法，不需要认证）
    struct WhitelistRule {
        std::string path;
        crow::HTTPMethod method;
        
        WhitelistRule(const std::string& p, crow::HTTPMethod m) : path(p), method(m) {}
    };

    std::vector<WhitelistRule> whitelist_rules = {
        // 免除 session 验证 白名单列表
        {"/api/users", crow::HTTPMethod::Post},                 // 用户注册
        {"/api/auth", crow::HTTPMethod::Post},                  // 用户登录
        {"/api/auth", crow::HTTPMethod::Delete}                 // 用户登出
    };

    struct context {
        bool authenticated = false;

        // 根据 session_id 获取的请求方的用户信息，而非请求方需要操作的目标用户的用户信息
        int auth_user_id;
        user_type_t auth_user_type;
    };
    
    void before_handle(crow::request& req, [[maybe_unused]] crow::response& res, context& ctx) {
        // 获取请求路径和方法
        std::string path = req.url;
        crow::HTTPMethod method = req.method;
        
        // 1. 非 API 路径一律跳过认证
        if (path.find("/api/") != 0) {
            // CROW_LOG_INFO << "Non-API path skipped authentication: " << path;
            return;
        }

        // 2. 对于 API 路径，检查是否在白名单中
        for (const auto& rule : whitelist_rules) {
            // 使用路径匹配（支持前缀匹配）
            bool path_matches = false;
            
            // 精确匹配或前缀匹配
            if (path == rule.path || path.find(rule.path) == 0) {
                path_matches = true;
            }
            
            // 检查方法和路径是否都匹配
            if (path_matches && rule.method == method) {
                CROW_LOG_INFO << "Whitelist rule matched: " << crow::method_name(rule.method) << " " << path << " - skipped authentication";
                return; // 白名单规则匹配，跳过认证
            }
        }

        // 从请求头中获取session_id
        auto session_id = req.get_header_value("session_id");
        
        // 如果session_id为空，返回错误
        if (session_id.empty()) {
            res.code = 401;
            res.write(R"({"message": "Missing Session"})");
            res.set_header("Content-Type", "application/json");
            res.end();
            return;
        }
        
        // 使用全局SessionManager验证session_id
        const auto session_info = global_session_manager.get_session_info(session_id);
        if(session_info){ // session_id有效，设置认证状态和user_id
            ctx.authenticated = true;
            ctx.auth_user_id = session_info->user_id;
            ctx.auth_user_type = session_info->user_type;

            // 后续 controller 中使用
            req.add_header(AUTH_BOOL, "true");
            req.add_header(AUTH_USER_ID, std::to_string(session_info->user_id));
            req.add_header(AUTH_USER_TYPE, [](user_type_t type) -> std::string {
                switch (type) {
                    case user_type_t::ADMIN: return USER_TYPE_ADMIN;
                    case user_type_t::USER: return USER_TYPE_USER;
                    case user_type_t::GUEST: return USER_TYPE_GUEST;
                    default: return USER_TYPE_INVALID;
                }
            }(session_info->user_type));

            CROW_LOG_INFO << "User " << ctx.auth_user_id << " authenticated successfully with session " << session_id;
        } else { // 如果session_id无效，返回错误
            res.code = 401;
            res.write(R"({"message": "Invalid Session"})");
            res.set_header("Content-Type", "application/json");
            res.end();
            return;
        }
    }

    void after_handle([[maybe_unused]] crow::request& req, [[maybe_unused]] crow::response& res, context& ctx) {
        // 可选的后续处理，比如记录日志等
        if (ctx.authenticated) {
            CROW_LOG_INFO << "Request completed for user " << ctx.auth_user_id;
        }
    }
};


#endif
