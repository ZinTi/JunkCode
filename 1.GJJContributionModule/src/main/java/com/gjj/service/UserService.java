package com.gjj.service;

import com.gjj.entity.User;

import java.util.List;
import java.util.UUID;

public interface UserService {


    /**
     * 根据 username 查询 userId
     * @param username 目标用户 username
     * @return userId 或 null
     */
    UUID getUserIdByUsername(final String username);

    /**
     * 通过 userId 获取单个用户信息
     * @param userId userId
     * @return User对象
     */
    Result.GenericResult<User> getUserById(final UUID userId);

    /**
     * 通过 username 获取单个用户信息
     * @param username username
     * @return User对象
     */
    Result.GenericResult<User> getUserByUsername(final String username);

    /**
     * 通过 用户昵称 查询用户信息
     * @param page 页码数
     * @param pageSize 每页数量
     * @param nickname 昵称
     * @return User 对象
     */
    Result.GenericResult<List<User>> getUserByNickname(int page, int pageSize, final String nickname);

    /**
     * 通过 Email 获取单个用户信息
     * @param email 邮箱
     * @return User 对象
     */
    Result.GenericResult<User> getUserByEmail(final String email);

    /**
     * 通过手机号获取单个用户信息
     * @param telephone 手机号
     * @return User 对象
     */
    Result.GenericResult<User> getUserByTelephone(final String telephone);

    /**
     * 获取所有用户信息
     * @param page 页数
     * @param pageSize 每页记录数
     * @return 用户列表
     */
    Result.GenericResult<List<User>> getAllUsers(int page, int pageSize);

    /**
     * 创建用户
     * @param username 用户名
     * @param nickname 昵称
     * @param password 密码
     * @param email 邮箱
     * @param telephone 手机号
     * @param gender 性别
     * @param userType 用户类别
     * @return 若新增成功则返回 user_id ，否则返回 null
     */
    Result.GenericResult<String> addUser(
        final String username,
        final String nickname,
        final String password,
        final String email,
        final String telephone,
        final String gender,
        final String userType);

    /**
     * 管理员通过 userId 删除用户
     * @param userId 目标用户 user_id
     * @return 成功与否
     */
    Result.CommonResult removeUserByUserId(final UUID userId);

    /**
     * 管理员通过 username 删除用户
     * @param username 目标用户 username
     * @return 成功与否
     */
    Result.CommonResult removeUserByUsername(final String username);

    /**
     * 管理员根据 userId 更新用户信息（不包括 password、salt、avatar、createdAt）
     * @param userId 目标用户 userId
     * @param username 用户名
     * @param nickname 昵称
     * @param email 电子邮箱
     * @param telephone 电话号码
     * @param gender 性别
     * @param userType 用户类别
     * @return 成功与否 + message
     */
    Result.CommonResult updateUserBaseInfoById(
            final UUID userId,
            final String username,
            final String nickname,
            final String email,
            final String telephone,
            final String gender,
            final String userType);

    /**
     * 管理员根据 username 更新用户信息（不包括 password、salt、avatar、createdAt）
     * @param username 目标用户 username
     * @param newUsername 新用户名
     * @param nickname 昵称
     * @param email 电子邮箱
     * @param telephone 电话号码
     * @param gender 性别
     * @param userType 用户类别
     * @return 成功与否 + message
     */
    Result.CommonResult updateUserBaseInfoByUsername(
            final String username,
            final String newUsername,
            final String nickname,
            final String email,
            final String telephone,
            final String gender,
            final String userType);

    /**
     * 更新用户头像
     * @param userId 目标用户 userId
     * @param avatarPath 用户头像路径
     * @return 成功与否 + message
     */
    Result.CommonResult updateAvatarPath(final UUID userId, final String avatarPath);

    /**
     * 更改用户密码（以及盐值）
     * @param userId userId
     * @param oldPassword 旧密码
     * @param newPassword 新密码
     * @return 成功与否 + message
     */
    Result.CommonResult updatePasswordAndSalt(final UUID userId, final String oldPassword, final String newPassword);

    /**
     * 重置用户密码
     * @param userId userId
     * @param TemporaryToken 临时 token
     * @param newPassword 新密码
     * @return 成功与否 + message
     */
    Result.CommonResult ResetPasswordAndSalt(UUID userId, String TemporaryToken, String newPassword);
}
