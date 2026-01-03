/**
 * @file time_id_generator.cpp
 * @brief 时间ID生成器模块实现文件
 */

#include "utils/time_id_generator.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

TimeIdGenerator::TimeIdGenerator(void) {
    // 构造函数，可在此初始化资源
}

TimeIdGenerator::~TimeIdGenerator(void) {
    // 析构函数，可在此释放资源
}

std::string TimeIdGenerator::generate_time_id(void) const {
    int year, month, day, hour, minute, second, millisecond;
    
    // 获取当前时间的各个组成部分
    get_time_components(year, month, day, hour, minute, second, millisecond);
    
    // 使用百分秒（2位）而不是毫秒（3位），以保持16位长度
    // 将毫秒转换为百分秒（0-99）
    int centisecond = millisecond / 10;
    
    // 组合成字符串ID：YYYYMMDDHHMMSSCC（16位）
    std::string time_id;
    time_id += format_number(year, 4);
    time_id += format_number(month, 2);
    time_id += format_number(day, 2);
    time_id += format_number(hour, 2);
    time_id += format_number(minute, 2);
    time_id += format_number(second, 2);
    time_id += format_number(centisecond, 2);
    
    return time_id;
}

std::string TimeIdGenerator::get_current_time_string(const bool include_milliseconds) const {
    int year, month, day, hour, minute, second, millisecond;
    
    // 获取当前时间的各个组成部分
    get_time_components(year, month, day, hour, minute, second, millisecond);
    
    // 格式化为易读的字符串
    std::ostringstream oss;
    oss << format_number(year, 4) << "-"
        << format_number(month, 2) << "-"
        << format_number(day, 2) << " "
        << format_number(hour, 2) << ":"
        << format_number(minute, 2) << ":"
        << format_number(second, 2);
    
    if (include_milliseconds) {
        oss << "." << format_number(millisecond, 3);
    }
    
    return oss.str();
}

long TimeIdGenerator::get_current_timestamp(void) const {
    // 获取当前时间戳（秒级）
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    return seconds.count();
}

void TimeIdGenerator::get_time_components(int& year, int& month, int& day, int& hour, int& minute, int& second, int& millisecond) const {
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();
    
    // 转换为time_t（秒级精度）
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    
    // 转换为本地时间结构体
    std::tm* local_time = std::localtime(&now_time_t);
    
    // 提取日期和时间部分
    year = local_time->tm_year + 1900;  // tm_year是从1900开始的年数
    month = local_time->tm_mon + 1;     // tm_mon是0-11，需要加1
    day = local_time->tm_mday;
    hour = local_time->tm_hour;
    minute = local_time->tm_min;
    second = local_time->tm_sec;
    
    // 获取毫秒部分
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    millisecond = millis % 1000;  // 取毫秒部分
}

std::string TimeIdGenerator::format_number(const int num, const int width) const {
    std::ostringstream oss;
    oss << std::setw(width) << std::setfill('0') << num;
    return oss.str();
}
