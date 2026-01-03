#ifndef CUSTOMER_CONTROLLER_H
#define CUSTOMER_CONTROLLER_H

#include <crow.h>
#include "models/customer_info.h"
#include "repositories/customer_info_dao.h"

class CustomerController {
private:
    CustomerInfoDAO& customer_dao_; 

public:
    explicit CustomerController(CustomerInfoDAO& dao);

    // RESTful API 处理函数
    crow::response create_customer(const crow::request& req);
    crow::response get_customer_by_id(const crow::request& req, const std::string& id);
    crow::response get_all_customers(const crow::request& req);
    crow::response update_customer(const crow::request& req, const std::string& id);
    crow::response delete_customer(const crow::request& req, const std::string& id);
    crow::response login(const crow::request& req);

    // 注册路由到 Blueprint
    void register_routes(crow::Blueprint& blueprint);

    // 获取配置好路由的 Blueprint
    crow::Blueprint get_blueprint();
};

#endif
