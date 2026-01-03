package com.gjj.dto.req.user;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class UserAddReq {
    private String username;
    private String nickname;
    private String password;
    private String email;
    private String telephone;
    private String gender;
    private String userType;

    public UserAddReq() {
    }

    public UserAddReq(String username, String nickname, String password, String email, String telephone, String gender, String userType) {
        this.username = username;
        this.nickname = nickname;
        this.password = password;
        this.email = email;
        this.telephone = telephone;
        this.gender = gender;
        this.userType = userType;
    }
}
