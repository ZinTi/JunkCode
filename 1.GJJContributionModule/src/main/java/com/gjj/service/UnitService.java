package com.gjj.service;

import com.gjj.entity.Unit;

import java.util.List;

/**
 * 单位基本信息服务接口
 * 提供单位信息的业务操作功能
 */
public interface UnitService {

    /**
     * 新增单位信息
     * @param unit 单位信息对象
     * @return 操作结果 + 消息
     */
    Result.CommonResult createUnit(Unit unit);

    /**
     * 删除单位信息
     * @param unitAccNumber 单位公积金账号
     * @return 操作结果 + 消息
     */
    Result.CommonResult deleteUnit(String unitAccNumber);

    /**
     * 更新单位完整信息
     * @param unit 单位信息对象
     * @return 操作结果 + 消息
     */
    Result.CommonResult updateUnit(Unit unit);

    /**
     * 更新单位账号状态
     * @param unitAccNumber 单位公积金账号
     * @param accountStatus 新的账户状态
     * @return 操作结果消息
     */
    Result.CommonResult updateUnitStatus(String unitAccNumber, String accountStatus);

    /**
     * 根据主键查询单位信息
     * @param unitAccNumber 单位公积金账号
     * @return 单位信息对象，未找到时返回null
     */
    Unit getUnitByAccNumber(String unitAccNumber);

    /**
     * 根据单位名称模糊查询
     * @param unitName 单位名称
     * @return 匹配的单位信息列表
     */
    List<Unit> searchUnitsByName(String unitName);
}