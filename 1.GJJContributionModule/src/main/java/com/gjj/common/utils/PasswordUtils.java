package com.gjj.common.utils;

import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;
import java.util.Base64;

public class PasswordUtils {
    // 迭代次数
    private static final int ITERATIONS = 10000;
    private static final int KEY_LENGTH = 256;

    public static String generateSalt() {
        SecureRandom random = new SecureRandom();
        byte[] salt = new byte[16];
        random.nextBytes(salt);
        return Base64.getEncoder().encodeToString(salt);
    }

    // 散列密码
    public static String digestPassword(String password, String salt) {
        try {
            PBEKeySpec spec = new PBEKeySpec(password.toCharArray(), salt.getBytes(), ITERATIONS, KEY_LENGTH);
            SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA256");
            byte[] hashedPassword = keyFactory.generateSecret(spec).getEncoded();
            return Base64.getEncoder().encodeToString(hashedPassword);
        } catch (NoSuchAlgorithmException | InvalidKeySpecException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * 比较密码是否一致
     * @param storedHashedPassword 存储的密码散列值
     * @param storedSalt 存储的盐值
     * @param password 要验证的密码
     * @return 如果密码一致则返回 true ，不一致返回 false
     */
    public static boolean verifyPassword(final String storedHashedPassword, final String storedSalt, final String password){
        if(storedHashedPassword == null || storedSalt == null || password == null){
            return false;
        }
        String hashedInputPassword = digestPassword(password, storedSalt);
        return storedHashedPassword.equals(hashedInputPassword);
    }
}
