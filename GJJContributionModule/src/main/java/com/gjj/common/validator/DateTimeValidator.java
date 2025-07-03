package com.gjj.common.validator;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.time.format.ResolverStyle;

public class DateTimeValidator {

    // 验证日期时间格式是否为 "yyyy-MM-dd HH:mm:ss"
    public static boolean isValidDateTime(String dateTimeStr) {
        if (dateTimeStr == null) {
            return false;
        }
        String trimmed = dateTimeStr.trim();
        try {
            DateTimeFormatter formatter = DateTimeFormatter.ofPattern("uuuu-MM-dd HH:mm:ss")
                    .withResolverStyle(ResolverStyle.STRICT);
            LocalDateTime.parse(trimmed, formatter);
            return true;
        } catch (DateTimeParseException e) {
            return false;
        }
    }

    // 验证日期格式是否为 "yyyy-MM-dd"
    public static boolean isValidDate(String dateStr) {
        if (dateStr == null) {
            return false;
        }
        String trimmed = dateStr.trim();
        try {
            DateTimeFormatter formatter = DateTimeFormatter.ofPattern("uuuu-MM-dd")
                    .withResolverStyle(ResolverStyle.STRICT);
            LocalDate.parse(trimmed, formatter);
            return true;
        } catch (DateTimeParseException e) {
            return false;
        }
    }

    // 验证时间格式是否为 "HH:mm:ss"
    public static boolean isValidTime(String timeStr) {
        if (timeStr == null) {
            return false;
        }
        String trimmed = timeStr.trim();
        try {
            DateTimeFormatter formatter = DateTimeFormatter.ofPattern("HH:mm:ss")
                    .withResolverStyle(ResolverStyle.STRICT);
            LocalTime.parse(trimmed, formatter);
            return true;
        } catch (DateTimeParseException e) {
            return false;
        }
    }
}

