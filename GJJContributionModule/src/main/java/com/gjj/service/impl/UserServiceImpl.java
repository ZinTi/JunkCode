package com.gjj.service.impl;

import com.gjj.common.utils.PasswordUtils;
import com.gjj.common.validator.EmailValidator;
import com.gjj.common.validator.TelephoneValidator;
import com.gjj.common.validator.DbEnumValidator;
import com.gjj.entity.User;
import com.gjj.entity.UserId;
import com.gjj.mapper.UserMapper;
import com.gjj.service.Result;
import com.gjj.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.List;
import java.util.UUID;

@Service
public class UserServiceImpl implements UserService {

    private final UserMapper userMapper;
    private static final int DEFAULT_PAGE_SIZE = 50;

    @Autowired
    public UserServiceImpl(UserMapper userMapper) {
        this.userMapper = userMapper;
    }

    @Override
    public UUID getUserIdByUsername(final String username) {
        UUID userId;
        try {
            UserId userIdWrapper = userMapper.findIdByUsername(username);
            if (userIdWrapper == null || userIdWrapper.getId() == null) {
                return null;
            }
            userId = userIdWrapper.getId();
        } catch (Exception e) {
            System.err.println("获取用户ID失败: " + e.getMessage());
            e.printStackTrace();
            return null;
        }
        return userId;
    }

    @Override
    public Result.GenericResult<User> getUserById(final UUID userId) {
        try {
            User user = userMapper.findById(userId);
            if (user == null) {
                return Result.GenericResult.failure("用户不存在");
            }
            return Result.GenericResult.success(user);
        } catch (Exception e) {
            return Result.GenericResult.failure("用户查询失败: " + e.getMessage());
        }
    }

    @Override
    public Result.GenericResult<User> getUserByUsername(final String username) {
        try {
            User user = userMapper.findByUsername(username);
            if (user == null) {
                return Result.GenericResult.failure("用户不存在");
            }
            return Result.GenericResult.success(user);
        } catch (Exception e) {
            return Result.GenericResult.failure("用户查询失败: " + e.getMessage());
        }
    }

    @Override
    public Result.GenericResult<List<User>> getUserByNickname(int page, int pageSize, final String nickname) {
        try {
            int offset = calculateOffset(page, pageSize);
            List<User> users = userMapper.findAllByNickname(offset, pageSize, nickname);
            return Result.GenericResult.success(users);
        } catch (Exception e) {
            return Result.GenericResult.failure("用户查询失败: " + e.getMessage());
        }
    }

    @Override
    public Result.GenericResult<User> getUserByEmail(final String email) {
        try {
            User user = userMapper.findByEmail(email);
            if (user == null) {
                return Result.GenericResult.failure("用户不存在");
            }
            return Result.GenericResult.success(user);
        } catch (Exception e) {
            return Result.GenericResult.failure("用户查询失败: " + e.getMessage());
        }
    }

    @Override
    public Result.GenericResult<User> getUserByTelephone(final String telephone) {
        try {
            User user = userMapper.findByTelephone(telephone);
            if (user == null) {
                return Result.GenericResult.failure("用户不存在");
            }
            return Result.GenericResult.success(user);
        } catch (Exception e) {
            return Result.GenericResult.failure("用户查询失败: " + e.getMessage());
        }
    }

    @Override
    public Result.GenericResult<List<User>> getAllUsers(int page, int pageSize) {
        try {
            int offset = calculateOffset(page, pageSize);
            List<User> users = userMapper.findAll(offset, pageSize);
            return Result.GenericResult.success(users);
        } catch (Exception e) {
            return Result.GenericResult.failure("用户查询失败: " + e.getMessage());
        }
    }

