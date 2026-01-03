#ifndef BEAUTY_MAINTENANCE_PROJ_CONTROLLER_H
#define BEAUTY_MAINTENANCE_PROJ_CONTROLLER_H

#include <crow.h>
#include "models/beauty_maintenance_proj.h"
#include "repositories/beauty_maintenance_proj_dao.h"

class BeautyMaintenanceProjController {
private:
    BeautyMaintenanceProjDAO& beauty_maint_dao_;

public:
    explicit BeautyMaintenanceProjController(BeautyMaintenanceProjDAO& dao);

    // RESTful API 处理函数
    crow::response create_beauty_maint_proj(const crow::request& req);
    crow::response get_beauty_maint_proj_by_id(const crow::request& req, const std::string& id);
    crow::response get_all_beauty_maint_projs(const crow::request& req);
    crow::response update_beauty_maint_proj(const crow::request& req, const std::string& id);
    crow::response delete_beauty_maint_proj(const crow::request& req, const std::string& id);

    crow::Blueprint get_blueprint();

};


#endif // BEAUTY_MAINTENANCE_PROJ_H