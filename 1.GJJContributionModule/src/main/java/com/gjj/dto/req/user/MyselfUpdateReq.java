package com.gjj.dto.req.user;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class MyselfUpdateReq {
    private String username;
    private String nickname;
    private String email;
    private String telephone;
    private String gender;

    public MyselfUpdateReq() {
    }

    public MyselfUpdateReq(String username, String nickname, String email, String telephone, String gender) {
        this.username = username;
        this.nickname = nickname;
        this.email = email;
        this.telephone = telephone;
        this.gender = gender;
    }
}
