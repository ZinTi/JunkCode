package com.gjj.service;

import com.gjj.dto.res.user.LoginRes;

import java.util.UUID;

public interface AuthService {

    /**
     * 通过 UUID 验证用户合法性
     * @param userId userId
     * @param password 账号密码
     * @return 验证结果（代码 0 代表验证成功，否则失败）
     */
    Result.CommonResult validateUserById(final UUID userId, final String password);

    /**
     * 通过 用户自定义的 username 验证用户合法性
     * @param username 用户登录别名
     * @param password 账号密码
     * @return 验证结果（代码 0 代表验证成功，否则失败）
     */
    Result.CommonResult validateUserByUsername(final String username, final String password);

    /**
     * 通过 Email 验证用户合法性
     * @param email 用户电子邮箱（唯一）
     * @param password 账号密码
     * @return 验证结果（代码 0 代表验证成功，否则失败）
     */
    Result.CommonResult validateUserByEmail(final String email, final String password);

    /**
     * 通过 uuid + password 登录
     * @param userId user_id
     * @param password 密码
     * @return session_id
     */
    Result.GenericResult<LoginRes> loginById(final UUID userId, final String password);

    /**
     * 通过 username + password 登录
     * @param username 用户名
     * @param password 密码
     * @return session_id
     */
    Result.GenericResult<LoginRes> loginByUsername(final String username, final String password);

    /**
     * 通过 email + password 登录
     * @param email 邮箱
     * @param password 密码
     * @return session_id
     */
    Result.GenericResult<LoginRes> loginByEmail(final String email, final String password);

    /**
     * 登出
     * @param sessionId 会话 ID
     */
    void logout(final String sessionId);
}
