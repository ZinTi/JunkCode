package com.gjj.service;

import com.gjj.entity.Session;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;

import java.util.Date;
import java.util.Map;

public class SessionServiceImpl implements SessionService {
    static final int DEFAULT_TIMEOUT_SECONDS = 1800; // 30分钟
    static final String SESSION_PREFIX = "session:";

    final JedisPool jedisPool;

    public SessionServiceImpl() {
        // 配置连接池（生产环境应从配置文件读取）
        JedisPoolConfig poolConfig = new JedisPoolConfig();
        poolConfig.setMaxTotal(128);
        poolConfig.setMaxIdle(32);
        poolConfig.setMinIdle(4);

        this.jedisPool = new JedisPool(poolConfig, "localhost", 6379);
    }

    @Override
    public Session createSession(String userId, String username, String userType) {
        Session session = Session.createNew(userId, username, userType);
        try (Jedis jedis = jedisPool.getResource()) {
            String key = SESSION_PREFIX + session.getSessionId();

            // 使用Redis Hash存储会话数据
            jedis.hset(key, "userId", session.getUserId());
            jedis.hset(key, "username", session.getUsername());
            jedis.hset(key, "userType", session.getUserType());
            jedis.hset(key, "createdTime", String.valueOf(session.getCreatedTime().getTime()));
            jedis.hset(key, "lastAccessedTime", String.valueOf(session.getLastAccessedTime().getTime()));

            // 设置超时时间
            jedis.expire(key, DEFAULT_TIMEOUT_SECONDS);
        }
        return session;
    }

    @Override
    public boolean isValidSession(String sessionId) {
        try (Jedis jedis = jedisPool.getResource()) {
            String key = SESSION_PREFIX + sessionId;
            return jedis.exists(key);
        }
    }

    @Override
    public void invalidateSession(String sessionId) {
        try (Jedis jedis = jedisPool.getResource()) {
            jedis.del(SESSION_PREFIX + sessionId);
        }
    }

    @Override
    public Session getSession(String sessionId) {
        try (Jedis jedis = jedisPool.getResource()) {
            String key = SESSION_PREFIX + sessionId;
            if (!jedis.exists(key)) return null;

            // 获取所有字段
            Map<String, String> fields = jedis.hgetAll(key);

            // 更新最后访问时间并续期
            long currentTime = System.currentTimeMillis();
            jedis.hset(key, "lastAccessedTime", String.valueOf(currentTime));
            jedis.expire(key, DEFAULT_TIMEOUT_SECONDS);

            // 构建Session对象
            return new Session(
                    sessionId,
                    fields.get("userId"),
                    fields.get("username"),
                    fields.get("userType"),
                    new Date(Long.parseLong(fields.get("createdTime"))),
                    new Date(currentTime) // 使用新访问时间
            );
        }
    }

    @Override
    public void updateSession(Session session) {
        try (Jedis jedis = jedisPool.getResource()) {
            String key = SESSION_PREFIX + session.getSessionId();
            if (!jedis.exists(key)) return;

            // 更新可变字段
            session.setLastAccessedTime(new Date());
            jedis.hset(key, "userId", session.getUserId());
            jedis.hset(key, "username", session.getUsername());
            jedis.hset(key, "userType", session.getUserType());
            jedis.hset(key, "lastAccessedTime", String.valueOf(session.getLastAccessedTime().getTime()));

            // 更新后重置超时时间
            jedis.expire(key, DEFAULT_TIMEOUT_SECONDS);
        }
    }

    // 关闭连接池
    public void shutdown() {
        if (jedisPool != null && !jedisPool.isClosed()) {
            jedisPool.close();
        }
    }
}