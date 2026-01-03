INSERT INTO admin_info (id, name, salt, pwd)
VALUES
    ('admin', '超级管理员', '123456', 'e10adc3949ba59abbe56e057f20f883e'),
    ('alice', '艾莉丝', '123456', 'e10adc3949ba59abbe56e057f20f883e'),
    ('bob', '鲍勃', '123456', 'e10adc3949ba59abbe56e057f20f883e');

INSERT INTO customer_info (id, name, salt, pwd, gender, email, phone, address, zip_code, car_model, car_condition, car_id)
VALUES
    ('00001', '张三', '123456', 'e10adc3949ba59abbe56e057f20f883e', 'M', 'zhangsan@163.com', '13800138000', '北京市海淀区西二旗', '100000', '宝马X5', '新车', 'B001'),
    ('00002', '李四', '123456', 'e10adc3949ba59abbe56e057f20f883e', 'M', 'lisi@163.com', '13800138001', '北京市海淀区西二旗', '100000', '奥迪A8', '新车', 'B002');

INSERT INTO customer_car_info(id, condition, color, cust_name, model)
VALUES
    ('0001', '新车', '红色', '张三', '宝马X5'),
    ('0002', '新车', '蓝色', '李四', '奔驰C级');

INSERT INTO beauty_maintenance_proj (id, proj_name, price, group_id)
VALUES
    ('0001', '车漆保养', 10000.00, '0001'),
    ('0002', '轮胎保养', 15000.00, '0001');

INSERT INTO employee_info (id, name, gender, email, phone, address, zip_code, group_id, monthly_working_hours, monthly_salary)
VALUES
    ('0001', '张三', 'M', 'zhangsan@163.com', '13800138000', '北京市海淀区西二旗', '100000', '0001', 40.00, 5000.00),

-- 5. 职员信息表
CREATE TABLE employee_info
(
    id                    VARCHAR(20) PRIMARY KEY DEFAULT '(无)', --  职员编号（主键）
    name                  VARCHAR(50)    NOT NULL DEFAULT '(无)', --  职员姓名
    gender                CHAR(1)        NOT NULL DEFAULT 'M',    --  性别（男、女、未知）
    email                 VARCHAR(30)    NOT NULL DEFAULT '',     --  电子邮件地址
    phone                 VARCHAR(20)    NOT NULL DEFAULT '',     --  电话号码
    address               VARCHAR(100)   NOT NULL DEFAULT '',     --  住址
    zip_code              VARCHAR(10)    NOT NULL DEFAULT '',     --  邮政编码
    group_id              VARCHAR(20)    NOT NULL DEFAULT '',     --  班组编号
    monthly_working_hours DECIMAL(5, 2)  NOT NULL DEFAULT 0.00, --  本月总工时
    monthly_salary        DECIMAL(15, 2) NOT NULL DEFAULT 0.00  --  本月工资数
);

-- 6. 配件信息表
CREATE TABLE parts_info
(
    id             VARCHAR(20) PRIMARY KEY DEFAULT '(无)', --  配件编号（主键）
    model          VARCHAR(20)    NOT NULL DEFAULT '(无)', -- 配件型号
    name           VARCHAR(50)    NOT NULL DEFAULT '',     -- 配件名称
    number         INT            NOT NULL DEFAULT '',     -- 配件数量
    classification VARCHAR(20)    NOT NULL DEFAULT '',     -- 分类
    price          DECIMAL(15, 2) NOT NULL DEFAULT '',     -- 零售价格
    remarks        VARCHAR(100)   NOT NULL DEFAULT ''      -- 备注
);
