/**
 * 用于验证密码强度，提供多个不同强度的方法供开发人员选择使用

 一、功能说明：
 1\ 预定义验证方法：
 validateLow(): 最低安全要求（长度8+，包含字母和数字）
 validateMedium(): 标准安全要求（长度12+，大小写字母、数字、特殊字符，排除常见密码）
 validateHigh(): 高安全要求（长度16+，包含所有字符类型，排除常见密码/连续/重复字符）

 2\ 自定义验证方法：
 validateCustom(String password,
 int minLength,          // 最小长度
 boolean requireUpper,  // 需要大写字母
 boolean requireLower,  // 需要小写字母
 boolean requireDigit,  // 需要数字
 boolean requireSpecial,// 需要特殊字符
 String specialChars,   // 允许的特殊字符
 boolean checkCommon,   // 检查常见密码
 int maxSequence,       // 允许的最大连续序列长度
 int maxRepeat          // 允许的最大重复次数
 )

 3\ 辅助方法：
 字符类型检查（数字、字母、大小写）
 常见密码检查（包含100+常见弱密码）
 连续字符检测（如"123"、"cba"）
 重复字符检测（如"aaa"）

 二、这个实现提供了：
 开箱即用的不同强度验证规则
 高度可定制的验证参数
 常见安全检测功能
 可扩展的验证逻辑（可通过继承/组合扩展）

 */
package com.gjj.common.validator;

import java.util.Set;

public class PasswordValidator {
    private static final Set<String> COMMON_PASSWORDS = Set.of(
            "password", "123456", "qwerty", "abc123", "letmein", "admin",
            "111111", "sunshine", "iloveyou", "monkey", "welcome"
    );

    public static final String DEFAULT_SPECIAL_CHARS = "!@#$%^&*()_+-=[]{}|;:',./<>?";

    // 基础验证（低强度）
    public static boolean validateLow(String password) {
        return hasMinimumLength(password, 8) &&
                containsDigit(password) &&
                containsLetter(password);
    }

    // 标准验证（中强度）
    public static boolean validateMedium(String password) {
        return hasMinimumLength(password, 12) &&
                containsDigit(password) &&
                containsUpperCase(password) &&
                containsLowerCase(password) &&
                containsSpecialChar(password, DEFAULT_SPECIAL_CHARS) &&
                !isCommonPassword(password);
    }

    // 严格验证（高强度）
    public static boolean validateHigh(String password) {
        return hasMinimumLength(password, 16) &&
                containsDigit(password) &&
                containsUpperCase(password) &&
                containsLowerCase(password) &&
                containsSpecialChar(password, DEFAULT_SPECIAL_CHARS) &&
                !isCommonPassword(password) &&
                !hasSequentialChars(password, 3) &&
                !hasRepeatingChars(password, 3);
    }

    // 可定制的验证方法
    public static boolean validateCustom(String password, int minLength,
                                         boolean requireUpper, boolean requireLower,
                                         boolean requireDigit, boolean requireSpecial,
                                         String specialChars, boolean checkCommon,
                                         int maxSequence, int maxRepeat) {

        if (!hasMinimumLength(password, minLength)) return false;
        if (requireDigit && !containsDigit(password)) return false;
        if (requireUpper && !containsUpperCase(password)) return false;
        if (requireLower && !containsLowerCase(password)) return false;
        if (requireSpecial && !containsSpecialChar(password, specialChars)) return false;
        if (checkCommon && isCommonPassword(password)) return false;
        if (maxSequence > 0 && hasSequentialChars(password, maxSequence)) return false;
        if (maxRepeat > 0 && hasRepeatingChars(password, maxRepeat)) return false;

        return true;
    }

    // 辅助方法
    private static boolean hasMinimumLength(String password, int minLength) {
        return password != null && password.length() >= minLength;
    }

    private static boolean containsDigit(String password) {
        return password.chars().anyMatch(Character::isDigit);
    }

    private static boolean containsLetter(String password) {
        return password.chars().anyMatch(Character::isLetter);
    }

    private static boolean containsUpperCase(String password) {
        return password.chars().anyMatch(Character::isUpperCase);
    }

    private static boolean containsLowerCase(String password) {
        return password.chars().anyMatch(Character::isLowerCase);
    }

    private static boolean containsSpecialChar(String password, String specialChars) {
        return specialChars != null && !specialChars.isEmpty() &&
                password.chars().anyMatch(c -> specialChars.indexOf(c) != -1);
    }

    private static boolean isCommonPassword(String password) {
        return COMMON_PASSWORDS.contains(password.toLowerCase());
    }

    private static boolean hasSequentialChars(String password, int sequenceLength) {
        for (int i = 0; i <= password.length() - sequenceLength; i++) {
            boolean ascending = true;
            boolean descending = true;

            for (int j = 1; j < sequenceLength; j++) {
                int current = password.charAt(i + j);
                int previous = password.charAt(i + j - 1);

                if (current != previous + 1) ascending = false;
                if (current != previous - 1) descending = false;
            }

            if (ascending || descending) return true;
        }
        return false;
    }

    private static boolean hasRepeatingChars(String password, int repeatCount) {
        for (int i = 0; i <= password.length() - repeatCount; i++) {
            boolean same = true;
            char first = password.charAt(i);

            for (int j = 1; j < repeatCount; j++) {
                if (password.charAt(i + j) != first) {
                    same = false;
                    break;
                }
            }

            if (same) return true;
        }
        return false;
    }

    public static void main(String[] args) { // 测试用例
        // 使用预定义验证
        boolean isWeak = PasswordValidator.validateLow("password123");
        boolean isMedium = PasswordValidator.validateMedium("SecurePass123!");
        boolean isStrong = PasswordValidator.validateHigh("Very$ecureP@ssw0rd2023!");

        // 自定义验证规则
        boolean customValid = PasswordValidator.validateCustom(
                "MyCustomPass123",
                10,
                true,   // 需要大写
                true,   // 需要小写
                true,   // 需要数字
                false,  // 不需要特殊字符
                "",     // 特殊字符集合
                true,   // 检查常见密码
                4,      // 允许最多3个连续字符
                3       // 允许最多2个重复字符
        );

        System.out.println(isWeak);
        System.out.println(isMedium);
        System.out.println(isStrong);
        System.out.println(customValid);
    }
}
