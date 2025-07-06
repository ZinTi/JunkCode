/**
 * 活期存款
 * 中国建设银行-人民币-活期-存款利率（年利率）：0.050000%
 */
package com.example.model;

import lombok.Getter;

import java.time.LocalDate;
import java.time.temporal.ChronoUnit;

@Getter
public class CurrentDeposit extends Deposit {
    private final double CURRENT_RATE = 0.0005; // 中国建设银行当前活期利率 (2025年标准)

    private final LocalDate startDate;
    private final LocalDate endDate;
    private final double interest;
    private final double balance;
    private long days;

    public CurrentDeposit(String depositorName, String accountNumber, double amount,
                          LocalDate startDate, LocalDate endDate) {
        super(depositorName, accountNumber, amount);
        this.startDate = startDate;
        this.endDate = endDate;
        this.days = ChronoUnit.DAYS.between(startDate, endDate); // 计算存款天数
        this.interest = this.calculateDepositInterest();
        this.balance = amount + interest;
    }

    /**
     * 计算活期利息
     * @return 活期利息
     */
    @Override
    protected double calculateDepositInterest(){
        return this.amount * this.CURRENT_RATE * this.days / 360;  // 利息 = 本金 × 利率 × 存款天数 / 360
    }

    @Override
    public void debugPrint() {
        System.out.println("活期存款信息:");
        System.out.println("存款人姓名: " + depositorName);
        System.out.println("账号: " + accountNumber);
        System.out.println("存款金额: " + amount + " 元");
        System.out.println("起存日期: " + startDate);
        System.out.println("结束日期: " + endDate);
        System.out.println("存款天数: " + days + " 天");
        System.out.println("活期利息: " + String.format("%.2f", interest) + " 元");
        System.out.println("余额: " + String.format("%.2f", balance) + " 元");
        System.out.println("----------------------------");
    }

}
