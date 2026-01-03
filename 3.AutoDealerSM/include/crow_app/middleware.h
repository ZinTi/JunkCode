#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <crow.h>
#include "utils/session_manager.h"

extern SessionManager global_session_manager;

// 日志中间件（打印输出）
struct LogMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& ctx) {}
    
    void after_handle(crow::request& req, crow::response& res, context& ctx) {}
};

// 认证中间件
struct AuthMiddleware {
    // 白名单配置（路径 + 方法，不需要认证）
    struct WhitelistRule {
        std::string path;
        crow::HTTPMethod method;
        
        WhitelistRule(const std::string& p, crow::HTTPMethod m) : path(p), method(m) {}
    };

    std::vector<WhitelistRule> whitelist_rules = {
        // 格式：路径，HTTP方法
        {"/api/admin/login", crow::HTTPMethod::Post},
        {"/api/admin", crow::HTTPMethod::Post},        // 管理员-登录
        {"/", crow::HTTPMethod::Get},                  // 管理员-创建账号
        {"/api/customer/login", crow::HTTPMethod::Post},// 客户-登录
        {"/api/customer", crow::HTTPMethod::Post},      // 客户-创建账号
        {"/", crow::HTTPMethod::Patch},                // 首页
        {"/api/logout", crow::HTTPMethod::Delete}       // 登出接口
    };

    struct context {
        bool authenticated = false;
        std::string user_id;
    };
    
    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        // 获取请求路径和方法
        std::string path = req.url;
        crow::HTTPMethod method = req.method;
        
        // 检查是否在白名单中
        for (const auto& rule : whitelist_rules) {
            // 使用路径匹配（支持前缀匹配）
            bool path_matches = false;
            
            // 精确匹配或前缀匹配
            if (path == rule.path || path.find(rule.path) == 0) {
                path_matches = true;
            }
            
            // 检查方法和路径是否都匹配
            if (path_matches && rule.method == method) {
                CROW_LOG_INFO << "Whitelist rule matched: " << crow::method_name(rule.method) 
                            << " " << path << " - skipped authentication";
                return; // 白名单规则匹配，跳过认证
            }
        }

        // 从请求头中获取session_id
        auto session_id = req.get_header_value("session_id");
        
        // 如果session_id为空，返回错误
        if (session_id.empty()) {
            res.code = 401;
            res.write("Error: 缺失会话");
            res.end();
            return;
        }
        
        // 使用全局SessionManager验证session_id
        std::string user_id = global_session_manager.get(session_id);

        // 如果session_id无效，返回错误
        if (user_id.empty()) {
            res.code = 401;
            res.write("Error: 无效会话");
            res.end();
            return;
        }

        // session_id有效，设置认证状态和user_id
        ctx.authenticated = true;
        ctx.user_id = user_id;
        
        CROW_LOG_INFO << "User " << user_id << " authenticated successfully with session " << session_id;
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // 可选的后续处理，比如记录日志等
        if (ctx.authenticated) {
            CROW_LOG_INFO << "Request completed for user " << ctx.user_id;
        }
    }
};

#endif
