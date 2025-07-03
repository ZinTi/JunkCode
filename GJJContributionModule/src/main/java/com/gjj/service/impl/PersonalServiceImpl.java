package com.gjj.service.impl;

import com.gjj.entity.Personal;
import com.gjj.mapper.PersonalMapper;
import com.gjj.service.PersonalService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.List;

/**
 * 个人公积金账户服务实现类
 * 提供个人账户业务操作的具体实现
 */
@Service
public class PersonalServiceImpl implements PersonalService {

    private final PersonalMapper personalMapper;

    @Autowired
    public PersonalServiceImpl(PersonalMapper personalMapper) {
        this.personalMapper = personalMapper;
    }

    @Override
    public String createPersonalAccount(Personal personal) {
        try {
            // 验证关联的单位是否存在（可选，取决于业务需求）
            // 验证账户状态
            if (!isValidAccountStatus(personal.getAccountStatus())) {
                return "账户创建失败: 无效的账户状态值";
            }

            personalMapper.insert(personal);
            return "个人账户创建成功";
        } catch (Exception e) {
            String errorMessage = "个人账户创建失败: ";

            if (e.getMessage().contains("个人账号已存在")) {
                errorMessage += "个人账号已存在";
            } else if (e.getMessage().contains("关联的单位账号不存在")) {
                errorMessage += "关联的单位账号不存在";
            } else {
                errorMessage += "系统错误：" + e.getMessage();
            }

            return errorMessage;
        }
    }

    @Override
    public String deletePersonalAccount(String personalAccNumber) {
        try {
            personalMapper.deleteByPersonalAccNumber(personalAccNumber);
            return "个人账户删除成功";
        } catch (Exception e) {
            if (e.getMessage().contains("个人账号不存在")) {
                return "账户删除失败: 指定的个人账号不存在";
            }
            return "系统错误。账户删除失败: " + e.getMessage();
        }
    }

    @Override
    public String updatePersonalAccount(Personal personal) {
        try {
            // 验证账户状态
            if (!isValidAccountStatus(personal.getAccountStatus())) {
                return "账户更新失败: 无效的账户状态值";
            }

            personalMapper.update(personal);
            return "个人账户信息更新成功";
        } catch (Exception e) {
            String errorMessage = "个人账户更新失败: ";

            if (e.getMessage().contains("个人账号不存在")) {
                errorMessage += "指定的个人账号不存在";
            } else if (e.getMessage().contains("关联的单位账号不存在")) {
                errorMessage += "关联的单位账号不存在";
            } else {
                errorMessage += "系统错误：" + e.getMessage();
            }

            return errorMessage;
        }
    }

    @Override
    public String updateAccountStatus(String personalAccNumber, String accountStatus) {
        try {
            // 验证状态值是否合法
            if (!isValidAccountStatus(accountStatus)) {
                return "状态更新失败: 无效的账户状态值";
            }

            personalMapper.updateAccountStatus(personalAccNumber, accountStatus);
            return "个人账户状态更新成功";
        } catch (Exception e) {
            if (e.getMessage().contains("个人账号不存在")) {
                return "状态更新失败: 指定的个人账号不存在";
            }
            return "系统错误。状态更新失败: " + e.getMessage();
        }
    }

    /**
     * 验证账户状态是否合法
     * @param status 待验证的状态值
     * @return 是否合法
     */
    private boolean isValidAccountStatus(String status) {
        return Personal.AccountStatus.NORMAL.equals(status) ||
                Personal.AccountStatus.SEALED.equals(status) ||
                Personal.AccountStatus.CLOSED.equals(status);
    }

    @Override
    public Personal getPersonalAccount(String personalAccNumber) {
        try {
            return personalMapper.findByPersonalAccNumber(personalAccNumber);
        } catch (Exception e) {
            // 记录日志
            System.err.println("查询个人账户失败: " + e.getMessage());
            return null;
        }
    }

    @Override
    public List<Personal> getAccountsByUnit(String unitAccNumber) {
        try {
            return personalMapper.findByUnitAccNumber(unitAccNumber);
        } catch (Exception e) {
            // 记录日志
            System.err.println("查询单位关联账户失败: " + e.getMessage());
            return Collections.emptyList(); // 返回空列表
        }
    }
}