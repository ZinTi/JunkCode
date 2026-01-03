/**
 * 实现一个类，验证 中国大陆居民身份证号 IdCard 是否合法
 * 只实现第二代身份证 18 位长度（不考虑第一代身份证）
 */
package com.gjj.common.validator;

public class IdCardValidator {

    // 权重系数数组
    private static final int[] WEIGHT_COEFFICIENTS = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    // 校验码映射表
    private static final char[] CHECK_CODES = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};

    /**
     * 验证身份证号是否合法
     * @param idCard 身份证号码
     * @return 验证通过返回 true，否则返回 false
     */
    public static boolean isValid(String idCard) {
        // 1. 检查长度
        if (idCard == null || idCard.length() != 18) {
            return false;
        }

        // 2. 检查前17位是否为数字
        for (int i = 0; i < 17; i++) {
            if (!Character.isDigit(idCard.charAt(i))) {
                return false;
            }
        }

        // 3. 检查第18位是否合法（数字或X/x）
        char lastChar = idCard.charAt(17);
        if (!(Character.isDigit(lastChar) || lastChar == 'X' || lastChar == 'x')) {
            return false;
        }

        // 4. 检查出生日期是否有效
        String birthdayStr = idCard.substring(6, 14);
        if (!isValidBirthday(birthdayStr)) {
            return false;
        }

        // 5. 验证校验码
        return validateCheckCode(idCard);
    }

    /**
     * 验证出生日期是否有效（格式：yyyyMMdd）
     */
    private static boolean isValidBirthday(String birthday) {
        try {
            int year = Integer.parseInt(birthday.substring(0, 4));
            int month = Integer.parseInt(birthday.substring(4, 6));
            int day = Integer.parseInt(birthday.substring(6, 8));

            // 月份有效性 (1-12)
            if (month < 1 || month > 12) {
                return false;
            }

            // 日期有效性检查
            int maxDay;
            switch (month) {
                case 2: // 二月
                    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                        maxDay = 29; // 闰年
                    } else {
                        maxDay = 28; // 平年
                    }
                    break;
                case 4: case 6: case 9: case 11: // 小月 (30天)
                    maxDay = 30;
                    break;
                default: // 大月 (31天)
                    maxDay = 31;
            }
            return day > 0 && day <= maxDay;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    /**
     * 验证校验码是否正确
     */
    private static boolean validateCheckCode(String idCard) {
        int sum = 0;
        // 计算前17位的加权和
        for (int i = 0; i < 17; i++) {
            int digit = idCard.charAt(i) - '0';
            sum += digit * WEIGHT_COEFFICIENTS[i];
        }

        // 计算余数并获取正确的校验码
        int remainder = sum % 11;
        char correctCode = CHECK_CODES[remainder];

        // 获取输入的校验码（转换为大写）
        char inputCode = Character.toUpperCase(idCard.charAt(17));

        return correctCode == inputCode;
    }
}