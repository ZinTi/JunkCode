-- SQLite3
/*

SQLite 存储类：NULL INTEGER REAL TEXT BLOB 
SQLite 亲和类：TEXT NUMERIC INTEGER REAL NONE

使用亲和类创建表，不考虑 SQL 语句与其他标准 SQL 的兼容性。

只实现：首页、注册账号、登录、修改个人信息、强制重置密码、注销账号这几个基本功能及其页面/
*/

PRAGMA foreign_keys = ON;

CREATE TABLE gender
(
    id INTEGER PRIMARY KEY,
    sex TEXT        -- 性别描述
);

INSERT INTO gender VALUES
(0, '未知'),
(1, '男'),
(2, '女');

CREATE TABLE users
(
    id INTEGER PRIMARY KEY,
    username TEXT NOT NULL UNIQUE,
    nickname TEXT NOT NULL DEFAULT '新用户',
    salt    TEXT NOT NULL,
    passwd  TEXT NOT NULL,
    email TEXT NOT NULL UNIQUE,
    gender_id INTEGER NOT NULL,
    FOREIGN KEY (gender_id) REFERENCES gender(id)
);

-- 用户名：root
-- 密码：'Abc123.'
INSERT INTO users VALUES
(1, 'root', '超级管理员', '40c3b8e28e587f89', '34e47279d89702f8403ec5d0152c6aa7f2f16ce08fe91676dd1e1a4fd34a7d20', 'root@email.com', (SELECT id FROM gender WHERE sex = '未知'));

-- 创建视图
CREATE VIEW v_users AS
SELECT users.id, users.username, users.nickname,
users.email, gender.sex FROM users LEFT JOIN gender WHERE users.gender_id = gender.id;


