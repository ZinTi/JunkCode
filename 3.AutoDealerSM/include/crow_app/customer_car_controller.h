#ifndef CUSTOMER_CAR_CONTROLLER_H
#define CUSTOMER_CAR_CONTROLLER_H

#include <crow.h>
#include "models/customer_info.h"
#include "repositories/customer_car_info_dao.h"

class CustomerCarController {
private:
    CustomerCarInfoDAO& customer_car_dao_;

public:
    explicit CustomerCarController(CustomerCarInfoDAO& dao);

    // RESTful API 处理函数
    crow::response create_customer_car(const crow::request& req);
    crow::response get_customer_car_by_id(const crow::request& req, const std::string& id);
    crow::response get_all_customer_cars(const crow::request& req);
    crow::response update_customer_car(const crow::request& req, const std::string& id);
    crow::response delete_customer_car(const crow::request& req, const std::string& id);

    // 获取配置好路由的 Blueprint
    crow::Blueprint get_blueprint();

};

#endif
