/**
 * 定期存款
 * 截至北京时间 2025 年 07 月 06 日 15 时，查询 中国建设银行-人民币-整存整取 存款利率（年利率）如下：
 * 三个月：0.0065
 * 半年（六个月）：0.0085
 * 一年：0.0095
 * 两年：0.0105
 * 三年：0.0125
 * 五年：0.0130
 */
package com.example.model;

import lombok.Getter;

import java.time.LocalDate;
import java.time.Period;

@Getter
public class FixedDeposit extends Deposit {
    /**
     * 期限选项：三个月、半年、一年、两年、三年、五年
     */
    @Getter
    public enum Term {
        THREE_MONTHS(0.0065, Period.ofMonths(3)),
        SIX_MONTHS(0.0085, Period.ofMonths(6)),
        ONE_YEAR(0.0095, Period.ofYears(1)),
        TWO_YEARS(0.0105, Period.ofYears(2)),
        THREE_YEARS(0.0125, Period.ofYears(3)),
        FIVE_YEARS(0.0130, Period.ofYears(5));

        private final double interestRate;
        private final Period period;

        Term(double interestRate, Period period) {
            this.interestRate = interestRate;
            this.period = period;
        }

    }

    private final Term term;
    private final LocalDate depositDate;    // 存款日期
    private final LocalDate maturityDate;    // 到期日期
    private final double interest;

    public FixedDeposit(String depositorName, String accountNumber, double amount, Term term, LocalDate depositDate) {
        super(depositorName, accountNumber, amount);
        this.term = term;
        this.depositDate = depositDate;
        this.maturityDate = depositDate.plus(term.getPeriod());
        this.interest = this.calculateDepositInterest();
    }

    /**
     * 计算定期利息（整存整取）
     * @return 定期利息
     */
    @Override
    protected double calculateDepositInterest(){
        double years = this.term.getPeriod().getYears() + this.term.getPeriod().getMonths() / 12.0;
        return this.amount * this.term.getInterestRate() * years;
    }

    @Override
    public void debugPrint() {
        System.out.println("定期存款信息:");
        System.out.println("存款人姓名: " + depositorName);
        System.out.println("账号: " + accountNumber);
        System.out.println("存款金额: " + amount + " 元");
        System.out.println("存款期限: " + term.name().replace("_", " "));
        System.out.println("年利率: " + (term.getInterestRate() * 100) + "%");
        System.out.println("存款日期: " + depositDate);
        System.out.println("到期时间: " + maturityDate);
        System.out.println("到期利息: " + String.format("%.2f", interest) + " 元");
        System.out.println("----------------------------");
    }
}