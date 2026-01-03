#include "crow_app/parts_info_controller.h"
#include "utils/time_id_generator.h"

PartsInfoController::PartsInfoController(PartsInfoDAO& dao) : parts_dao_(dao){}

// RESTful API 处理函数
crow::response PartsInfoController::create_part(const crow::request& req){
    crow::json::wvalue res;
    try{
        PartsInfo parts;

        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }

        parts.set_id(json["id"].s());
        parts.set_model(json["model"].s());
        parts.set_name(json["name"].s());
        parts.set_number(json["number"].i());
        parts.set_classification(json["classification"].s());
        parts.set_price(json["price"].d());
        parts.set_remarks(json["remarks"].s());

        if(parts_dao_.insert(parts)){
            res["result"] = "零件信息创建成功";
            return crow::response(201, res);
        } else {
            res["result"] = "零件信息创建失败";
            return crow::response(500, res);
        }

    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response PartsInfoController::get_part_by_id(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        PartsInfo parts = parts_dao_.find_by_id(id);
        if(parts.get_id().empty()) {
            res["result"] = "未找到此零件信息";
            return crow::response(404, res);
        }

        res["id"] = parts.get_id();
        res["model"] = parts.get_model();
        res["name"] = parts.get_name();
        res["number"] = parts.get_number();
        res["classification"] = parts.get_classification();
        res["price"] = parts.get_price();
        res["remarks"] = parts.get_remarks();

        return crow::response(200, res);
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response PartsInfoController::get_all_parts(const crow::request& req){
    crow::json::wvalue res;
    try{
        std::vector<PartsInfo> parts_list = parts_dao_.find_all();

        int index = 0;
        for (const auto& parts : parts_list) {
            res[index]["id"] = parts.get_id();
            res[index]["model"] = parts.get_model();
            res[index]["name"] = parts.get_name();
            res[index]["number"] = parts.get_number();
            res[index]["classification"] = parts.get_classification();
            res[index]["price"] = parts.get_price();
            res[index]["remarks"] = parts.get_remarks();
            index++;
        }
        return crow::response(200, res);
    }catch(const std::exception& e){
        res["res"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response PartsInfoController::update_part(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }

        PartsInfo parts;
        parts.set_id(id);
        parts.set_model(json["model"].s());
        parts.set_name(json["name"].s());
        parts.set_number(json["number"].i());
        parts.set_classification(json["classification"].s());
        parts.set_price(json["price"].d());
        parts.set_remarks(json["remarks"].s());

        if(parts_dao_.update(parts)){
            res["result"] = "零件信息更新成功";
            return crow::response(200, res);
        } else {
            res["result"] = "零件信息更新失败";
            return crow::response(500, res);
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response PartsInfoController::delete_part(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        if(parts_dao_.remove(id)){
            res["result"] = "零件信息删除成功";
            return crow::response(200, res);
        } else {
            res["result"] = "零件信息删除失败";
            return crow::response(500, res);
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::Blueprint PartsInfoController::get_blueprint(){
    crow::Blueprint parts_bp("api/parts");

    // POST /api/parts/ - 创建零件信息
    CROW_BP_ROUTE(parts_bp, "").methods("POST"_method)
    ([this](const crow::request& req) {
        return this->create_part(req);
    });

    // GET /api/parts/<string> - 根据ID获取零件信息
    CROW_BP_ROUTE(parts_bp, "/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->get_part_by_id(req, id);
    });

    // GET /api/parts/ - 获取所有零件信息
    CROW_BP_ROUTE(parts_bp, "").methods("GET"_method)
    ([this](const crow::request& req) {
        return this->get_all_parts(req);
    });

    // PUT /api/parts/<string> - 更新零件信息
    CROW_BP_ROUTE(parts_bp, "/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->update_part(req, id);
    });

    // DELETE /api/parts/<string> - 删除零件信息
    CROW_BP_ROUTE(parts_bp, "/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->delete_part(req, id);
    });
    return parts_bp;
}
