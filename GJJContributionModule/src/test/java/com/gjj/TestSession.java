package com.gjj;

import com.gjj.entity.Session;
import com.gjj.service.SessionService;
import com.gjj.service.SessionServiceImpl;

public class TestSession {
    public static void main(String[] args) {
        SessionService service = new SessionServiceImpl();

        // 创建新会话
        Session session = service.createSession("user123", "john_doe", "admin");
        System.out.println("Created session: " + session.getSessionId());

        // 查询会话
        Session retrieved = service.getSession(session.getSessionId());
        System.out.println("Username: " + retrieved.getUsername());

        // 更新会话
        retrieved.setUsername("new_username");
        service.updateSession(retrieved);

        // 验证会话
        if (service.isValidSession(session.getSessionId())) {
            System.out.println("Session is valid");
        }

        // 使会话失效
        service.invalidateSession(session.getSessionId());

        // 关闭连接池
        ((SessionServiceImpl) service).shutdown();
    }
}
