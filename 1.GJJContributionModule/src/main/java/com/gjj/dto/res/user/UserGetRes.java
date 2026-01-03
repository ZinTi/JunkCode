package com.gjj.dto.res.user;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;

@Setter
@Getter
public class UserGetRes {
    private String userId;
    private String username;
    private String nickname;
    private String email;
    private String telephone;
    private String gender;
    private String avatarPath;
    private String userType;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;

    public UserGetRes() {
    }

    public UserGetRes(String userId, String username, String nickname, String email, String telephone, String gender, String avatarPath, String userType, LocalDateTime createdAt, LocalDateTime updatedAt) {
        this.userId = userId;
        this.username = username;
        this.nickname = nickname;
        this.email = email;
        this.telephone = telephone;
        this.gender = gender;
        this.avatarPath = avatarPath;
        this.userType = userType;
        this.createdAt = createdAt;
        this.updatedAt = updatedAt;
    }
}
