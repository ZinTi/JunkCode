package com.gjj.service;

import com.gjj.entity.Session;
import redis.clients.jedis.Jedis;

import static com.gjj.service.SessionServiceImpl.DEFAULT_TIMEOUT_SECONDS;
import static com.gjj.service.SessionServiceImpl.SESSION_PREFIX;

public class SessionUtils {
    // 验证会话有效性并返回最新会话数据
    public static Session validateAndGet(SessionService service, String sessionId) {
        if (!service.isValidSession(sessionId)) {
            throw new SecurityException("Session is invalid or expired");
        }
        return service.getSession(sessionId);
    }

    // 刷新会话超时时间
    public static void refreshSession(SessionService service, String sessionId) {
        if (service.isValidSession(sessionId)) {
            try (Jedis jedis = ((SessionServiceImpl)service).jedisPool.getResource()) {
                jedis.expire(SESSION_PREFIX + sessionId, DEFAULT_TIMEOUT_SECONDS);
            }
        }
    }
}