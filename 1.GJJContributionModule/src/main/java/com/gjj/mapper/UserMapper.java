package com.gjj.mapper;

import com.gjj.common.handler.UUIDTypeHandler;
import com.gjj.entity.User;
import com.gjj.entity.UserId;
import org.apache.ibatis.annotations.*;

import java.time.LocalDateTime;
import java.util.List;
import java.util.UUID;

@Mapper
public interface UserMapper {

    /**
     * 统计 users 总记录数
     * @return 总记录数
     */
    int countAll();

    /**
     * 根据 id 查询用户信息
     * @param id 用户 id
     * @return User
     */
    User findById(@Param("id") UUID id);

    /**
     * 根据 username 查询 userId
     * @param username 目标 username
     * @return 若成功返回 userId (因Java17反射限制MyBatis，故不使用UUID类型)，否则返回 null
     */
    UserId findIdByUsername(@Param("username") String username);

    /**
     * 根据 email 查询用户信息
     * @param email email
     * @return User
     */
    User findByEmail(@Param("email") String email);

    /**
     * 根据 telephone 查询用户信息
     * @param telephone 手机号
     * @return User
     */
    User findByTelephone(@Param("telephone") String telephone);

    /**
     * 根据 ID 列表批量查询用户
     * @param ids ID 列表
     * @return 用户列表
     */
    List<User> findByIds(@Param("ids") List<UUID> ids);

    /**
     * 根据 username 查询用户信息
     * @param username 用户名
     * @return User
     */
    User findByUsername(@Param("username") String username);

    /**
     * 分页查询多个用户
     * @param offset 偏移量
     * @param pageSize 每页显示的记录数量（行数）
     * @return 用户列表
     */
    List<User> findAll(
            @Param("offset") int offset,
            @Param("pageSize") int pageSize
    );

    /**
     * 根据 nickname 分页查询多个用户
     * @param offset 偏移量
     * @param pageSize 每页显示的记录数量（行数）
     * @param nickname 昵称
     * @return 用户列表
     */
    List<User> findAllByNickname(
            @Param("offset") int offset,
            @Param("pageSize") int pageSize,
            @Param("nickname") String nickname
    );

    /**
     * 新增用户信息
     * @param user 用户
     * @return 受影响行数
     */
    int add(User user);

    /**
     * 根据 userId 更新用户（排除 password、salt、avatar、created_at 字段）
     * @param user 用户
     * @return 返回受影响的行数
     */
    int updateBaseInfo(User user);

    /**
     * 根据 id 更新用户盐值和密码：盐值 salt、密码散列值 password、更新时间 updated_at
     * @param id 用户 id
     * @param newHashedPassword 用户密码新散列值
     * @param newSalt 盐值
     * @param updatedTime 更新时间
     * @return 返回受影响的行数
     */
    int updateHashedPassword(
            @Param("id") UUID id,
            @Param("newHashedPassword") String newHashedPassword,
            @Param("newSalt") String newSalt,
            @Param("updatedTime") LocalDateTime updatedTime
    );

    /**
     * 根据 userId 更新用户的头像路径
     * @param id userId
     * @param avatarPath 头像
     * @param updatedTime 更新时间
     * @return 受影响的行数
     */
    int updateAvatarPath(
            @Param("id") UUID id,
            @Param("avatarPath") String avatarPath,
            @Param("updatedTime") LocalDateTime updatedTime
    );

    /**
     * 根据 ID 删除用户
     * @param userId 用户主键
     * @return 返回受影响的行数
     */
    int remove(@Param("userId") UUID userId);

    /**
     * 根据 username 删除用户
     * @param username 用户主键
     * @return 返回受影响的行数
     */
    int removeByUsername(@Param("username") String username);
}