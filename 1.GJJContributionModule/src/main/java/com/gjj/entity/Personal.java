package com.gjj.entity;

import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;

@Setter
@Getter
public class Personal {
    private String personalAccNumber;                       // 个人公积金账号
    private String unitAccNumber;                           // 单位公积金账号（外键）
    private LocalDate openDate;                             // 开户日期
    private BigDecimal fundBalance = BigDecimal.ZERO;       // 公积金余额
    private String accountStatus = AccountStatus.NORMAL;    // 账户状态
    private BigDecimal depositBase;                         // 缴存基数
    private BigDecimal unitRatio;                           // 单位比例 (%)
    private BigDecimal personalRatio;                       // 个人比例 (%)
    private String lastPaymentMonth;                        // 最后汇缴月 (YYYY-MM)
    private BigDecimal unitMonthlyDue;                      // 单位月应缴额
    private BigDecimal personalMonthlyDue;                  // 个人月应缴额
    private BigDecimal annualPayment = BigDecimal.ZERO;     // 本年汇补缴额
    private BigDecimal annualWithdrawal = BigDecimal.ZERO;  // 年提取额
    private BigDecimal annualInterest = BigDecimal.ZERO;    // 年度结息
    private String institutionCode;                         // 公积金中心机构代码
    private String operator;                                // 柜员
    private String remark;                                  // 备注

    // 账户状态常量类
    public static final class AccountStatus {
        public static final String NORMAL = "normal";   // 正常
        public static final String SEALED = "sealed";   // 封存
        public static final String CLOSED = "closed";   // 销户

        private AccountStatus() {} // 防止实例化
    }

    public Personal() {
    }

    public Personal(String personalAccNumber, String unitAccNumber, LocalDate openDate, BigDecimal fundBalance, String accountStatus, BigDecimal depositBase, BigDecimal unitRatio, BigDecimal personalRatio, String lastPaymentMonth, BigDecimal unitMonthlyDue, BigDecimal personalMonthlyDue, BigDecimal annualPayment, BigDecimal annualWithdrawal, BigDecimal annualInterest, String institutionCode, String operator, String remark) {
        this.personalAccNumber = personalAccNumber;
        this.unitAccNumber = unitAccNumber;
        this.openDate = openDate;
        this.fundBalance = fundBalance;
        this.accountStatus = accountStatus;
        this.depositBase = depositBase;
        this.unitRatio = unitRatio;
        this.personalRatio = personalRatio;
        this.lastPaymentMonth = lastPaymentMonth;
        this.unitMonthlyDue = unitMonthlyDue;
        this.personalMonthlyDue = personalMonthlyDue;
        this.annualPayment = annualPayment;
        this.annualWithdrawal = annualWithdrawal;
        this.annualInterest = annualInterest;
        this.institutionCode = institutionCode;
        this.operator = operator;
        this.remark = remark;
    }
}