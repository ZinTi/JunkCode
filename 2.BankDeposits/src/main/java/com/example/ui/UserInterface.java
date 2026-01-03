package com.example.ui;

import com.example.model.CurrentDeposit;
import com.example.model.FixedDeposit;
import com.example.model.FixedDeposit.Term;
import com.example.service.BankService;
import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;

public class UserInterface extends JFrame {

    private BankService bankService = new BankService();
    private JTabbedPane tabbedPane;

    // 活期存款组件
    private JTextField currentNameField;
    private JTextField currentAccountField;
    private JTextField currentAmountField;
    private JTextField currentStartDateField;
    private JTextField currentEndDateField;
    private JTextArea currentResultArea;

    // 定期存款组件
    private JTextField fixedNameField;
    private JTextField fixedAccountField;
    private JTextField fixedAmountField;
    private JComboBox<Term> termComboBox;
    private JTextField fixedDepositDateField;
    private JTextArea fixedResultArea;

    public UserInterface() {
        setTitle("居民个人储蓄存款业务问题（利率数据：中国建设银行-2025年07月06日）");
        setSize(600, 500);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        tabbedPane = new JTabbedPane();
        tabbedPane.addTab("活期存款", createCurrentDepositPanel());
        tabbedPane.addTab("定期存款", createFixedDepositPanel());

        add(tabbedPane);
    }

    private JPanel createCurrentDepositPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(new EmptyBorder(10, 10, 10, 10));

        // 输入表单
        JPanel formPanel = new JPanel(new GridLayout(5, 2, 5, 5));

        formPanel.add(new JLabel("存款人姓名:"));
        currentNameField = new JTextField();
        formPanel.add(currentNameField);

        formPanel.add(new JLabel("账号:"));
        currentAccountField = new JTextField();
        formPanel.add(currentAccountField);

        formPanel.add(new JLabel("存款金额:"));
        currentAmountField = new JTextField();
        formPanel.add(currentAmountField);

        formPanel.add(new JLabel("起存日期 (yyyy-MM-dd):"));
        currentStartDateField = new JTextField();
        formPanel.add(currentStartDateField);

        formPanel.add(new JLabel("结束日期 (yyyy-MM-dd):"));
        currentEndDateField = new JTextField();
        formPanel.add(currentEndDateField);

