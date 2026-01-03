package com.gjj.dto.req.module;

import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Getter
@Setter
public class UnitAddReq {
    // private String unitAccNumber;       // 单位公积金账号 - auto gen
    private String unitName;            // 单位名称
    private String unitAddress;         // 单位地址
    private String orgCode;             // 组织机构代码
    private String unitCategory;        // 单位类别
    private String unitType;            // 企业类型
    private Byte salaryDate;            // 发薪日期 (1-31)
    private String unitPhone;           // 联系电话
    private String contactPerson;       // 单位联系人
    private String agentId;             // 经办人身份证号码
    private String accountStatus ;    // 账户状态
    private BigDecimal fundBalance;    // 公积金余额
    private BigDecimal depositBase;      // 缴存基数
    private BigDecimal unitRatio;        // 单位比例 (%)
    private BigDecimal employeeRatio;    // 个人比例 (%)
    private BigDecimal unitMonthlyDue;   // 单位月应缴额
    private BigDecimal employeeMonthlyDue; // 个人月应缴额
    private Integer employeeCount;       // 单位人数
    private String lastPaymentMonth;  // 最后汇缴月 (YYYY-MM)
    private String institutionCode;      // 公积金中心机构代码
    // private String operator;             // 柜员 - 根据柜员的 sessionId 查询 username，然后用作 op
    // private LocalDateTime createDateTime;    // 建立日期
    private String remark;               // 备注 (TEXT)
}
