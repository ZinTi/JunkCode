/**
 * 正则表达式判断 标准 UUID 格式
 */

package com.gjj.common.validator;

import java.util.regex.Pattern;

public class UUIDStringValidator {
    // 预编译正则表达式（不区分大小写）
    private static final Pattern UUID_PATTERN = Pattern.compile(
            "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$",
            Pattern.CASE_INSENSITIVE
    );

    private static final Pattern NO_HYPHEN_PATTERN = Pattern.compile(
            "^[0-9a-f]{32}$",
            Pattern.CASE_INSENSITIVE
    );

    /**
     * 验证字符串是否为合法UUID
     * @param input 待验证字符串
     * @return true: 合法UUID, false: 非法UUID
     */
    public static boolean isValidUUID(String input) {
        if (input == null) return false;

        int len = input.length();

        // 检查32位无连字符格式
        if (len == 32) {
            return NO_HYPHEN_PATTERN.matcher(input).matches();
        }
        // 检查36位标准格式
        else if (len == 36) {
            // 额外验证连字符位置（正则不验证位置）
            return hasCorrectHyphenPositions(input) &&
                    UUID_PATTERN.matcher(input).matches();
        }

        return false;
    }

    // 验证连字符位置（索引从0开始）
    private static boolean hasCorrectHyphenPositions(String uuid) {
        return uuid.charAt(8) == '-' &&
                uuid.charAt(13) == '-' &&
                uuid.charAt(18) == '-' &&
                uuid.charAt(23) == '-';
    }
}