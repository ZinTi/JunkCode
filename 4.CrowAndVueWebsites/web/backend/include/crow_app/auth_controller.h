#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H

#include <crow.h>
#include "crow_app/middleware.h"
#include "dao/user_dao.h"
#include "model/v_user.h"

class AuthController {
private:
    UserDao& user_dao_;

public:
    explicit AuthController(UserDao& dao);

    // RESTful API 处理函数
    void login(const crow::request& req, crow::response& res);
    void logout(const crow::request& req, crow::response& res);

    /**
     * @brief 测试 session_id 是否过期
     * 如果未过期，返回 204 No Content
     * @param req 请求   session_id 必须在请求头中
     * @param res 响应
     */
    void test(const crow::request& req, crow::response& res);

    // 配置路由
    void bp_route(crow::Blueprint& auth_blueprint); 


};

#endif
