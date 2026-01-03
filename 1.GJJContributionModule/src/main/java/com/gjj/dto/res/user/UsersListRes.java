package com.gjj.dto.res.user;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;
import java.util.List;

@Setter
@Getter
public class UsersListRes {
    // 分页元数据
    private int currentPage;    // 当前页码
    private int pageSize;       // 每页数据量
    private long totalItems;    // 总数据量
    private List<UserItem> users; // 用户列表数据

    // 静态内部类：单个用户项
    @Setter
    @Getter
    public static class UserItem {
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

        public UserItem() {
        }

        public UserItem(String userId, String username, String nickname, String email, String telephone, String gender, String avatarPath, String userType, LocalDateTime createdAt, LocalDateTime updatedAt) {
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

}
