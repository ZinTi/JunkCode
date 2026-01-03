#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <sstream>
#include <fstream>
#include <print> // C++ 新标准库

// 宏常量定义
#define PROJECT_NAME            "AutoDealerSM"
#define PROJECT_VERSION         "0.0.1"

// 宏函数定义
#define UNUSED(x) (void)(x)

// 定义静态文件夹和端点
#define CROW_STATIC_DIRECTORY   "static/"
#define CROW_STATIC_ENDPOINT    "/s/<path>"

#define USER_TYPE_ADMIN      "ADMIN"
#define USER_TYPE_CUSTOMER   "CUSTOMER"
#define USER_TYPE_EMPLOYEE   "EMPLOYEE"

/**
 * @brief 初始化程序
 * @return 0 成功，其他失败
 */
int init(void);

/**
 * @brief 退出程序
 */
void destroy(void);

/**
 * @brief 设置命令行编码
 */
void ch_console_codepage(unsigned int codepage_id);

void test(void);


#endif //MAIN_H
