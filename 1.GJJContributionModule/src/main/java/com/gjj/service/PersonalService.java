package com.gjj.service;

import com.gjj.entity.Personal;

import java.util.List;

/**
 * 个人公积金账户服务接口
 * 提供个人公积金账户的业务操作功能
 */
public interface PersonalService {

    /**
     * 创建个人公积金账户
     * @param personal 个人账户信息对象
     * @return 操作结果消息
     */
    String createPersonalAccount(Personal personal);

    /**
     * 删除个人公积金账户
     * @param personalAccNumber 个人公积金账号
     * @return 操作结果消息
     */
    String deletePersonalAccount(String personalAccNumber);

    /**
     * 更新个人账户完整信息
     * @param personal 个人账户信息对象
     * @return 操作结果消息
     */
    String updatePersonalAccount(Personal personal);

    /**
     * 更新个人账户状态
     * @param personalAccNumber 个人公积金账号
     * @param accountStatus 新的账户状态
     * @return 操作结果消息
     */
    String updateAccountStatus(String personalAccNumber, String accountStatus);

    /**
     * 根据主键查询个人账户信息
     * @param personalAccNumber 个人公积金账号
     * @return 个人账户信息对象，未找到时返回null
     */
    Personal getPersonalAccount(String personalAccNumber);

    /**
     * 根据单位账号查询关联的个人账户列表
     * @param unitAccNumber 单位公积金账号
     * @return 关联的个人账户列表
     */
    List<Personal> getAccountsByUnit(String unitAccNumber);
}