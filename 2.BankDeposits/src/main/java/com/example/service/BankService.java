/**
 * 银行服务
 */
package com.example.service;

import com.example.model.CurrentDeposit;
import com.example.model.FixedDeposit;
import com.example.model.FixedDeposit.Term;
import java.time.LocalDate;

public class BankService {
    public CurrentDeposit createCurrentDeposit(
            String name, String account, double amount, LocalDate startDate, LocalDate endDate) {
        return new CurrentDeposit(name, account, amount, startDate, endDate);
    }

    public FixedDeposit createFixedDeposit(
            String name, String account, double amount, Term term, LocalDate depositDate) {
        return new FixedDeposit(name, account, amount, term, depositDate);
    }
}