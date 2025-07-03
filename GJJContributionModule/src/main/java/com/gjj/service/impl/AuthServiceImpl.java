package com.gjj.service.impl;

import com.gjj.common.utils.PasswordUtils;
import com.gjj.common.validator.UUIDStringValidator;
import com.gjj.dto.res.user.LoginRes;
import com.gjj.entity.Session;
import com.gjj.entity.User;
import com.gjj.mapper.UserMapper;
import com.gjj.service.AuthService;
import com.gjj.service.Result;
import com.gjj.service.SessionService;
import com.gjj.service.SessionServiceImpl;
import org.springframework.stereotype.Service;

import java.util.UUID;

@Service
public class AuthServiceImpl implements AuthService {
    private final UserMapper userMapper;

    public AuthServiceImpl(UserMapper userMapper) {
        this.userMapper = userMapper;
    }

    @Override
    public Result.CommonResult validateUserById(final UUID userId, final String password) {
        try{
            User user = userMapper.findById(userId);
            if(user == null){
                return new Result.CommonResult(false, "Password does not match"); // 用户不存在
            }

            if(PasswordUtils.verifyPassword(user.getPassword(), user.getSalt(), password)){
                return new Result.CommonResult(true, "Password verified"); // 密码匹配
            }
            return new Result.CommonResult(false, "Password does not match"); // 密码不匹配
        }catch(Exception e){
            return new Result.CommonResult(false, "用户查询失败：" + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult validateUserByUsername(final String username, final String password) {
        try{
            User user = userMapper.findByUsername(username);
            if(user == null){
                return new Result.CommonResult(false, "Password does not match"); // 用户不存在
            }

            if(PasswordUtils.verifyPassword(user.getPassword(), user.getSalt(), password)){
                return new Result.CommonResult(true, "Password verified"); // 密码匹配
            }
            return new Result.CommonResult(false, "Password does not match"); // 密码不匹配
        } catch (Exception e) {
            return new Result.CommonResult(false, "用户查询失败：" + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult validateUserByEmail(final String email, final String password) {
        try{
            User user = userMapper.findByEmail(email);
            if(user == null){
                return new Result.CommonResult(false, "Password does not match"); // 用户不存在
            }

            if(PasswordUtils.verifyPassword(user.getPassword(), user.getSalt(), password)){
                return new Result.CommonResult(true, "Password verified"); // 密码匹配
            }
            return new Result.CommonResult(false, "Password does not match"); // 密码不匹配
        } catch (Exception e) {
            return new Result.CommonResult(false, "用户查询失败：" + e.getMessage());
        }
    }


    @Override
    public Result.GenericResult<LoginRes> loginById(final UUID userId, final String password){
        // 1. 验证 user_id + password 是否正确
        try{
            User user = userMapper.findById(userId);
            if(user == null){
                return Result.GenericResult.failure("用户或密码错误"); // 用户不存在
            }

            if(!PasswordUtils.verifyPassword(user.getPassword(), user.getSalt(), password)){
                return Result.GenericResult.failure("用户或密码错误");   // 密码不匹配
            }

            // 2. 若正确，创建新会话
            SessionService service = new SessionServiceImpl();
            Session session = service.createSession(
                    userId.toString(),
                    user.getUsername(),
                    user.getUserType()
            );

            // 3. 返回 session_id + 用户信息
            return Result.GenericResult.success(
                    new LoginRes(
                            session.getSessionId(),
                            user.getId().toString(),
                            user.getUsername(),
                            user.getNickname(),
                            user.getGender(),
                            user.getAvatarPath(),
                            user.getUserType()
                    )
            );
        }catch(Exception e){
            return Result.GenericResult.failure(e.getMessage());
        }
    }

    @Override
    public Result.GenericResult<LoginRes> loginByUsername(final String username, final String password){
        // 1. 验证 user_id + password 是否正确
        try{
            User user = userMapper.findByUsername(username);
            if(user == null){
                return Result.GenericResult.failure("用户或密码错误"); // 用户不存在
            }

            if(!PasswordUtils.verifyPassword(user.getPassword(), user.getSalt(), password)){
                return Result.GenericResult.failure("用户或密码错误");   // 密码不匹配
            }

            // 2. 若正确，创建新会话
            SessionService service = new SessionServiceImpl();
            Session session = service.createSession(
                    user.getId().toString(),
                    user.getUsername(),
                    user.getUserType()
            );

            // 3. 返回 session_id + 用户信息
            return Result.GenericResult.success(
                    new LoginRes(
                            session.getSessionId(),
                            user.getId().toString(),
                            user.getUsername(),
                            user.getNickname(),
                            user.getGender(),
                            user.getAvatarPath(),
                            user.getUserType()
                    )
            );
        }catch(Exception e){
            return Result.GenericResult.failure(e.getMessage());
        }
    }

    @Override
    public Result.GenericResult<LoginRes> loginByEmail(final String email, final String password){
        // 1. 验证 user_id + password 是否正确
        try{
            User user = userMapper.findByEmail(email);
            if(user == null){
                return Result.GenericResult.failure("用户或密码错误"); // 用户不存在
            }

            if(!PasswordUtils.verifyPassword(user.getPassword(), user.getSalt(), password)){
                return Result.GenericResult.failure("用户或密码错误");   // 密码不匹配
            }

            // 2. 若正确，创建新会话
            SessionService service = new SessionServiceImpl();
            Session session = service.createSession(
                    user.getId().toString(),
                    user.getUsername(),
                    user.getUserType()
            );

            // 3. 返回 session_id + 用户信息
            return Result.GenericResult.success(
                    new LoginRes(
                            session.getSessionId(),
                            user.getId().toString(),
                            user.getUsername(),
                            user.getNickname(),
                            user.getGender(),
                            user.getAvatarPath(),
                            user.getUserType()
                    )
            );
        }catch(Exception e){
            return Result.GenericResult.failure(e.getMessage());
        }
    }

    @Override
    public void logout(final String sessionId){
        // 检查会话 ID 是否属于 UUID 格式
        if(UUIDStringValidator.isValidUUID(sessionId)){
            // 使会话失效
            SessionService service = new SessionServiceImpl();
            service.invalidateSession(sessionId);
        }
    }

}
