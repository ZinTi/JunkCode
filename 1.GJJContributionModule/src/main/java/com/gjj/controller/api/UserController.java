package com.gjj.controller.api;

import com.gjj.common.utils.UUIDConverter;
import com.gjj.common.validator.UUIDStringValidator;
import com.gjj.dto.req.user.MyselfUpdateReq;
import com.gjj.dto.req.user.UserAddReq;
import com.gjj.dto.req.user.UserUpdateReq;
import com.gjj.dto.res.user.UserAddRes;
import com.gjj.dto.res.user.UserGetRes;
import com.gjj.dto.res.user.UsersListRes;
import com.gjj.entity.Session;
import com.gjj.entity.User;
import com.gjj.service.Result;
import com.gjj.service.SessionService;
import com.gjj.service.SessionServiceImpl;
import com.gjj.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

@RestController
@RequestMapping("/api/users")
public class UserController {
    private final UserService userSvc;

    @Autowired
    public UserController(UserService userSvc) {
        this.userSvc = userSvc;
    }

    /**
     * 管理员列出用户信息
     * @param sessionId 请求头 会话ID
     * @param page 请求参数 页码
     * @param pageSize 请求参数 每页记录数
     * @return JSON格式 元数据 + 用户列表数据
     */
    @GetMapping
    public ResponseEntity<?> getUsers(
            @RequestHeader("sessionId") final String sessionId,
            @RequestParam(value = "page", required = false) final int page,
            @RequestParam(value = "pageSize", required = false) final int pageSize
    ) {
        // 1. 检查会话有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 验证请求方的角色权限
        Session session = sessionSvc.getSession(sessionId);
        User requester = userSvc.getUserByUsername(session.getUsername()).data();
        if (!requester.getUserType().equals("admin")) {
            return ResponseEntity
                    .status(HttpStatus.FORBIDDEN)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. You lack ADMIN privileges."
                    ));
        }

        // 3. 列出用户
        List<User> users = userSvc.getAllUsers(page, pageSize).data();
        List<UsersListRes.UserItem> userItems = new ArrayList<>();
        for(User usr : users){
            UsersListRes.UserItem userItem = new UsersListRes.UserItem(
                    usr.getId().toString(),
                    usr.getUsername(),
                    usr.getNickname(),
                    usr.getEmail(),
                    usr.getTelephone(),
                    usr.getGender(),
                    usr.getAvatarPath(),
                    usr.getUserType(),
                    usr.getCreatedTime(),
                    usr.getUpdatedTime()
            );
            userItems.add(userItem);
        }

        UsersListRes resp = new UsersListRes();
        resp.setCurrentPage(page); // 当前页码
        resp.setPageSize(pageSize); // 每页数据量
        resp.setTotalItems(users.size()); // 总数据量
        resp.setUsers(userItems);

        return ResponseEntity.ok(Map.of(
                "result", true,
                "message", "成功",
                "data", resp
        ));
    }

    /**
     * 管理员根据 userId 或 username 查询某用户完整信息
     * @param sessionId 请求头 会话 ID
     * @param identifier userId 或 username
     * @return JSON 格式 用户完整信息
     */
    @GetMapping("/{identifier}")
    public ResponseEntity<?> getUserByIdOrUsername(
            @RequestHeader("sessionId") final String sessionId,
            @PathVariable("identifier") String identifier
            ) {
        // 1. 验证请求方即操作者的 session 是否有效
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                .status(HttpStatus.UNAUTHORIZED)
                .body(Map.of(
                    "result", false,
                    "message", "FAILED. 会话过期或无效，请重新登录."));
        }

        // 2. 验证请求方即操作者的角色权限（合法情况：请求方查询自己信息、请求方为管理员查询自己或他人信息）
        Session session = sessionSvc.getSession(sessionId);    // 根据 sessionId 获取请求方操作者的 session 信息
        boolean valid = session.getUserType().equals("admin");   // 如果是操作者是管理员则权限合法
        boolean isUuid;
        if(UUIDStringValidator.isValidUUID(identifier)){
            isUuid = true;
            if(session.getUserId().equals(identifier)) valid = true; // 查询目标与当前操作者一致（匹配 userId）
        } else {
            isUuid = false;
            if(session.getUsername().equals(identifier)) valid = true;  // 查询目标与当前操作者一致（匹配 username）
        }
        if(!valid){
            return ResponseEntity
                .status(HttpStatus.FORBIDDEN)
                .body(Map.of(
                    "result", false,
                    "message", "FAILED. You lack ADMIN privileges."));
        }

        // 3. 列出目标用户的完整信息
        User targetUser;
        if(isUuid){
            UUID targetUserId = UUIDConverter.stringToUUID(identifier);
            targetUser = userSvc.getUserById(targetUserId).data();
        } else {
            targetUser = userSvc.getUserByUsername(identifier).data();
        }
        if(targetUser == null){
            return ResponseEntity
                    .status(HttpStatus.NOT_FOUND)
                    .body(Map.of(
                            "result", false,
                            "message", "Resource not found. User with ID " + identifier + " does not exist"
                    ));
        }
        UserGetRes resp = new UserGetRes(
                targetUser.getId().toString(),
                targetUser.getUsername(),
                targetUser.getNickname(),
                targetUser.getEmail(),
                targetUser.getTelephone(),
                targetUser.getGender(),
                targetUser.getAvatarPath(),
                targetUser.getUserType(),
                targetUser.getCreatedTime(),
                targetUser.getUpdatedTime()
        );
        return ResponseEntity
                .status(HttpStatus.OK)
                .body(Map.of(
                        "result", true,
                        "message", "Success",
                        "data", resp
                ));
    }

    /**
     * 根据 userId 或 username 查看某用户公开信息
     * @param identifier userId 或 username
     * @return 用户完整信息
     */
    @GetMapping("/{identifier}/public")
    public ResponseEntity<?> getUserByIdOrUsernamePublic(
            @PathVariable("identifier") String identifier
    ) {
        // 1. 列出用户公开信息
        User targetUser;
        if(UUIDStringValidator.isValidUUID(identifier)){ // 判断 identifier 是 userId (UUID格式) 还是 username
            UUID targetUserId = UUIDConverter.stringToUUID(identifier);
            targetUser = userSvc.getUserById(targetUserId).data();
        }else{
            targetUser = userSvc.getUserByUsername(identifier).data();
        }
        if(targetUser == null){
            return ResponseEntity
                    .status(HttpStatus.NOT_FOUND)
                    .body(Map.of(
                            "result", false,
                            "message", "Resource not found. User with ID " + identifier + " does not exist"
                    ));
        }
        UserGetRes resp = new UserGetRes(
                targetUser.getId().toString(),
                targetUser.getUsername(),
                targetUser.getNickname(),
                null,
                null,
                targetUser.getGender(),
                targetUser.getAvatarPath(),
                targetUser.getUserType(),
                null,
                null
        );
        return ResponseEntity
                .status(HttpStatus.OK)
                .body(Map.of(
                        "result", true,
                        "message", "Success",
                        "data", resp
                ));
    }

    /**
     * 正常用户根据 sessionId 查询自己的个人用户完整信息
     * @param sessionId 请求头 会话 ID
     * @return JSON 格式 用户完整信息
     */
    @GetMapping("/me")
    public ResponseEntity<?> getMyselfBySessionId(
            @RequestHeader("sessionId") final String sessionId
    ) {
        // 1. 验证请求方即操作者的 session 是否有效
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."));
        }

        // 2. 将 sessionId 转换成 userId (查询个人信息，不必验证权限)
        final Session session = sessionSvc.getSession(sessionId);
        final String myUserId = session.getUserId();
        if(myUserId == null || myUserId.isEmpty()){
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话故障，服务器无法获取到用户数据."
                    ));
        }

        // 3. 列出我的完整用户信息
        User me = userSvc.getUserById(UUIDConverter.stringToUUID(myUserId)).data();
        if(me == null){
            return ResponseEntity
                    .status(HttpStatus.NOT_FOUND)
                    .body(Map.of(
                            "result", false,
                            "message", "Resource not found. User with session id " + sessionId + " does not exist"
                    ));
        }
        UserGetRes resp = new UserGetRes(
                me.getId().toString(),
                me.getUsername(),
                me.getNickname(),
                me.getEmail(),
                me.getTelephone(),
                me.getGender(),
                me.getAvatarPath(),
                me.getUserType(),
                me.getCreatedTime(),
                me.getUpdatedTime()
        );
        return ResponseEntity
                .status(HttpStatus.OK)
                .body(Map.of(
                        "result", true,
                        "message", "Success",
                        "data", resp
                ));
    }

    /**
     * 管理员创建用户（验证 sessionId ）
     * @param sessionId 请求头 会话 ID
     * @param req 包括 username 在内的新用户必要信息
     * @return 若成功返回 JSON 格式 数据 userId + username + nickname
     */
    @PostMapping
    public ResponseEntity<?> addUser(
            @RequestHeader("sessionId") final String sessionId,
            @RequestBody UserAddReq req
    ) {
        // 1. 首先检查请求方的 session 有效性和验证 username 不得为关键字 me
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }
        if(req.getUsername().equals("me")){
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. username 不得为 me，因为这是关键字."
                    ));
        }

        // 2. 验证请求方的角色权限
        Session session = sessionSvc.getSession(sessionId);
        User requester = userSvc.getUserByUsername(session.getUsername()).data();
        if(requester == null || !requester.getUserType().equals("admin") ){
            return ResponseEntity.status(HttpStatus.FORBIDDEN)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. You lack ADMIN privileges."
                    ));
        }

        // 3. 创建新用户
        Result.GenericResult<String> ret = userSvc.addUser(
                req.getUsername(),
                req.getNickname(),
                req.getPassword(),
                req.getEmail(),
                req.getTelephone(),
                req.getGender(),
                (StringUtils.hasText(req.getUserType()) ? req.getUserType() : "admin")
        );
        if(!ret.result()){
            return ResponseEntity
                    .status(HttpStatus.OK)
                    .body(Map.of(
                            "result", false,
                            "message", ret.message()
                    ));
        } else {
            return ResponseEntity
                    .status(HttpStatus.OK)
                    .body(Map.of(
                            "result", true,
                            "message", "Success",
                            "data", new UserAddRes(
                                    ret.data(),
                                    req.getUsername(),
                                    req.getNickname()
                            )
                    ));
        }
    }

    /**
     * 管理员创建用户（验证 secretCode ）
     * @param secretCode 内部注册码
     * @param req 包括 username 在内的新用户必要信息
     * @return 若成功返回 JSON 格式 数据 userId + username + nickname
     */
    @PostMapping("/internal")
    public ResponseEntity<?> addUserInternal(
            @RequestHeader("secretCode") final String secretCode,
            @RequestBody UserAddReq req
    ) {
        // 1. 验证 secretCode 和 username 关键字
        if(!secretCode.equals("admin666")){
            return ResponseEntity.status(HttpStatus.FORBIDDEN)
                    .body(Map.of(
                            "result", false,
                            "message", "Wrong secret code."
                    ));
        }
        if(req.getUsername().equals("me")){
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. username 不得为 me，因为这是关键字."
                    ));
        }

        // 2. 新增
        Result.GenericResult<String> ret = userSvc.addUser(
                req.getUsername(),
                req.getNickname(),
                req.getPassword(),
                req.getEmail(),
                req.getTelephone(),
                req.getGender(),
                (!StringUtils.hasText(req.getUserType()) ? "admin" : req.getUserType())
        );

        if(!ret.result()){
            return ResponseEntity
                    .status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", "注册失败" + ret.message()
                    ));
        }
        return ResponseEntity
                .status(HttpStatus.OK)
                .body(Map.of(
                        "result", true,
                        "message", "注册成功",
                        "data", new UserAddRes(
                                ret.data(),
                                req.getUsername(),
                                req.getNickname()
                        )
                ));
    }

    /**
     * 管理员更新用户用户基本信息（不含 password、salt、avatar、createdTime）
     * @param sessionId 请求头 会话 ID （操作者）
     * @param identifier 路径参数 更新目标的 userId 或 username
     * @param req 请求体 包括 username 在内的需要更新的信息
     * @return JSON 格式 结果 + 状态 + 其他信息
     */
    @PutMapping("/{identifier}")
    public ResponseEntity<?> updateUser(
            @RequestHeader("sessionId") final String sessionId,
            @PathVariable("identifier") final String identifier,
            @RequestBody UserUpdateReq req) {
        // 1. 验证 session 是否合法
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 验证请求方的角色权限（合法情况：请求方更改个人用户信息、或请求方为管理员）
        Session session = sessionSvc.getSession(sessionId);    // 根据 sessionId 获取请求方操作者的 session 信息
        boolean valid = session.getUserType().equals("admin");   // 如果是操作者是管理员则权限合法
        boolean isUuid;
        if(UUIDStringValidator.isValidUUID(identifier)){
            isUuid = true;
            if(session.getUserId().equals(identifier)) valid = true; // 查询目标与当前操作者一致（匹配 userId）
        } else {
            isUuid = false;
            if(session.getUsername().equals(identifier)) valid = true;  // 查询目标与当前操作者一致（匹配 username）
        }
        if(!valid){
            return ResponseEntity
                    .status(HttpStatus.FORBIDDEN)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. You lack ADMIN privileges."));
        }

        // 3. 更新目标用户的信息
        Result.CommonResult ret;
        try {
            if (isUuid) {
                ret = userSvc.updateUserBaseInfoById(
                        UUIDConverter.stringToUUID(identifier),
                        req.getUsername(),
                        req.getNickname(),
                        req.getEmail(),
                        req.getTelephone(),
                        req.getGender(),
                        req.getUserType()
                );
            } else {
                ret = userSvc.updateUserBaseInfoByUsername(
                        identifier,
                        req.getUsername(),
                        req.getNickname(),
                        req.getEmail(),
                        req.getTelephone(),
                        req.getGender(),
                        req.getUserType()
                );
            }
        } catch (Exception e) {
            return ResponseEntity
                    .status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", e.getMessage()
                    ));
        }

        if(!ret.result()){
            return ResponseEntity
                    .status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. " + ret.message()
                    ));
        }

        return ResponseEntity
                .status(HttpStatus.OK)
                .body(Map.of(
                        "result", true,
                        "message", "Success"
                ));
    }

    /**
     * 更新个人用户基本信息（不含 password、salt、avatar、createdTime）
     * @param sessionId 请求头 会话 ID （操作者）
     * @param req 请求体 包括 username 在内的需要更新的信息
     * @return JSON 格式 结果 + 状态 + 其他信息
     */
    @PutMapping
    public ResponseEntity<?> updateMyself(
            @RequestHeader("sessionId") final String sessionId,
            @RequestBody MyselfUpdateReq req) {
        // 1. 验证 session 是否合法
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 将 sessionId 转换成 userId (修改个人信息，不必验证权限)
        final Session session = sessionSvc.getSession(sessionId);
        final String userId = session.getUserId();
        if(userId == null || userId.isEmpty()){
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话故障，服务器无法获取到用户数据."
                    ));
        }

        // 3. 更新用户信息
        Result.CommonResult ret;
        try {
            ret = userSvc.updateUserBaseInfoById(
                    UUIDConverter.stringToUUID(userId),
                    req.getUsername(),
                    req.getNickname(),
                    req.getEmail(),
                    req.getTelephone(),
                    req.getGender(),
                    session.getUserType()   // 不能更改自己的 userType
            );
        } catch (Exception e) {
            return ResponseEntity
                    .status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", e.getMessage()
                    ));
        }

        if(!ret.result()){
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. " + ret.message()
                    ));
        }

        return ResponseEntity.status(HttpStatus.OK)
                .body(Map.of(
                        "result", true,
                        "message", "Success"
                ));
    }

    /**
     * 管理员根据 userId 或 username 删除用户信息
     * @param sessionId 请求头 会话 ID
     * @param identifier 删除目标用户 userId 或 username
     * @return JSON 格式 结果 + 信息 + 其他数据
     */
    @DeleteMapping("/{identifier}")
    public ResponseEntity<?> deleteUser(
            @RequestHeader("sessionId") final String sessionId,
            @PathVariable("identifier") String identifier
    ) {
        // 1. 检查会话有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                .status(HttpStatus.UNAUTHORIZED)
                .body(Map.of(
                        "result", false,
                        "message", "FAILED. 会话过期或无效，请重新登录."
                ));
        }

        // 2. 检查权限，验证请求方的角色权限，只有管理员有权删除用户
        Session session = sessionSvc.getSession(sessionId);    // 根据 sessionId 获取请求方操作者的 session 信息
        if(!session.getUserType().equals("admin")){ // 如果是操作者是管理员则权限合法
            return ResponseEntity
                .status(HttpStatus.FORBIDDEN)
                .body(Map.of(
                    "result", false,
                    "message", "FAILED. You lack ADMIN privileges."));
        }

        // 3. 执行删除用户的操作
        Result.CommonResult ret;
        if(UUIDStringValidator.isValidUUID(identifier)){
            ret = userSvc.removeUserByUserId(UUIDConverter.stringToUUID(identifier));
        } else {
            ret = userSvc.removeUserByUsername(identifier);
        }
        if(!ret.result()){
            return ResponseEntity
                .status(HttpStatus.INTERNAL_SERVER_ERROR)
                .body(Map.of(
                    "result", false,
                    "message", "FAILED. Deletion failed: " + ret.message()
                ));
        }else{
            return ResponseEntity
                .status(HttpStatus.OK)
                .body(Map.of(
                    "result", true,
                    "message", "Success"
                ));
        }
    }
}
