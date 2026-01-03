/**
 * 测试用例
 */

import com.example.model.Deposit;
import com.example.model.FixedDeposit;
import com.example.service.BankService;

import java.time.LocalDate;

public class TestBankSvc {
    public static void main(String[] args) {
        BankService bankService = new BankService();

        // 创建活期存款（起止日期）
        Deposit current = bankService.createCurrentDeposit(
                "张三", "C1001", 5000.0,
                LocalDate.of(2023, 1, 1),
                LocalDate.of(2023, 12, 31));

        // 创建定期存款 (3个月)
        Deposit fixed1 = bankService.createFixedDeposit(
                "李四", "F2001", 10000.0,
                FixedDeposit.Term.THREE_MONTHS, LocalDate.now());

        // 创建定期存款 (2年)
        Deposit fixed2 = bankService.createFixedDeposit(
                "王五", "F3001", 20000.0,
                FixedDeposit.Term.TWO_YEARS, LocalDate.now());

        // 显示存款信息
        current.debugPrint();
        fixed1.debugPrint();
        fixed2.debugPrint();
    }
}
