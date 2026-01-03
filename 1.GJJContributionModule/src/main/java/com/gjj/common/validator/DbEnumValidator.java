/**
 * 数据库 ENUM 类型 合法性判断（不区分大小写）
 * 1、用户类型合法：ENUM('admin', 'teller', 'suspended')
 * 2、性别合法：ENUM('Male', 'Female', 'Unknown')
 * 3、账号合法：ENUM('normal','sealed','closed')
 */

package com.gjj.common.validator;

import java.util.Arrays;
import java.util.Set;
import java.util.stream.Collectors;

public class DbEnumValidator {

    // 用户类型枚举值
    private static final Set<String> VALID_USER_TYPES = createCaseInsensitiveSet(
            "admin", "teller", "suspended"
    );

    // 性别枚举值
    private static final Set<String> VALID_GENDERS = createCaseInsensitiveSet(
            "male", "female", "unknown"
    );

    // 账号状态枚举值
    private static final Set<String> VALID_ACCOUNT_STATUSES = createCaseInsensitiveSet(
            "normal", "sealed", "closed"
    );

    // 创建不区分大小写的枚举值集合
    private static Set<String> createCaseInsensitiveSet(String... values) {
        return Arrays.stream(values)
                .map(String::toLowerCase)
                .collect(Collectors.toSet());
    }

    /**
     * 验证用户类型是否合法
     * @param userType 待验证的用户类型
     * @return 合法返回 true，否则返回 false
     */
    public static boolean isValidUserType(String userType) {
        return userType != null && VALID_USER_TYPES.contains(userType.toLowerCase());
    }

    /**
     * 验证性别是否合法
     * @param gender 待验证的性别
     * @return 合法返回 true，否则返回 false
     */
    public static boolean isValidGender(String gender) {
        return gender != null && VALID_GENDERS.contains(gender.toLowerCase());
    }

    /**
     * 验证账号状态是否合法
     * @param status 待验证的账号状态
     * @return 合法返回 true，否则返回 false
     */
    public static boolean isValidAccountStatus(String status) {
        return status != null && VALID_ACCOUNT_STATUSES.contains(status.toLowerCase());
    }
}