package com.gjj.common.utils;

import java.nio.ByteBuffer;
import java.util.UUID;

/**
 * UUID转换工具类，提供UUID与String、byte[]之间的相互转换功能
 */
public final class UUIDConverter {

    /**
     * 禁止实例化
     */
    private UUIDConverter() {
        throw new AssertionError("Cannot instantiate utility class");
    }

    /**
     * 将字符串转换为UUID对象
     * @param str 支持以下格式：
     *            1. 标准UUID格式（8-4-4-4-12）
     *            2. 无连字符的32位十六进制字符串
     * @return 对应的UUID对象
     * @throws IllegalArgumentException 如果输入格式无效
     */
    public static UUID stringToUUID(String str) {
        String stripped = str.replace("-", "");
        if (stripped.length() != 32) {
            throw new IllegalArgumentException("Invalid UUID string: "
                    + "Expected 32 hex characters, got " + stripped.length());
        }

        if (!stripped.matches("^[0-9a-fA-F]{32}$")) {
            throw new IllegalArgumentException("Invalid UUID string: "
                    + "Contains non-hexadecimal characters");
        }

        // 重新构建标准格式的UUID字符串
        String formatted = stripped.replaceFirst(
                "(\\w{8})(\\w{4})(\\w{4})(\\w{4})(\\w{12})",
                "$1-$2-$3-$4-$5"
        );

        try {
            return UUID.fromString(formatted);
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("Malformed UUID string", e);
        }
    }

    /**
     * 将UUID转换为标准格式字符串（带连字符）
     */
    public static String uuidToString(UUID uuid) {
        return uuid.toString();
    }

    /**
     * 将UUID转换为无连字符的压缩字符串
     */
    public static String uuidToCompactString(UUID uuid) {
        return uuid.toString().replace("-", "");
    }

    /**
     * 将UUID转换为16字节的字节数组
     */
    public static byte[] uuidToBytes(UUID uuid) {
        ByteBuffer bb = ByteBuffer.wrap(new byte[16]);
        bb.putLong(uuid.getMostSignificantBits());
        bb.putLong(uuid.getLeastSignificantBits());
        return bb.array();
    }

    /**
     * 将16字节的字节数组转换为UUID
     * @throws IllegalArgumentException 如果字节数组长度不等于16
     */
    public static UUID bytesToUUID(byte[] bytes) {
        if (bytes.length != 16) {
            throw new IllegalArgumentException(
                    "Invalid byte array length: Expected 16, got " + bytes.length);
        }

        ByteBuffer bb = ByteBuffer.wrap(bytes);
        return new UUID(bb.getLong(), bb.getLong());
    }
}