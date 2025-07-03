package com.gjj.common.validator;

public class TelephoneValidator {
    public static boolean isValidChineseMainlandPhone(String phoneNumber) {
        // 清理空格和连字符（自动处理类似 135-0001-0002 或 135 0001 0002 的格式）
        String cleaned = phoneNumber.replaceAll("[\\s-]", "");
        // 验证是否为11位纯数字、以1开头（不含国际前缀如+86）
        return cleaned.matches("^1\\d{10}$");
    }

    public static void main(String[] args) {
        System.out.println(isValidChineseMainlandPhone("13500010002"));      // true
        System.out.println(isValidChineseMainlandPhone("135-0001-0002"));   // true（清理后有效）
        System.out.println(isValidChineseMainlandPhone("135 0001 0002"));   // true（清理后有效）
        System.out.println(isValidChineseMainlandPhone("1350001002"));      // false（长度不足）
        System.out.println(isValidChineseMainlandPhone("135o001.002"));     // false（含非法字符）
        System.out.println(isValidChineseMainlandPhone("+8613500010002"));  // false（包含+86）
        System.out.println(isValidChineseMainlandPhone("23500010002"));      // false
    }
}