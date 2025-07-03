package com.gjj.controller.api;

import com.gjj.common.utils.UUIDConverter;
import com.gjj.common.validator.UUIDStringValidator;
import com.gjj.dto.req.user.LoginReq;
import com.gjj.dto.res.user.LoginRes;
import com.gjj.service.AuthService;
import com.gjj.service.Result;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.*;

import java.util.Map;
import java.util.UUID;

@RestController
@RequestMapping("/api/auth")
public class AuthController {
    private final AuthService authSvc;

    @Autowired
    public AuthController(AuthService authSvc) {
        this.authSvc = authSvc;
    }

    @PostMapping("/token")
    public ResponseEntity<?> login(
            @RequestBody LoginReq req
    ) {
        // 1. 检查请求数据有效性
        if (!StringUtils.hasText(req.getAuthType())
                || !StringUtils.hasText(req.getCredential())
                || !StringUtils.hasText(req.getPassword())
                || !StringUtils.hasText(req.getCaptcha())) {
            return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(Map.of(
                    "result", false,
                    "message", "Invalid request"
            ));
        }

        // 2. 验证 captcha （功能略）

        // 3. 登录
        Result.GenericResult<LoginRes> ret;
        switch (req.getAuthType()) {
            case "userId" -> {
                UUID userId;
                if(!UUIDStringValidator.isValidUUID(req.getCredential())) {
                    return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(Map.of(
                            "result", false,
                            "message", "用户或密码错误"
                    ));
                }
                userId = UUIDConverter.stringToUUID(req.getCredential());
                ret = authSvc.loginById(userId, req.getPassword());
            }
            case "username" -> ret = authSvc.loginByUsername(req.getCredential(), req.getPassword());
            case "email" -> ret = authSvc.loginByEmail(req.getCredential(), req.getPassword());
            default -> {
                return ResponseEntity.badRequest().body(Map.of(
                        "result", false,
                        "message", "Invalid auth type"
                ));
            }
        }

        if(!ret.result()){
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", ret.message()
                    ));
        }else{
            return ResponseEntity.status(HttpStatus.OK)
                .body(Map.of(
                    "result", true,
                    "message", "成功",
                    "data", ret.data()
                ));
        }

    }

    @PostMapping("/revoke")
    public void logout(
            @RequestHeader("sessionId") String sessionId
    ) {
        authSvc.logout(sessionId);
    }
}
