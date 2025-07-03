package com.gjj.entity;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;
import java.util.UUID;

@Setter
@Getter
public class User {
    private UUID id;            // user_id, uuid
    private String username;
    private String nickname;
    private String password;
    private String salt;
    private String email;
    private String telephone;
    private String gender;
    private String avatarPath;
    private String userType;
    private LocalDateTime createdTime;
    private LocalDateTime updatedTime;

    public User() {
    }

    public User(UUID id, String username, String nickname, String password, String salt, String email, String telephone, String gender, String avatarPath, String userType, LocalDateTime createdTime, LocalDateTime updatedTime) {
        this.id = id;
        this.username = username;
        this.nickname = nickname;
        this.password = password;
        this.salt = salt;
        this.email = email;
        this.telephone = telephone;
        this.gender = gender;
        this.avatarPath = avatarPath;
        this.userType = userType;
        this.createdTime = createdTime;
        this.updatedTime = updatedTime;
    }

}
