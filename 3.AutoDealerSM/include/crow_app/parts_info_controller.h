#ifndef PARTS_INFO_CONTROLLER_H
#define PARTS_INFO_CONTROLLER_H

#include <crow.h>
#include "models/parts_info.h"
#include "repositories/parts_info_dao.h"

class PartsInfoController {
private:
    PartsInfoDAO& parts_dao_;
public:
    explicit PartsInfoController(PartsInfoDAO& dao);

    // RESTful API 处理函数
    crow::response create_part(const crow::request& req);
    crow::response get_part_by_id(const crow::request& req, const std::string& id);
    crow::response get_all_parts(const crow::request& req);
    crow::response update_part(const crow::request& req, const std::string& id);
    crow::response delete_part(const crow::request& req, const std::string& id);
    
    crow::Blueprint get_blueprint();
};

#endif // PARTS_INFO_CONTROLLER_H
