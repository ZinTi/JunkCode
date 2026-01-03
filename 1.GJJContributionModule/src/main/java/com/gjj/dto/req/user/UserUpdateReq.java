package com.gjj.dto.req.user;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class UserUpdateReq {
    private String username;
    private String nickname;
    private String email;
    private String telephone;
    private String gender;
    private String userType;

    public UserUpdateReq() {
    }

    public UserUpdateReq(String username, String nickname, String email, String telephone, String gender, String userType) {
        this.username = username;
        this.nickname = nickname;
        this.email = email;
        this.telephone = telephone;
        this.gender = gender;
        this.userType = userType;
    }
}
