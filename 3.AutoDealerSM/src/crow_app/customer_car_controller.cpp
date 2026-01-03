#include "crow_app/customer_car_controller.h"
#include "utils/time_id_generator.h"

CustomerCarController::CustomerCarController(CustomerCarInfoDAO& dao) : customer_car_dao_(dao){}

// RESTful API 处理函数
crow::response CustomerCarController::create_customer_car(const crow::request& req){
    crow::json::wvalue res;
    try{
        CustomerCarInfo car_info;

        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }

        TimeIdGenerator id_generator;
        std::string car_id = id_generator.generate_time_id();
        car_info.set_id(car_id);
        car_info.set_condition(json["condition"].s());
        car_info.set_color(json["color"].s());
        car_info.set_cust_name(json["cust_name"].s());
        car_info.set_model(json["model"].s());

        if(customer_car_dao_.insert(car_info)){
            res["result"] = "客户车辆信息创建成功";
            return crow::response(201, res);
        } else {
            res["result"] = "客户车辆信息创建失败";
            return crow::response(500, res);
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerCarController::get_customer_car_by_id(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        CustomerCarInfo car_info = customer_car_dao_.find_by_id(id);
        if(car_info.get_id().empty()) {
            res["result"] = "未找到此客户车辆信息";
            return crow::response(404, res);
        }

        crow::json::wvalue json_response;
        json_response["id"] = car_info.get_id();
        json_response["condition"] = car_info.get_condition();
        json_response["color"] = car_info.get_color();
        json_response["cust_name"] =  car_info.get_cust_name();
        json_response["model"] = car_info.get_model();

        return crow::response(200, json_response);
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerCarController::get_all_customer_cars(const crow::request& req){
    crow::json::wvalue res;
    try{
        std::vector<CustomerCarInfo> cars = customer_car_dao_.find_all();
        crow::json::wvalue result;
        
        int index = 0;
        for (const auto& car : cars) {
            result[index]["id"] = car.get_id();
            result[index]["condition"] = car.get_condition();
            result[index]["color"] = car.get_color();
            result[index]["cust_name"] =  car.get_cust_name();
            result[index]["model"] = car.get_model();
            index++;
        }
        return crow::response(200, result);
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerCarController::update_customer_car(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }

        CustomerCarInfo car_info;
        car_info.set_id(id);
        car_info.set_condition(json["condition"].s());
        car_info.set_color(json["color"].s());
        car_info.set_cust_name(json["cust_name"].s());
        car_info.set_model(json["model"].s());

        if(customer_car_dao_.update(car_info)){
            res["result"] = "客户车辆信息更新成功";
            return crow::response(200, res);
        } else {
            res["result"] = "客户车辆信息更新失败";
            return crow::response(404, res);
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response CustomerCarController::delete_customer_car(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        if(customer_car_dao_.remove(id)){
            res["result"] = "客户车辆信息删除成功";
            return crow::response(200, res);
        } else {
            res["result"] = "未找到此客户车辆信息，删除失败";
            return crow::response(404, res);
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

// 获取配置好路由的 Blueprint
crow::Blueprint CustomerCarController::get_blueprint(){
    crow::Blueprint car_bp("api/customer_car");

    // POST /api/customer_car/ - 创建新客户车辆信息
    CROW_BP_ROUTE(car_bp, "").methods("POST"_method)
    ([this](const crow::request& req){
        return this->create_customer_car(req);
    });

    // GET /api/customer_car/<id> - 根据ID获取客户车辆信息
    CROW_BP_ROUTE(car_bp, "/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id){
        return this->get_customer_car_by_id(req, id);
    });

    // GET /api/customer_car/ - 获取所有客户车辆信息
    CROW_BP_ROUTE(car_bp, "").methods("GET"_method)
    ([this](const crow::request& req){
        return this->get_all_customer_cars(req);
    });

    // PUT /api/customer_car/<id> - 更新客户车辆信息
    CROW_BP_ROUTE(car_bp, "/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id){
        return this->update_customer_car(req, id);
    });

    // DELETE /api/customer_car/<id> - 删除客户车辆信息
    CROW_BP_ROUTE(car_bp, "/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id){
        return this->delete_customer_car(req, id);
    });
    return car_bp;
}
