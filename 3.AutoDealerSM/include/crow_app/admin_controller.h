#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <crow.h>
#include "models/admin_info.h"
#include "repositories/admin_info_dao.h"

class AdminController {
private:
    AdminInfoDAO& admin_dao_;

public:
    explicit AdminController(AdminInfoDAO& dao);

    // RESTful API 处理函数
    crow::response create_admin(const crow::request& req);
    crow::response get_admin_by_id(const crow::request& req, const std::string& id);
    crow::response get_all_admin(const crow::request& req);
    crow::response update_admin(const crow::request& req, const std::string& id);
    crow::response delete_admin(const crow::request& req, const std::string& id);
    crow::response login(const crow::request& req);

    // 获取配置好路由的 Blueprint
    crow::Blueprint get_blueprint();
};

#endif
