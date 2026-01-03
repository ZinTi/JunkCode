package com.gjj.dto.req.user;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class LoginReq {
    private String authType;
    private String credential; // userId(uuid) / username / email
    private String password;
    private String captcha; // 图灵测试验证码

    public LoginReq() {
    }

    public LoginReq(String authType, String credential, String password, String captcha) {
        this.authType = authType;
        this.credential = credential;
        this.password = password;
        this.captcha = captcha;
    }
}
