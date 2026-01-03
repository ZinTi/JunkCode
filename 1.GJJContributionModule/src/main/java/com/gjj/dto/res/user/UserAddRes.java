package com.gjj.dto.res.user;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class UserAddRes {
    private String userId;
    private String username;
    private String nickname;

    public UserAddRes() {
    }

    public UserAddRes(String userId, String username, String nickname) {
        this.userId = userId;
        this.username = username;
        this.nickname = nickname;
    }
}
