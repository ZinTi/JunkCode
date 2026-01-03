/**
 * 存储基类
 */
package com.example.model;

import lombok.Getter;

@Getter
public abstract class Deposit {
    protected String depositorName; // 存款人姓名
    protected String accountNumber; //  账号
    protected double amount;        // 余额

    public Deposit(String depositorName, String accountNumber, double amount) {
        this.depositorName = depositorName;
        this.accountNumber = accountNumber;
        this.amount = amount;
    }

    protected abstract double calculateDepositInterest();

    public abstract void debugPrint();
}