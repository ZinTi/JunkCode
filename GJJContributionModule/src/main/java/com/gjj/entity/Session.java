package com.gjj.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;
import java.util.Date;
import java.util.UUID;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class Session implements Serializable {
    private String sessionId;       // 会话ID (UUID)
    private String userId;         // 用户ID
    private String username;       // 用户名
    private String userType;
    private Date createdTime;      // 创建时间
    private Date lastAccessedTime; // 上次访问时间

    // 生成新会话的工厂方法
    public static Session createNew(String userId, String username, String userType) {
        return new Session(
                UUID.randomUUID().toString(),
                userId,
                username,
                userType,
                new Date(),
                new Date()
        );
    }
}