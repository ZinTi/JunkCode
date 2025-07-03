package com.gjj.service;

public class Result {
    /**
     * 通用结果封装类
     * @param result 成功与否
     * @param message 消息
     * @param data 泛型数据字段
     * @param <T> 泛型数据类型
     */
    public record GenericResult<T>(
            boolean result,
            String message,
            T data
    ) {
        // 成功结果的快捷创建方法
        public static <T> GenericResult<T> success(T data) {
            return new GenericResult<>(true, "Success", data);
        }

        // 失败结果的快捷创建方法
        public static <T> GenericResult<T> failure(String errorMessage) {
            return new GenericResult<>(false, errorMessage, null);
        }

        // 带自定义消息的成功结果
        public static <T> GenericResult<T> success(String message, T data) {
            return new GenericResult<>(true, message, data);
        }
    }

    /**
     * 普通结果封装类
     * @param result
     * @param message
     */
    public record CommonResult(boolean result, String message) {}


}
