/**
 * @file time_id_generator.h
 * @brief 时间ID生成器模块头文件
 * @details 提供基于当前时间生成唯一字符串ID的功能
 */

#ifndef TIME_ID_GENERATOR_H
#define TIME_ID_GENERATOR_H

#include <string>

/**
 * @class TimeIdGenerator
 * @brief 时间ID生成器类
 * @details 用于获取当前时间并生成基于时间的字符串ID
 */
class TimeIdGenerator {
public:
    /**
     * @brief 构造函数
     */
    TimeIdGenerator(void);
    
    /**
     * @brief 析构函数
     */
    ~TimeIdGenerator(void);
    
    /**
     * @brief 获取当前时间生成的ID
     * @return 基于当前时间的字符串ID，格式：YYYYMMDDHHMMSSXXXX（16位）
     * @note 示例：2025121021571296 表示2025年12月10日21时57分12秒96（百分秒）
     */
    std::string generate_time_id(void) const;
    
    /**
     * @brief 获取当前时间的格式化字符串
     * @param include_milliseconds 是否包含毫秒部分
     * @return 格式化后的时间字符串
     */
    std::string get_current_time_string(const bool include_milliseconds) const;
    
    /**
     * @brief 获取当前时间戳（秒级）
     * @return 当前时间戳（自1970年以来的秒数）
     */
    long get_current_timestamp(void) const;

private:
    /**
     * @brief 获取当前时间的各个组成部分
     * @param year 年（输出参数）
     * @param month 月（输出参数）
     * @param day 日（输出参数）
     * @param hour 时（输出参数）
     * @param minute 分（输出参数）
     * @param second 秒（输出参数）
     * @param millisecond 毫秒（输出参数）
     */
    void get_time_components(int& year, int& month, int& day, int& hour, int& minute, int& second,int& millisecond) const;
    
    /**
     * @brief 格式化数字为固定长度字符串
     * @param num 要格式化的数字
     * @param width 固定宽度
     * @return 格式化后的字符串
     */
    std::string format_number(const int num, const int width) const;
};

#endif // TIME_ID_GENERATOR_H