        // 计算按钮
        JButton calculateButton = new JButton("计算利息");
        calculateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                calculateCurrentDeposit();
            }
        });

        // 结果区域
        currentResultArea = new JTextArea();
        currentResultArea.setEditable(false);
        currentResultArea.setBorder(BorderFactory.createTitledBorder("银行存款活期利息计算结果"));

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        buttonPanel.add(calculateButton);

        panel.add(formPanel, BorderLayout.NORTH);
        panel.add(new JScrollPane(currentResultArea), BorderLayout.CENTER); // 结果区域占据中心
        panel.add(buttonPanel, BorderLayout.SOUTH); // 按钮放在底部

        return panel;
    }

    private JPanel createFixedDepositPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(new EmptyBorder(10, 10, 10, 10));

        // 输入表单
        JPanel formPanel = new JPanel(new GridLayout(5, 2, 5, 5));

        formPanel.add(new JLabel("存款人姓名:"));
        fixedNameField = new JTextField();
        formPanel.add(fixedNameField);

        formPanel.add(new JLabel("账号:"));
        fixedAccountField = new JTextField();
        formPanel.add(fixedAccountField);

        formPanel.add(new JLabel("存款金额:"));
        fixedAmountField = new JTextField();
        formPanel.add(fixedAmountField);

        formPanel.add(new JLabel("存款期限:"));
        termComboBox = new JComboBox<>(Term.values());
        formPanel.add(termComboBox);

        formPanel.add(new JLabel("存款日期 (yyyy-MM-dd):"));
        fixedDepositDateField = new JTextField(LocalDate.now().toString());
        formPanel.add(fixedDepositDateField);

        // 计算按钮
        JButton calculateButton = new JButton("计算利息");
        calculateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                calculateFixedDeposit();
            }
        });

        // 结果区域
        fixedResultArea = new JTextArea();
        fixedResultArea.setEditable(false);
        fixedResultArea.setBorder(BorderFactory.createTitledBorder("银行存款定期（整存整取）利息计算结果"));

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        buttonPanel.add(calculateButton);

        panel.add(formPanel, BorderLayout.NORTH);
        panel.add(new JScrollPane(fixedResultArea), BorderLayout.CENTER); // 结果区域占据中心
        panel.add(buttonPanel, BorderLayout.SOUTH); // 按钮放在底部

        return panel;
    }

    private void calculateCurrentDeposit() {
        try {
            String name = currentNameField.getText().trim();
            String account = currentAccountField.getText().trim();
            double amount = Double.parseDouble(currentAmountField.getText().trim());
            LocalDate startDate = parseDate(currentStartDateField.getText().trim());
            LocalDate endDate = parseDate(currentEndDateField.getText().trim());

            if (endDate.isBefore(startDate)) {
                JOptionPane.showMessageDialog(this, "结束日期不能早于开始日期", "输入错误", JOptionPane.ERROR_MESSAGE);
                return;
            }

            CurrentDeposit deposit = bankService.createCurrentDeposit(name, account, amount, startDate, endDate);

            String result = "存款人姓名: " + deposit.getDepositorName() + "\n" +
                    "账号: " + deposit.getAccountNumber() + "\n" +
                    "存款金额: " + deposit.getAmount() + " 元\n" +
                    "起存日期: " + deposit.getStartDate() + "\n" +
                    "结束日期: " + deposit.getEndDate() + "\n" +
                    "存款天数: " + deposit.getDays() + " 天\n" +
                    "活期利息: " + String.format("%.2f", deposit.getInterest()) + " 元\n" +
                    "余额: " + String.format("%.2f", deposit.getBalance()) + " 元";

            currentResultArea.setText(result);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "请输入有效的金额", "输入错误", JOptionPane.ERROR_MESSAGE);
        } catch (DateTimeParseException e) {
            JOptionPane.showMessageDialog(this, "日期格式不正确，请使用yyyy-MM-dd格式", "输入错误", JOptionPane.ERROR_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "发生错误: " + e.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void calculateFixedDeposit() {
        try {
            String name = fixedNameField.getText().trim();
            String account = fixedAccountField.getText().trim();
            double amount = Double.parseDouble(fixedAmountField.getText().trim());
            Term term = (Term) termComboBox.getSelectedItem();
            LocalDate depositDate = parseDate(fixedDepositDateField.getText().trim());

            FixedDeposit deposit = bankService.createFixedDeposit(name, account, amount, term, depositDate);

            String result = "存款人姓名: " + deposit.getDepositorName() + "\n" +
                    "账号: " + deposit.getAccountNumber() + "\n" +
                    "存款金额: " + deposit.getAmount() + " 元\n" +
                    "存款期限: " + term.name().replace("_", " ") + "\n" +
                    "年利率: " + (term.getInterestRate() * 100) + "%\n" +
                    "存款日期: " + deposit.getDepositDate() + "\n" +
                    "到期时间: " + deposit.getMaturityDate() + "\n" +
                    "到期利息: " + String.format("%.2f", deposit.getInterest()) + " 元";

            fixedResultArea.setText(result);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "请输入有效的金额", "输入错误", JOptionPane.ERROR_MESSAGE);
        } catch (DateTimeParseException e) {
            JOptionPane.showMessageDialog(this, "日期格式不正确，请使用yyyy-MM-dd格式", "输入错误", JOptionPane.ERROR_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "发生错误: " + e.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
        }
    }

    private LocalDate parseDate(String dateString) throws DateTimeParseException {
        return LocalDate.parse(dateString, DateTimeFormatter.ISO_LOCAL_DATE);
    }


}