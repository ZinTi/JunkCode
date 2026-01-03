#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <crow.h>
#include "crow_app/middleware.h"
#include "dao/user_dao.h"
#include "model/v_user.h"

class UserController {
private:
    UserDao& user_dao_;

public:
    explicit UserController(UserDao& dao);

    // RESTful API 处理函数
    void create_user(const crow::request& req, crow::response& res);
    void get_user_by_id(const crow::request& req, crow::response& res, int id);
    void get_user_by_username(const crow::request& req, crow::response& res, const std::string& username);
    void get_all_users(const crow::request& req, crow::response& res);
    void update_user(const crow::request& req, crow::response& res, int id);
    void delete_user(const crow::request& req, crow::response& res, int id);

    // 配置路由
    void bp_route(crow::Blueprint& user_blueprint); 
};

#endif
