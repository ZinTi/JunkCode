# 住房公积金管理系统-筹集子系统（未完成）

## 一、说明
受人所托免费帮人写的Java课程实训作业，因时间关系只写了个用户管理就没时间写了，很草率，但够用了，只要知道一般的应用程序开发和 web 应用原理就行了。
代码部分上传 github 存档。

- 后端使用 Java 17 + Spring + MyBatis + MySQL + Redis
- 前端使用 HTML + CSS + Javascript
- 使用 RESTFul API 交互

最初版本我用 Spring Boot 写的，结果听说好像他们老师说不让用 Spring Boot，原因是没有 教过 Spring Boot，要下学期才教，不准用，只能用 Spring，然后就改成 Spring MVC 重写了。
后来他们答辩时，听说因为前端用了HTML + CSS + JS ，所以被怀疑是代写的（虽然确实如此），才知道他们要用 JSP 写，属实给我整笑了。

业务逻辑的那两张表是按照他们给的项目指导书写的，没怎么改，实际上这两张表键设计得一塌糊涂，一点也不符合数据库的设计规范和要求，不知道是哪一届学长或哪个老师设计的，被这老师放在指导书中一代一代传下来。

## 二、依赖
| item                      | value                         |
|---------------------------|-------------------------------|
| `OS version`              | Windows 11 24H2               |
| `Architecture`            | amd64                         |
| `JDK`                     | java 17.0.12 2024-07-16 LTS   |
| `Database`                | MySQL  Ver 8.0.40             |
| `Session Store`           | Redis 8.0.2                   |
| `Build & dependency tool` | Apache Maven 3.9.10           |
| `IDE`                     | IntelliJ IDEA Ultimate        |
| `Default locale`          | zh_CN, platform encoding: GBK |
| `Project encoding`        | UTF-8                         |


## 三、制图与实训报告部分
UML那些图画得太草率，论文也是AI写得，就没有上传的必要了。