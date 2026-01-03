/**
 * MySQL Ver 8.0.40 for Win64 on x86_64 (MySQL Community Server - GPL)
 *
 * Description: 住房公积金管理系统——筹集子系统
 * Schema version: v1.0
 * Last modified: 2025-06
 */

-- 检查数据库是否存在，如果存在则删除
SELECT CONCAT('[', NOW(), '] ', '正在删除旧版数据库: housing_provident_fund') AS log;
DROP DATABASE IF EXISTS housing_provident_fund;

-- 新的数据库
SELECT CONCAT('[', NOW(), '] ', '正在创建数据库: housing_provident_fund') AS log;
CREATE DATABASE housing_provident_fund CHARACTER SET utf8mb4;

-- 选择使用该数据库
SELECT CONCAT('[', NOW(), '] ', '正在切换数据库: housing_provident_fund') AS log;
USE housing_provident_fund;

-- 这个表不用，通过业务代码约束输入性别只能为 'Male', 'Female', 'Unknown'
SELECT CONCAT('[', NOW(), '] ', '正在创建gender性别词库表和插入数据') AS log;
CREATE TABLE gender
(
    id VARCHAR(50) PRIMARY KEY COMMENT '性别称呼',
    val ENUM('Male', 'Female', 'Unknown') NOT NULL COMMENT '性别值'
);

SELECT CONCAT('[', NOW(), '] ', '正在创建表: 用户表 users') AS log;
CREATE TABLE users
(
    id BINARY(16) NOT NULL COMMENT '用户ID', -- 系统分配 UUID
    username VARCHAR(6) NOT NULL UNIQUE COMMENT '用户名', -- 可用于登录的账号名，仅[09AZaz_]
    nickname VARCHAR(50) NOT NULL COMMENT '昵称',
    password VARCHAR(255) NOT NULL COMMENT '密文密码',
    salt VARCHAR(255) NOT NULL COMMENT '盐值', -- 防止彩虹表攻击
    email VARCHAR(255) NOT NULL UNIQUE COMMENT '绑定邮箱',
    telephone VARCHAR(20) NOT NULL UNIQUE COMMENT '手机号',  -- 为简化使用本地文件路径
    gender ENUM('Male', 'Female', 'Unknown') NOT NULL DEFAULT 'Unknown' COMMENT '性别',
    avatar_path TEXT NULL COMMENT '头像路径',
    user_type ENUM('admin', 'teller', 'suspended') NOT NULL DEFAULT 'teller' COMMENT '用户类型',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP COMMENT '注册时间',
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '修改时间',
    CONSTRAINT pk_users PRIMARY KEY (id),
    CONSTRAINT chk_valid_format CHECK (username REGEXP '^[A-Za-z_][A-Za-z0-9_]*$') -- 正则约束：以大小写字母或下划线为首，大小写字母数字下划线结尾
) ENGINE=InnoDB CHARACTER SET utf8mb4 COMMENT '用户表';

SELECT CONCAT('[', NOW(), '] ', '正在创建表: 系统序列参数表 sys_sequence') AS log;
CREATE TABLE sys_sequence
(
    seq_name VARCHAR(50) NOT NULL COMMENT '序列名称（对应业务表名）',
    current_val BIGINT NOT NULL COMMENT '当前序号值', -- 从1开始，每次取完递增
    max_val BIGINT NOT NULL COMMENT '最大允许序号',
    description VARCHAR(100) NULL COMMENT '序列描述',
    remark TEXT NULL COMMENT '备注',
    CONSTRAINT pk_sys_sequence PRIMARY KEY (seq_name)
) ENGINE=InnoDB CHARACTER SET utf8mb4 COMMENT '系统序列参数表';

-- 通过存储过程更新序列值性能最佳，缺点是比应用层更新的方式更难维护和跨平台性差，最不推荐触发器的方式
SELECT CONCAT('[', NOW(), '] ', '正在创建存储过程: 获取下一个序列值的存储过程 get_next_sequence') AS log;
DELIMITER $$
CREATE PROCEDURE get_next_sequence_str(
    IN p_seq_name VARCHAR(50),
    OUT p_next_val CHAR(12)
)
BEGIN
    DECLARE v_next BIGINT;

    START TRANSACTION;
    -- 锁定行防止并发问题
    SELECT current_val INTO v_next
    FROM sys_sequence
    WHERE seq_name = p_seq_name
        FOR UPDATE;

    -- 计算新值
    SET v_next = v_next + 1;

    -- 更新序列值
    UPDATE sys_sequence
    SET current_val = v_next
    WHERE seq_name = p_seq_name;

    COMMIT;

    -- 在MySQL中格式化数字
    SET p_next_val = LPAD(v_next, 12, '0');
END$$
DELIMITER ;

