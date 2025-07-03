/**
 * 在 Java 17 中，由于强封装性（Strong Encapsulation）的限制，MyBatis 无法通过反射访问 UUID 的私有构造函数。
 * 这是 JDK 16+ 引入的模块系统限制，导致直接映射到 UUID 类型会失败。
 * 因此创建当前这个专门的 UserId 包装类，避免了反射问题
 */
package com.gjj.entity;

import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Setter
@Getter
public class UserId {
    private UUID id;

    public UserId() {

    }
    public UserId(UUID id) {
        this.id = id;
    }
}
