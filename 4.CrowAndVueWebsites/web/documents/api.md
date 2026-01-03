# API 文档

## 一、静态页面  

### 1.1 主页  
（1）请求  
```http
GET / HTTP/1.1
Host: 127.0.0.1
```
（2）响应  
重定向到 `/s/index.html`，返回静态页面 `./static/index.html`；  

（3）功能  
网站主页，若未登录自动跳转 `/s/login.html`。

### 1.2 注册页  
（1）请求  
```http
GET /s/register.html HTTP/1.1
Host: 127.0.0.1
```
（2）响应  
返回静态页面 `./static/register.html`；

（3）功能
注册用户的页面，提供一个跳转登录页 `/s/login.html` 的按键。

### 1.3 登录页  
（1）请求  
```http
GET /s/login.html HTTP/1.1
Host: 127.0.0.1
```
（2）响应  
返回静态页面 `./static/login.html`；

（3）功能
登录用户的页面，提供一个跳转注册页 `/s/register.html` 的按键。

### 1.4 个人信息页面
（1）请求  
```http
GET /s/info.html HTTP/1.1
Host: 127.0.0.1
```

（2）响应  
返回静态页面 `./static/info.html`；

（3）功能
查看、编辑（更新）个人信息，退出登录，注销账号（删除个人账号）

## 二、后端 API 服务  
### 2.1 注册账号
（1）请求  
```http
POST /api/users HTTP/1.1
Host 127.0.0.1

{
    "username": "admin",
    "nickname": "张三",
    "password": "Abc123.",
    "email": "zs@email.com",
    "gender": "男"
}
```

（2）响应  
```json
{
    "result": "OK",
    "message": "成功",
    "id": 3
}
```
或
```json
{
    "result": "ERROR",
    "message": "失败"
}
```

### 2.2 根据 ID 查询账号
(1) 请求  
```http
GET /api/users/<id> HTTP/1.1
Host 127.0.0.1
session-id: abc123

```

(2) 响应  
```http
HTTP/1.1 201 OK

{
    "id": 3,
    "username": "admin",
    "nickname": "张三",
    "email": "zs@email.com",
    "gender": "男"
}
```

### 2.3 根据 Username 查询账号 / 查询所有账号（当username不存在时）
(1) 请求  
```http
GET /api/users/?username=<username> HTTP/1.1
Host 127.0.0.1
session-id: abc123

```

(2) 响应  
如果 `?username=<username>` 无效则代表查询所有账号
```http
HTTP/1.1 200 OK

{
    "id": 3,
    "username": "admin",
    "nickname": "张三",
    "email": "zs@email.com",
    "gender": "男"
}
```


### 2.4 修改个人账号
(1) 请求  
```http
PUT /api/users/<id> HTTP/1.1
Host 127.0.0.1
session-id: abc123

{
    "id": 3,
    "username": "admin",
    "nickname": "李四",
    "email": "ls@email.com",
    "gender": "女",
    "password": "Abc123.",
    "new_password": "Pwd123."
}

```

(2) 响应  
```json
HTTP/1.1 200 OK

{
    "result": "OK",
    "message": "成功"
}
```

### 2.5 删除个人账号
(1) 请求  
```http
DELETE /api/users/<id> HTTP/1.1
Host 127.0.0.1
session-id: abc123

```

(2) 响应  
```json
HTTP/1.1 200 OK

{
    "result": "OK",
    "message": "成功"
}

```


### 2.6 登录账号
(1) 请求  
```http
POST /api/auth HTTP/1.1
Host 127.0.0.1
session-id: abc123

{
    "username": "admin",
    "password": "Abc123."
}
```

(2) 响应  
```json
HTTP/1.1 200 OK

{
    "session_id": "session123"
}
```

### 2.7 登出账号
(1) 请求  
```http
DELETE /api/auth HTTP/1.1
Host 127.0.0.1
session-id: abc123

```

(2) 响应  
```http
HTTP/1.1 200 OK
```
