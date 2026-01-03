#ifndef MAIN_H
#define MAIN_H

// 定义静态文件夹和端点（覆盖 Crow 默认的 static 目录和端点），定义在 #include <crow.h> 前面
#ifndef CROW_STATIC_DIRECTORY
#define CROW_STATIC_DIRECTORY   "dist/assets/"         // 修改默认的 static/ 目录为 dist/assets 目录
#endif
#ifndef CROW_STATIC_ENDPOINT
#define CROW_STATIC_ENDPOINT    "/assets/<path>"       // 修改默认的 /static/<path> 端点为 /assets/<path> 端点
#endif

/* !!! 上面的宏有点问题，最好直接在编译选项中定义，因为预处理时宏定义的顺序对程序行为有影响 */
// 例如 CMakeLists.txt 脚本中
// target_compile_definitions(${PROJECT_NAME} PRIVATE
//     CROW_STATIC_DIRECTORY=\"dist/assets/\"
//     CROW_STATIC_ENDPOINT=\"/assets/<path>\"
// )

// 或者直接修改第三方库文件中宏，库文件路径：third_party/crow/include/settings.h
/*
// #ifndef CROW_STATIC_DIRECTORY
// #define CROW_STATIC_DIRECTORY "static/"
// #endif
// #ifndef CROW_STATIC_ENDPOINT
// #define CROW_STATIC_ENDPOINT "/static/<path>"
// #endif

#ifndef CROW_STATIC_DIRECTORY
#define CROW_STATIC_DIRECTORY "dist/assets/"
#endif
#ifndef CROW_STATIC_ENDPOINT
#define CROW_STATIC_ENDPOINT "/assets/<path>"
#endif

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include "crow_app/middleware.h"

// 宏函数定义
#define UNUSED(x) (void)(x)

/**
 * @brief 初始化程序
 */
void init(void);

/**
 * @brief 启动服务
 */
bool start_server(crow::App<AuthMW>& app, const std::vector<int>& ports);

/**
 * @brief 读取文件内容的函数
 * @param filename 文件名
 */
std::string read_file(const std::string& filename);

#endif