    @Override
    public Result.GenericResult<String> addUser(
            final String username,
            final String nickname,
            final String password,
            final String email,
            final String telephone,
            final String gender,
            final String userType) {

        // 验证邮箱和电话格式
        if (!EmailValidator.isValid(email)) {
            return Result.GenericResult.failure("邮箱格式不合法");
        }
        if (!TelephoneValidator.isValidChineseMainlandPhone(telephone)) {
            return Result.GenericResult.failure("手机号格式不合法");
        }

        // 检查用户名是否已存在
        if (userMapper.findByUsername(username) != null) {
            return Result.GenericResult.failure("用户名已存在");
        }

        User user = new User();
        user.setId(UUID.randomUUID());
        user.setUsername(username);
        user.setNickname(nickname);
        String salt = PasswordUtils.generateSalt();
        user.setPassword(PasswordUtils.digestPassword(password, salt));
        user.setSalt(salt);
        user.setEmail(email);
        user.setTelephone(telephone);
        user.setGender(gender);
        user.setUserType(userType);
        LocalDateTime now = LocalDateTime.now();
        user.setCreatedTime(now);
        user.setUpdatedTime(now);

        try {
            int row = userMapper.add(user);
            if (row == 1) {
                return Result.GenericResult.success(user.getId().toString());
            } else {
                return Result.GenericResult.failure("用户新增失败");
            }
        } catch (Exception e) {
            return Result.GenericResult.failure("用户新增失败: " + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult removeUserByUserId(final UUID userId) {
        try {
            int row = userMapper.remove(userId);
            if (row == 1) {
                return new Result.CommonResult(true, "删除成功");
            } else {
                return new Result.CommonResult(false, "用户不存在");
            }
        } catch (Exception e) {
            return new Result.CommonResult(false, "用户删除失败: " + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult removeUserByUsername(final String username) {
        try {
            int row = userMapper.removeByUsername(username);
            if (row == 1) {
                return new Result.CommonResult(true, "删除成功");
            } else {
                return new Result.CommonResult(false, "用户不存在");
            }
        } catch (Exception e) {
            return new Result.CommonResult(false, "用户删除失败: " + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult updateUserBaseInfoById(
            final UUID userId,
            final String username,
            final String nickname,
            final String email,
            final String telephone,
            final String gender,
            final String userType) {

        // 验证参数有效性
        if (!EmailValidator.isValid(email)) {
            return new Result.CommonResult(false, "邮箱格式不合法");
        }
        if (!TelephoneValidator.isValidChineseMainlandPhone(telephone)) {
            return new Result.CommonResult(false, "手机号格式不合法");
        }
        if(!DbEnumValidator.isValidUserType(userType)) {
            return new Result.CommonResult(false, "用户类型不合法");
        }
        if(!DbEnumValidator.isValidGender(gender)) {
            return new Result.CommonResult(false, "用户性别不合法");
        }

        // 检查用户名是否已被其他用户使用
        User existingUser = userMapper.findByUsername(username);
        if (existingUser != null && !existingUser.getId().equals(userId)) {
            return new Result.CommonResult(false, "用户名已被其他用户使用");
        }

        User user = new User();
        user.setId(userId);
        user.setUsername(username);
        user.setNickname(nickname);
        user.setEmail(email);
        user.setTelephone(telephone);
        user.setGender(gender);
        user.setUserType(userType);
        user.setUpdatedTime(LocalDateTime.now());

        try {
            int row = userMapper.updateBaseInfo(user);
            if (row == 1) {
                return new Result.CommonResult(true, "更新成功");
            } else {
                return new Result.CommonResult(false, "用户不存在");
            }
        } catch (Exception e) {
            return new Result.CommonResult(false, "更新失败: " + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult updateUserBaseInfoByUsername(
            final String username,
            final String newUsername,
            final String nickname,
            final String email,
            final String telephone,
            final String gender,
            final String userType) {

        // 验证参数有效性
        if (!EmailValidator.isValid(email)) {
            return new Result.CommonResult(false, "邮箱格式不合法");
        }
        if (!TelephoneValidator.isValidChineseMainlandPhone(telephone)) {
            return new Result.CommonResult(false, "手机号格式不合法");
        }
        if(!DbEnumValidator.isValidUserType(userType)) {
            return new Result.CommonResult(false, "用户类型不合法");
        }
        if(!DbEnumValidator.isValidGender(gender)) {
            return new Result.CommonResult(false, "用户性别不合法");
        }

        // 检查新用户名是否已被使用
        if (!username.equals(newUsername)) {
            User existingUser = userMapper.findByUsername(newUsername);
            if (existingUser != null) {
                return new Result.CommonResult(false, "新用户名已被其他用户使用");
            }
        }

        // 获取用户ID
        UUID userId;
        try {
            UserId userIdWrapper = userMapper.findIdByUsername(username);
            if (userIdWrapper == null || userIdWrapper.getId() == null) {
                return new Result.CommonResult(false, "用户不存在");
            }
            userId =userIdWrapper.getId();
        } catch(Exception e) {
            return new Result.CommonResult(false, "系统发生错误： " + e.getMessage());
        }

        User user = new User();
        user.setId(userId);
        user.setUsername(newUsername);
        user.setNickname(nickname);
        user.setEmail(email);
        user.setTelephone(telephone);
        user.setGender(gender);
        user.setUserType(userType);
        user.setUpdatedTime(LocalDateTime.now());

        try {
            int row = userMapper.updateBaseInfo(user);
            if (row == 1) {
                return new Result.CommonResult(true, "更新成功");
            } else {
                return new Result.CommonResult(false, "更新失败");
            }
        } catch (Exception e) {
            return new Result.CommonResult(false, "更新失败: " + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult updateAvatarPath(final UUID userId, final String avatarPath) {
        try {
            int row = userMapper.updateAvatarPath(userId, avatarPath, LocalDateTime.now());
            if (row == 1) {
                return new Result.CommonResult(true, "头像更新成功");
            } else {
                return new Result.CommonResult(false, "用户不存在");
            }
        } catch (Exception e) {
            return new Result.CommonResult(false, "头像更新失败: " + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult updatePasswordAndSalt(UUID userId, String oldPassword, String newPassword) {
        try {
            User targetUser = userMapper.findById(userId);
            if (targetUser == null) {
                return new Result.CommonResult(false, "用户不存在");
            }

            // 验证旧密码
            if (!PasswordUtils.verifyPassword(targetUser.getPassword(), targetUser.getSalt(), oldPassword)) {
                return new Result.CommonResult(false, "旧密码错误");
            }

            // 更新密码
            String newSalt = PasswordUtils.generateSalt();
            String hashedNewPassword = PasswordUtils.digestPassword(newPassword, newSalt);
            int row = userMapper.updateHashedPassword(userId, hashedNewPassword, newSalt, LocalDateTime.now());

            if (row == 1) {
                return new Result.CommonResult(true, "密码更新成功");
            } else {
                return new Result.CommonResult(false, "密码更新失败");
            }
        } catch (Exception e) {
            return new Result.CommonResult(false, "密码更新失败: " + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult ResetPasswordAndSalt(UUID userId, String TemporaryToken, String newPassword) {
        // TODO: 实现Token验证逻辑
        /*
        if (!validateToken(userId, TemporaryToken)) {
            return new Result.CommonResult(false, "Token验证失败");
        }
        */

        try {
            // 生成新密码
            String newSalt = PasswordUtils.generateSalt();
            String hashedNewPassword = PasswordUtils.digestPassword(newPassword, newSalt);

            // 更新密码
            int row = userMapper.updateHashedPassword(
                    userId,
                    hashedNewPassword,
                    newSalt,
                    LocalDateTime.now()
            );

            if (row == 1) {
                return new Result.CommonResult(true, "密码重置成功");
            } else {
                return new Result.CommonResult(false, "用户不存在");
            }
        } catch (Exception e) {
            return new Result.CommonResult(false, "密码重置失败: " + e.getMessage());
        }
    }

    // 辅助方法：计算分页偏移量
    private int calculateOffset(int page, int pageSize) {
        if (page < 1) page = 1;
        if (pageSize < 1) pageSize = DEFAULT_PAGE_SIZE;
        return (page - 1) * pageSize;
    }

    // TODO: 实现Token验证方法
    /*
    private boolean validateToken(UUID userId, String token) {
        // 实现实际的Token验证逻辑
        return true;
    }
    */
}