SELECT CONCAT('[', NOW(), '] ', '正在创建表: 单位基本资料表 unit') AS log;
CREATE TABLE unit
(
    unit_acc_number VARCHAR(12) NOT NULL COMMENT '单位公积金账号',
    unit_name VARCHAR(100) NOT NULL COMMENT '单位名称',
    unit_address VARCHAR(200) NOT NULL COMMENT '单位地址',
    org_code VARCHAR(20) NOT NULL COMMENT '组织机构代码',
    unit_category VARCHAR(20) NOT NULL COMMENT '单位类别', -- 1-企业 2-事业 3-机关 4-团体 5-其他
    unit_type VARCHAR(30) NULL COMMENT '企业类型', -- 110-国有经济、120-集体经济、130-股份合作企业、140-联营企业、150-有限责任公司、160-股份有限公司、170-私营企业 、190-其他企业、200-港、澳、台商投资企业、300-外商投资企业、900-其他
    salary_date TINYINT NULL COMMENT '发薪日期 (1-31)',
    unit_phone VARCHAR(20) NOT NULL COMMENT '单位联系电话',
    contact_person VARCHAR(30) NOT NULL COMMENT '单位联系人',
    agent_id CHAR(18) NULL COMMENT '经办人身份证号码',
    -- 状态：正常、封存、销户
    account_status ENUM('normal','sealed','closed') NOT NULL DEFAULT 'normal' COMMENT '账户状态',
    fund_balance DECIMAL(16,2) NOT NULL DEFAULT 0.00 COMMENT '公积金余额',
    deposit_base DECIMAL(16,2) NOT NULL COMMENT '缴存基数',
    unit_ratio DECIMAL(6,3) NOT NULL COMMENT '单位比例 (%)',
    employee_ratio DECIMAL(6,3) NOT NULL COMMENT '个人比例 (%)',
    unit_monthly_due DECIMAL(16,2) NOT NULL COMMENT '单位月应缴额',
    employee_monthly_due DECIMAL(16,2) NOT NULL COMMENT '个人月应缴额',
    employee_count INT NOT NULL COMMENT '单位人数', -- 非销户人数
    last_payment_month VARCHAR(7) NOT NULL COMMENT '最后汇缴月 (YYYY-MM格式)',
    institution_code CHAR(8) NOT NULL COMMENT '公积金中心机构代码',
    operator VARCHAR(6) NOT NULL COMMENT '柜员',
    create_datetime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '建立时间',
    remark TEXT NULL COMMENT '备注',

    CONSTRAINT pk_unit PRIMARY KEY (unit_acc_number),
    CONSTRAINT fk_unit_1 FOREIGN KEY (operator) REFERENCES users(username)
        ON DELETE RESTRICT -- 删除保护
        ON UPDATE CASCADE,   -- 级联更新
    UNIQUE KEY (org_code)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='单位基本资料表';

SELECT CONCAT('[', NOW(), '] ', '正在创建表: 个人基本资料表 personal') AS log;
CREATE TABLE personal
(
    personal_acc_number VARCHAR(12) NOT NULL COMMENT '个人公积金账号',
    unit_acc_number VARCHAR(12) NOT NULL COMMENT '单位公积金账号',
    open_date DATE NOT NULL COMMENT '开户日期',
    fund_balance DECIMAL(16,2) NOT NULL DEFAULT 0.00 COMMENT '公积金余额',
    -- 状态：正常、封存、销户
    account_status ENUM('normal','sealed','closed') NOT NULL DEFAULT 'normal' COMMENT '个人账户状态',
    deposit_base DECIMAL(16,2) NOT NULL COMMENT '缴存基数',
    unit_ratio DECIMAL(6,3) NOT NULL COMMENT '单位比例 (%)',
    personal_ratio DECIMAL(6,3) NOT NULL COMMENT '个人比例 (%)',
    last_payment_month DATE NULL COMMENT '最后汇缴月 (YYYY-MM格式)',
    unit_monthly_due DECIMAL(16,2) NULL COMMENT '单位月应缴额',
    personal_monthly_due DECIMAL(16,2) NULL COMMENT '个人月应缴额',
    annual_payment DECIMAL(16,2) NOT NULL DEFAULT 0.00 COMMENT '本年汇补缴额',
    annual_withdrawal DECIMAL(16,2) NOT NULL DEFAULT 0.00 COMMENT '年提取额',
    annual_interest DECIMAL(16,2) NOT NULL DEFAULT 0.00 COMMENT '年度结息',
    institution_code CHAR(8) NOT NULL COMMENT '公积金中心机构代码',
    operator VARCHAR(6) NULL COMMENT '柜员',
    remark TEXT NULL COMMENT '备注',

    CONSTRAINT pk_personal PRIMARY KEY (personal_acc_number),
    CONSTRAINT fk_personal_1 FOREIGN KEY (unit_acc_number) REFERENCES unit(unit_acc_number)
        ON DELETE RESTRICT -- 删除保护
        ON UPDATE CASCADE,   -- 级联更新
    CONSTRAINT fk_personal_2 FOREIGN KEY (operator) REFERENCES users(username)
        ON DELETE RESTRICT -- 删除保护
        ON UPDATE CASCADE,   -- 级联更新
    INDEX idx_unit_acc (unit_acc_number)  -- 索引
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='个人基本资料表';
