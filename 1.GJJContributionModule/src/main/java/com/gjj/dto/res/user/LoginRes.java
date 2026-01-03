package com.gjj.dto.res.user;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class LoginRes {
    private String sessionId;

    private String userId;
    private String username;
    private String nickname;
    private String gender;
    private String avatarPath;
    private String userType;

    public LoginRes() {
    }

    public LoginRes(String sessionId, String userId, String username, String nickname, String gender, String avatarPath, String userType) {
        this.sessionId = sessionId;
        this.userId = userId;
        this.username = username;
        this.nickname = nickname;
        this.gender = gender;
        this.avatarPath = avatarPath;
        this.userType = userType;
    }
}
