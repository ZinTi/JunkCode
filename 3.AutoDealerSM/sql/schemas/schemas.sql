/*
 * 汽车 4S 店服务管理系统数据库 car_data.db
 *
 * 数据库结构设计主要依据 “2-2025 软件工程项目开发I课程设计指导书.doc”，对部分表结构进行了修改调整。
 * 原有表结构设计存在以下问题：
 * 1. 安全性问题，例如密码明文存储；
 * 2. 数据长度不符合规范，例如 UTF-8 编码的中文字符长度为 3 字节，部分西北地区少数民族的名字较长，故扩充 VARCHAR 长度；
 * 3. 代码风格规范问题，例如字段名不符合规范；
 *
 */

-- 1. 管理员信息表
CREATE TABLE admin_info
(
    id   VARCHAR(20) PRIMARY KEY, --  管理员账号（主键，原account）
    name VARCHAR(50) NOT NULL,    --  管理员姓名（延长字符长度）
    salt TEXT NOT NULL,           --  管理员密码盐值（随机生成的字符串）
    pwd  TEXT    NOT NULL         --  管理员密码（SHA256 哈希散列后的值，非明文存储）
);

-- 2. 客户信息表
CREATE TABLE customer_info
(
    id            VARCHAR(20) PRIMARY KEY,                  --  客户编号（主键，原MemberID）
    name          VARCHAR(50)  NOT NULL,                    --  客户姓名
    salt          TEXT NULL,                                --  客户密码盐值（随机生成的字符串）
    pwd           TEXT NULL DEFAULT NULL,                   --  客户密码（SHA256 哈希散列后的值，非明文存储）
    gender        CHAR(1)      NOT NULL DEFAULT 'M',        --  性别（男、女、未知）
    email         VARCHAR(30)  NOT NULL DEFAULT '',         --  电子邮件
    phone         VARCHAR(20)  NOT NULL DEFAULT '',         --  电话
    address       VARCHAR(100) NOT NULL DEFAULT '',         --  地址
    zip_code      VARCHAR(10)  NOT NULL DEFAULT '',         --  邮政编码
    car_model     VARCHAR(20)  NOT NULL DEFAULT '',         --  车辆型号(原Car)
    car_condition VARCHAR(50)  NOT NULL DEFAULT '',         --  车辆情况（原CarQ）
    car_id        VARCHAR(20)  NOT NULL DEFAULT '',         --  车辆编号（原CarB）
    CONSTRAINT chk_gender CHECK (gender IN ('M', 'F', 'U')) --  性别约束（男、女、未知），应用层程序转换成对应的汉字
);

-- 3. 客户车辆信息表
CREATE TABLE customer_car_info
(
    id        VARCHAR(20) PRIMARY KEY, --  车辆编号（主键，原KCarID）
    condition VARCHAR(20) NOT NULL,    --  车辆情况（原KCarQ）
    color     VARCHAR(20) NOT NULL,    --  车辆颜色（原yanse）
    cust_name VARCHAR(50) NOT NULL,    --  所属客户姓名
    model     VARCHAR(20) NOT NULL     --  所属客户车辆型号（原KCarT）
);

-- 4. 美容保养项目表
CREATE TABLE beauty_maintenance_proj
(
    id        VARCHAR(20) PRIMARY KEY,              --  项目编号（主键，原MeirongID）
    proj_name VARCHAR(20)    NOT NULL,              --  项目名称
    price     DECIMAL(15, 2) NOT NULL DEFAULT 0.00, --  项目价格（实际上SQLite中实例是REAL类型，原MeirongType）
    group_id  VARCHAR(20)    NOT NULL               --  班组编号
);

-- 5. 职员信息表
CREATE TABLE employee_info
(
    id                    VARCHAR(20) PRIMARY KEY,              --  职员编号（主键）
    name                  VARCHAR(50)    NOT NULL,              --  职员姓名
    gender                CHAR(1)        NOT NULL DEFAULT 'M',  --  性别（男、女、未知）
    email                 VARCHAR(30)    NOT NULL DEFAULT '',   --  电子邮件地址
    phone                 VARCHAR(20)    NOT NULL DEFAULT '',   --  电话号码
    address               VARCHAR(100)   NOT NULL DEFAULT '',   --  住址
    zip_code              VARCHAR(10)    NOT NULL DEFAULT '',   --  邮政编码
    group_id              VARCHAR(20)    NOT NULL DEFAULT '',   --  班组编号
    monthly_working_hours DECIMAL(5, 2)  NOT NULL DEFAULT 0.00, --  本月总工时
    monthly_salary        DECIMAL(15, 2) NOT NULL DEFAULT 0.00  --  本月工资数
);

-- 6. 配件信息表
CREATE TABLE parts_info
(
    id             VARCHAR(20) PRIMARY KEY,              --  配件编号（主键）
    model          VARCHAR(20)    NOT NULL,              -- 配件型号
    name           VARCHAR(50)    NOT NULL DEFAULT '',   -- 配件名称
    number         INT            NOT NULL DEFAULT '',   -- 配件数量
    classification VARCHAR(20)    NOT NULL DEFAULT '',   -- 分类
    price          DECIMAL(15, 2) NOT NULL DEFAULT 0.00, -- 零售价格
    remarks        VARCHAR(100)   NOT NULL DEFAULT ''    -- 备注
);

