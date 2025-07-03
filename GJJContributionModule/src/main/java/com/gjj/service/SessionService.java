package com.gjj.service;

import com.gjj.entity.Session;

public interface SessionService {
    // 创建新会话（带默认超时时间）
    Session createSession(String userId, String username, String userType);

    // 检查会话是否有效
    boolean isValidSession(String sessionId);

    // 使会话无效（删除）
    void invalidateSession(String sessionId);

    // 根据ID查询会话
    Session getSession(String sessionId);

    // 更新会话数据（包括续期）
    void updateSession(Session session);
}