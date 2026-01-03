#include "main.h"
#include "dao/user_dao.h"
#include "dao/gender_dao.h"
#include "util/security_util.h"
#include "crow_app/user_controller.h"
#include "crow_app/auth_controller.h"

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * 1. 所有 /api/<path> 路由                     - 后端处理
 * 2. 所有 /s/<path> 路由                       - 静态资源服务
 * 3. 所有 /app/<path> 路由                     - 回退前端处理（避免前后端路由竞争）
 * 3. 特殊路由：/ , /app                        - 重定向至 /app/ 端点
 * 4. 其他未匹配路由                            - 404
 */

int main(const int argc, char** argv) {
    UNUSED(argc);
    UNUSED(argv);

    init();

    crow::App<AuthMW> app;    // 创建带中间件的 Crow 应用

    // 1. /api/<path> 路由
    //***************************************** BIGIN 应用接口服务 BEGIN ****************************************************** */
    UserDao user_dao;
    GenderDao gender_dao;

    UserController user_ctrl(user_dao);
    AuthController auth_ctrl(user_dao);

    crow::Blueprint user_bp("api/users");
    crow::Blueprint auth_bp("api/auth");
    user_ctrl.bp_route(user_bp);
    auth_ctrl.bp_route(auth_bp);

    app.register_blueprint(user_bp);
    app.register_blueprint(auth_bp);

    //***************************************** END 应用接口服务 END ****************************************************** */

    // 2. /app/<path> 路由
    //***************************************** BEGIN 静态文件服务 BEGIN ****************************************************** */
    // 2.1 默认处理，修改 CROW_STATIC_DIRECTORY 和 CROW_STATIC_ENDPOINT 修改配置
    // URL 为 /assets/<path> 由 Crow 自动处理

    // 2.2 默认图标、防爬虫文件
    CROW_ROUTE(app, "/favicon.ico").methods("GET"_method)
    ([](crow::response& res) {
        res.set_static_file_info("dist/favicon.ico");
        res.end();
    });

    CROW_ROUTE(app, "/robots.txt").methods("GET"_method)
    ([](crow::response& res) {
        res.set_static_file_info("dist/robots.txt");
        res.end();
    });

    // 2.3 前端路由回退 - 所有 /app/* 路径都返回前端入口文件
    CROW_ROUTE(app, "/app/")
    ([](crow::response& res) {      // 虽然写的是 "/app/"，但实际 "/app" 也会自动重定向到 "/app/"，反之则不能
        res.set_static_file_info("dist/index.html");
        res.end();
    });
    CROW_ROUTE(app, "/app/<string>").methods("GET"_method)
    ([](crow::response& res, const std::string& path) {
        UNUSED(path);
        res.set_static_file_info("dist/index.html");
        res.end();
    });
    CROW_ROUTE(app, "/")
    ([](crow::response& res) {
        res.moved_perm("/app/");    // 301 永久移动
        res.end();
    });

    //***************************************** END 静态文件服务 END ****************************************************** */
    
    // app.bindaddr("0.0.0.0").port(80).multithreaded().run();
    // 尝试启动服务器
    std::vector<int> ports = {80, 8080, 8000, 8081, 3000};
    
    if (!start_server(app, ports)) {
        std::cerr << "❌ 服务器启动失败！尝试了以下端口: ";
        for (size_t i = 0; i < ports.size(); ++i) {
            std::cerr << ports[i];
            if (i != ports.size() - 1) std::cerr << ", ";
        }
        std::cerr << std::endl;
        
        std::cerr << "\n可能的解决方案：" << std::endl;
        std::cerr << "1. 以管理员身份运行程序（Windows端口<1024需要管理员权限）" << std::endl;
        std::cerr << "2. 检查端口占用: netstat -ano | findstr :端口号" << std::endl;
        std::cerr << "3. 在代码中修改端口列表" << std::endl;
        std::cerr << "4. 关闭代理程序或抓包测试工具" << std::endl;
        std::cerr << "5. 关闭其他占用相关端口的服务进程" << std::endl;
        
        return 1;
    }
    
    return 0;
}

void init(void){
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
}

bool start_server(crow::App<AuthMW>& app, const std::vector<int>& ports) {
    for (int port : ports) {
        try {
            std::cout << "🚀 尝试在端口 " << port << " 启动服务器..." << std::endl;
            
            app.bindaddr("0.0.0.0").port(port).multithreaded().run();
            return true;  // 成功启动
        } 
        catch (const std::system_error& e) {
            if (e.code().value() == 10013 || e.code().value() == 13) {
                // 端口被占用或权限不足
                std::cerr << "⚠️ 端口 " << port << " 绑定失败: 端口被占用或权限不足" << std::endl;
            } else {
                std::cerr << "⚠️ 端口 " << port << " 绑定失败: " << e.what() << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "⚠️ 端口 " << port << " 绑定失败: " << e.what() << std::endl;
        }
    }
    return false;  // 所有端口都失败
}

// 读取文件内容的辅助函数
std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return "File not found: " + filename;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
