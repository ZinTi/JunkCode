#include "crow_app/beauty_maintenance_proj_controller.h"
#include "utils/time_id_generator.h"


BeautyMaintenanceProjController::BeautyMaintenanceProjController(BeautyMaintenanceProjDAO& dao) : beauty_maint_dao_(dao){}

// RESTful API 处理函数
crow::response BeautyMaintenanceProjController::create_beauty_maint_proj(const crow::request& req){
    crow::json::wvalue res;
    try{
        BeautyMaintenanceProj proj;

        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }

        TimeIdGenerator id_generator;
        std::string proj_id = id_generator.generate_time_id();
        proj.set_id(proj_id);
        proj.set_group_id(json["group_id"].s());
        proj.set_proj_name(json["proj_name"].s());
        proj.set_price(json["price"].d());

        if(beauty_maint_dao_.insert(proj)){
            res["result"] = "美容保养项目信息创建成功";
            return crow::response(201, res);
        } else {
            res["result"] = "美容保养项目信息创建失败";
            return crow::response(500, res);
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response BeautyMaintenanceProjController::get_beauty_maint_proj_by_id(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        BeautyMaintenanceProj proj = beauty_maint_dao_.find_by_id(id);
        if(proj.get_id().empty()) {
            crow::json::wvalue res;
            res["result"] = "未找到此美容保养项目";
            return crow::response(404, res);
        }

        crow::json::wvalue json_response;
        json_response["id"] = proj.get_id();
        json_response["proj_name"] = proj.get_proj_name();
        json_response["price"] = proj.get_price();
        json_response["group_id"] = proj.get_group_id();

        return crow::response(200, json_response);
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response BeautyMaintenanceProjController::get_all_beauty_maint_projs(const crow::request& req){
    crow::json::wvalue res;
    try{
        std::vector<BeautyMaintenanceProj> projs = beauty_maint_dao_.find_all();
        crow::json::wvalue result;
        
        int index = 0;
        for (const auto& proj : projs) {
            result[index]["id"] = proj.get_id();
            result[index]["proj_name"] = proj.get_proj_name();
            result[index]["price"] = proj.get_price();
            result[index]["group_id"] = proj.get_group_id();
            index++;
        }

        return crow::response(200, result);
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response BeautyMaintenanceProjController::update_beauty_maint_proj(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        auto json = crow::json::load(req.body);
        if(!json) {
            res["result"] = "Invalid JSON"; return crow::response(400, res);
        }

        BeautyMaintenanceProj proj;
        proj.set_id(id);
        proj.set_group_id(json["group_id"].s());
        proj.set_proj_name(json["proj_name"].s());
        proj.set_price(json["price"].d());

        if(beauty_maint_dao_.update(proj)){
            res["result"] = "美容保养项目信息更新成功";
            return crow::response(200, res);
        } else {
            res["result"] = "未找到此美容保养项目，更新失败";
            return crow::response(500, res);
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::response BeautyMaintenanceProjController::delete_beauty_maint_proj(const crow::request& req, const std::string& id){
    crow::json::wvalue res;
    try{
        if(beauty_maint_dao_.remove(id)){
            res["result"] = "美容保养项目信息删除成功";
            return crow::response(200, res);
        } else {
            res["result"] = "未找到此美容保养项目，删除失败";
            return crow::response(404, res);
        }
    }catch(const std::exception& e){
        res["result"] = std::string("Error: ") + e.what(); return crow::response(500, res);
    }
}

crow::Blueprint BeautyMaintenanceProjController::get_blueprint(){
    crow::Blueprint beauty_bp("api/beauty_maintenance_proj");

    // POST /api/beauty_maintenance_proj/  创建美容保养项目
    CROW_BP_ROUTE(beauty_bp, "").methods("POST"_method)
    ([this](const crow::request& req) {
        return this->create_beauty_maint_proj(req);
    });

    // GET /api/beauty_maintenance_proj/<string>  根据ID获取美容保养项目信息
    CROW_BP_ROUTE(beauty_bp, "/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->get_beauty_maint_proj_by_id(req, id);
    });

    // GET /api/beauty_maintenance_proj/  获取所有美容保养项目信息
    CROW_BP_ROUTE(beauty_bp, "").methods("GET"_method)
    ([this](const crow::request& req) {
        return this->get_all_beauty_maint_projs(req);
    });

    // PUT /api/beauty_maintenance_proj/<string>  更新美容保养项目信息
    CROW_BP_ROUTE(beauty_bp, "/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->update_beauty_maint_proj(req, id);
    });

    // DELETE /api/beauty_maintenance_proj/<string>  删除美容保养项目信息
    CROW_BP_ROUTE(beauty_bp, "/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id) {
        return this->delete_beauty_maint_proj(req, id);
    });
    return beauty_bp;
}
