/**
 * 获取日期时间、日期和时间的Java工具类，使用Java 8引入的java.time API
 * 获取的时间是字符串类型，如果需要获取其他类型请使用其他库，不建议使用此实现

 一、功能说明：
 1\ 默认格式：
 日期时间：yyyy-MM-dd HH:mm:ss
 日期：yyyy-MM-dd
 时间：HH:mm:ss

 2\ 自定义格式：
 所有方法都提供重载版本，允许传入自定义的日期时间模式（遵循DateTimeFormatter模式语法）

 3\ 异常处理：
 如果传入非法格式会抛出DateTimeParseException

 4\ 扩展方法：
 添加了获取原始日期时间对象的方法（getDateTimeObject()等），方便进一步操作

 5\ 最佳实践：
 使用线程安全的java.time API
 工具类通过私有构造器防止实例化
 使用预定义的Formatter提升性能

 二、注意事项：
 格式化模式需符合DateTimeFormatter规范
 所有方法都基于系统默认时区，如需指定时区可添加如下方法：
 public static String getDateTimeWithZone(String pattern, ZoneId zone) {
 return LocalDateTime.now(zone).format(DateTimeFormatter.ofPattern(pattern));
 }

 **/
package com.gjj.common.utils;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;

public final class DateTimeUtils {

    // 私有构造器防止实例化
    private DateTimeUtils() {
        throw new AssertionError("Cannot instantiate utility class");
    }

    // 默认格式
    private static final String DEFAULT_DATE_PATTERN = "yyyy-MM-dd";
    private static final String DEFAULT_TIME_PATTERN = "HH:mm:ss";
    private static final String DEFAULT_DATETIME_PATTERN = "yyyy-MM-dd HH:mm:ss";

    // 预定义的Formatter（线程安全）
    private static final DateTimeFormatter DEFAULT_DATETIME_FORMATTER
            = DateTimeFormatter.ofPattern(DEFAULT_DATETIME_PATTERN);
    private static final DateTimeFormatter DEFAULT_DATE_FORMATTER
            = DateTimeFormatter.ofPattern(DEFAULT_DATE_PATTERN);
    private static final DateTimeFormatter DEFAULT_TIME_FORMATTER
            = DateTimeFormatter.ofPattern(DEFAULT_TIME_PATTERN);

    // 获取完整日期时间（默认格式）
    public static String getDateTime() {
        return LocalDateTime.now().format(DEFAULT_DATETIME_FORMATTER);
    }

    // 获取自定义格式的日期时间
    public static String getDateTime(String pattern) throws DateTimeParseException {
        return LocalDateTime.now().format(DateTimeFormatter.ofPattern(pattern));
    }

    // 获取日期（默认格式）
    public static String getDate() {
        return LocalDate.now().format(DEFAULT_DATE_FORMATTER);
    }

    // 获取自定义格式的日期
    public static String getDate(String pattern) throws DateTimeParseException {
        return LocalDate.now().format(DateTimeFormatter.ofPattern(pattern));
    }

    // 获取时间（默认格式）
    public static String getTime() {
        return LocalTime.now().format(DEFAULT_TIME_FORMATTER);
    }

    // 获取自定义格式的时间
    public static String getTime(String pattern) throws DateTimeParseException {
        return LocalTime.now().format(DateTimeFormatter.ofPattern(pattern));
    }

    // 可选：获取日期时间对象
    public static LocalDateTime getDateTimeObject() {
        return LocalDateTime.now();
    }

    // 可选：获取日期对象
    public static LocalDate getDateObject() {
        return LocalDate.now();
    }

    // 可选：获取时间对象
    public static LocalTime getTimeObject() {
        return LocalTime.now();
    }
}