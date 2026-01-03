#include "main.h"
#include <string>
#include "crow_app/middleware.h"

#include "repositories/sqlite_connection_pool.h"
#include "repositories/admin_info_dao.h"
#include "repositories/customer_info_dao.h"
#include "repositories/customer_car_info_dao.h"
#include "repositories/beauty_maintenance_proj_dao.h"
#include "repositories/employee_info_dao.h"
#include "repositories/parts_info_dao.h"

#include "crow_app/admin_controller.h"
#include "crow_app/customer_controller.h"
#include "crow_app/customer_car_controller.h"
#include "crow_app/beauty_maintenance_proj_controller.h"
#include "crow_app/employee_info_controller.h"
#include "crow_app/parts_info_controller.h"
#include "utils/session_manager.h"

// 全局会话管理器
SessionManager global_session_manager;

int main(const int argc, char** argv){
    UNUSED(argc);
    UNUSED(argv);
    if(init()){
        std::cout << "Failed to initialize! " << "初始化失败！" << std::endl;
    }

    crow::App<LogMiddleware, AuthMiddleware> app;    // 创建带中间件的 Crow 应用

    //***************************************** BEGIN 静态文件服务 BEGIN ****************************************************** */

    // 静态文件服务（首页）
    CROW_ROUTE(app, "/").methods("GET"_method, "PATCH"_method)
    ([]() {
        crow::response res;
        res.redirect("/s/index.html");  // 重定向到静态文件目录下的首页
        return res;

        // auto page = crow::mustache::load_text("s/index.html");
        // return page;
    });

    //***************************************** END 静态文件服务 END ****************************************************** */

    //***************************************** BIGIN 应用接口服务 BEGIN ****************************************************** */

    // 实例化 DAO、控制器，并注册蓝图
    AdminInfoDAO admin_dao;
    CustomerInfoDAO customer_dao;
    CustomerCarInfoDAO customer_car_dao;
    BeautyMaintenanceProjDAO beauty_maint_dao;
    EmployeeInfoDAO employee_dao;
    PartsInfoDAO parts_dao;

    AdminController admin_ctrl(admin_dao);
    CustomerController customer_ctrl(customer_dao);
    CustomerCarController customer_car_ctrl(customer_car_dao);
    BeautyMaintenanceProjController beauty_maint_ctrl(beauty_maint_dao);
    EmployeeInfoController employee_ctrl(employee_dao);
    PartsInfoController parts_ctrl(parts_dao);

    crow::Blueprint admin_bp = admin_ctrl.get_blueprint();
    crow::Blueprint customer_bp = customer_ctrl.get_blueprint();
    crow::Blueprint customer_car_bp = customer_car_ctrl.get_blueprint();
    crow::Blueprint beauty_maint_bp = beauty_maint_ctrl.get_blueprint();
    crow::Blueprint employee_bp = employee_ctrl.get_blueprint();
    crow::Blueprint parts_bp = parts_ctrl.get_blueprint();
    

    app.register_blueprint(admin_bp);
    app.register_blueprint(customer_bp);
    app.register_blueprint(customer_car_bp);
    app.register_blueprint(beauty_maint_bp);
    app.register_blueprint(employee_bp);
    app.register_blueprint(parts_bp);

    // DELETE /api/auth/  - 用户登出
    crow::Blueprint auth_bp("api/auth");
    CROW_BP_ROUTE(auth_bp, "/").methods("DELETE"_method)
    ([](const crow::request& req) {
        const std::string session_id = req.get_header_value("session_id"); // 从请求头中获取session_id
        
        if (session_id.empty()) { // 如果session_id为空
            return crow::response(401, "Error: 缺失会话");
        }

        global_session_manager.remove(session_id);
        return crow::response(200);
    });


    //***************************************** END 应用接口服务 END ****************************************************** */

    // 确保放在所有正常路由之后
    CROW_CATCHALL_ROUTE(app)
    ([](){
        crow::response res;
        res.redirect("/s/not_found.html");  // 重定向到 NOT FOUND 页面
        return res;
    });
    
    app.bindaddr("0.0.0.0").port(80).multithreaded().run();

    destroy();
    return 0;
}

int init(void){
    ch_console_codepage(CP_UTF8);

    // 初始化连接池
    if (!SQLiteConnectionPool::get_instance().initialize("car_data.db")) {
        std::cerr << "Failed to initialize connection pool" << std::endl;
        return -1;
    }

    return 0;
}

void destroy(void) {
    // 清理资源
    SQLiteConnectionPool::get_instance().shutdown();
}

void ch_console_codepage(const unsigned int codepage_id){
    SetConsoleCP(codepage_id);
    SetConsoleOutputCP(codepage_id);
}
