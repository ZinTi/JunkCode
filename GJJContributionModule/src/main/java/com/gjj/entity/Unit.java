package com.gjj.entity;

import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Setter
@Getter
public class Unit {
    private String unitAccNumber;       // 单位公积金账号
    private String unitName;            // 单位名称
    private String unitAddress;         // 单位地址
    private String orgCode;             // 组织机构代码
    private String unitCategory;        // 单位类别
    private String unitType;            // 企业类型
    private Byte salaryDate;            // 发薪日期 (1-31)
    private String unitPhone;           // 联系电话
    private String contactPerson;       // 单位联系人
    private String agentId;             // 经办人身份证号码
    private String accountStatus = AccountStatus.NORMAL; // 账户状态
    private BigDecimal fundBalance = BigDecimal.ZERO;    // 公积金余额
    private BigDecimal depositBase;      // 缴存基数
    private BigDecimal unitRatio;        // 单位比例 (%)
    private BigDecimal employeeRatio;    // 个人比例 (%)
    private BigDecimal unitMonthlyDue;   // 单位月应缴额
    private BigDecimal employeeMonthlyDue; // 个人月应缴额
    private Integer employeeCount;       // 单位人数
    private String lastPaymentMonth;  // 最后汇缴月 (YYYY-MM)
    private String institutionCode;      // 公积金中心机构代码
    private String operator;             // 柜员
    private LocalDateTime createDateTime;    // 建立日期
    private String remark;               // 备注 (TEXT)

    /**
     * 账户状态常量类
     * 使用字符串常量代替枚举
     */
    public static final class AccountStatus {
        public static final String NORMAL = "normal";   // 正常
        public static final String SEALED = "sealed";   // 封存
        public static final String CLOSED = "closed";   // 销户

        private AccountStatus() {} // 防止实例化
    }

    public Unit() {
    }

    public Unit(String unitAccNumber, String unitName, String unitAddress, String orgCode, String unitCategory,
                String unitType, Byte salaryDate, String unitPhone, String contactPerson, String agentId,
                String accountStatus, BigDecimal fundBalance, BigDecimal depositBase, BigDecimal unitRatio,
                BigDecimal employeeRatio, BigDecimal unitMonthlyDue, BigDecimal employeeMonthlyDue,
                Integer employeeCount, String lastPaymentMonth, String institutionCode, String operator,
                LocalDateTime createDateTime, String remark)
    {
        this.unitAccNumber = unitAccNumber;
        this.unitName = unitName;
        this.unitAddress = unitAddress;
        this.orgCode = orgCode;
        this.unitCategory = unitCategory;
        this.unitType = unitType;
        this.salaryDate = salaryDate;
        this.unitPhone = unitPhone;
        this.contactPerson = contactPerson;
        this.agentId = agentId;
        this.accountStatus = accountStatus;
        this.fundBalance = fundBalance;
        this.depositBase = depositBase;
        this.unitRatio = unitRatio;
        this.employeeRatio = employeeRatio;
        this.unitMonthlyDue = unitMonthlyDue;
        this.employeeMonthlyDue = employeeMonthlyDue;
        this.employeeCount = employeeCount;
        this.lastPaymentMonth = lastPaymentMonth;
        this.institutionCode = institutionCode;
        this.operator = operator;
        this.createDateTime = createDateTime;
        this.remark = remark;
    }
}