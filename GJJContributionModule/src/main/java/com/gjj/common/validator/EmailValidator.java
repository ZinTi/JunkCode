/** 验证邮箱地址是否合法，主要参考 RFC 3696 标准，而非 RFC 5322 标准。
 一、代码说明：
 1\ 总体验证流程：
 检查基本格式（长度、@符号数量和位置）
 分离本地部分和域名部分分别验证

 2\ 本地部分验证：
 支持带引号和不带引号两种格式
 引号内允许特殊字符和转义字符
 严格校验非引号格式的特殊字符和点号位置

 3\ 域名部分验证：
 完整实现RFC域名规范
 校验标签长度、有效字符和首尾字符
 禁止全数字的顶级域名

 4\ 特殊处理：
 转义字符处理（引号内的特殊字符）
 精确的RFC长度限制（本地部分64字符，域名255字符）

 二、注意事项：
 未处理国际化域名（IDN）的Punycode转换
 未处理IP地址格式的域名（如user@[IPv6:2001:db8::1]）
 严格遵循RFC规范，实际使用时可能需要根据业务需求调整
 该实现平衡了RFC规范要求和实际开发实用性，能够满足大多数标准邮箱地址的验证需求。
 */

package com.gjj.common.validator;

public class EmailValidator {

    public static boolean isValid(String email) {
        // Check if email is null or exceeds maximum length
        if (email == null || email.isEmpty() || email.length() > 320) {
            return false;
        }

        // Check for exactly one '@' and proper positioning
        int atIndex = email.lastIndexOf('@');
        if (atIndex <= 0 || atIndex == email.length() - 1 || email.indexOf('@') != atIndex) {
            return false;
        }

        String localPart = email.substring(0, atIndex);
        String domainPart = email.substring(atIndex + 1);

        return isValidLocalPart(localPart) && isValidDomainPart(domainPart);
    }

    private static boolean isValidLocalPart(String localPart) {
        // Check local part length
        if (localPart.length() < 1 || localPart.length() > 64) {
            return false;
        }

        // Handle quoted and unquoted local parts differently
        if (localPart.startsWith("\"") && localPart.endsWith("\"")) {
            // Quoted local part
            String content = localPart.substring(1, localPart.length() - 1);
            return content.matches("^(?:[^\"\\\\]|\\\\.)*$");
        } else {
            // Unquoted local part
            return localPart.matches("^[A-Za-z0-9!#$%&'*+/=?^_`{|}~-]+(\\.[A-Za-z0-9!#$%&'*+/=?^_`{|}~-]+)*$");
        }
    }

    private static boolean isValidDomainPart(String domainPart) {
        // Check domain part length
        if (domainPart.length() < 1 || domainPart.length() > 255) {
            return false;
        }

        // Split domain into labels
        String[] labels = domainPart.split("\\.");
        if (labels.length < 1) {
            return false;
        }

        // Validate each label
        for (String label : labels) {
            if (!isValidDomainLabel(label)) {
                return false;
            }
        }

        // Check if top-level domain is all numeric
        String topLevelDomain = labels[labels.length - 1];
        return !topLevelDomain.matches("^\\d+$");
    }

    private static boolean isValidDomainLabel(String label) {
        // Check label length
        if (label.length() < 1 || label.length() > 63) {
            return false;
        }

        // Check first and last character
        if (!Character.isLetterOrDigit(label.charAt(0))) {
            return false;
        }
        if (!Character.isLetterOrDigit(label.charAt(label.length() - 1))) {
            return false;
        }

        // Check valid characters
        return label.matches("^[A-Za-z0-9-]+$");
    }

    public static void main(String[] args) {
        // Test cases
        System.out.println(isValid("user.name@example.com"));        // true
        System.out.println(isValid("\"user.name\"@example.com"));    // true
        System.out.println(isValid("\"user\\\"name\"@example.com")); // true
        System.out.println(isValid(".username@example.com"));        // false
        System.out.println(isValid("user..name@example.com"));       // false
        System.out.println(isValid("user@[IPv6:2001:db8::1]"));     // false (special cases not handled)
        System.out.println(isValid("user@123.example"));             // true
        System.out.println(isValid("user@123.45"));                  // false (numeric TLD)
        System.out.println(isValid("user@-example.com"));            // false (invalid label start)
    }
}
