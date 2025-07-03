package com.gjj.service.impl;

import com.gjj.entity.Unit;
import com.gjj.mapper.UnitMapper;
import com.gjj.service.Result;
import com.gjj.service.SysSequenceService;
import com.gjj.service.UnitService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * 单位基本信息服务实现类
 * 提供单位信息的业务操作具体实现
 */
@Service
public class UnitServiceImpl implements UnitService {
    private final UnitMapper unitMapper;
    private final SysSequenceService sysSequenceService;

    @Autowired
    public UnitServiceImpl(UnitMapper unitMapper, SysSequenceService sysSequenceService) {
        this.unitMapper = unitMapper;
        this.sysSequenceService = sysSequenceService;
    }

    @Override
    public Result.CommonResult createUnit(Unit unit) {
        try {
            unitMapper.insert(unit);
            return new Result.CommonResult(true, "单位创建成功");
        } catch (Exception e) {
            String errorMessage = "单位创建失败: ";

            // 处理特定约束错误
            if (e.getMessage().contains("单位账号已存在")) {
                errorMessage += "单位账号已存在";
            } else if (e.getMessage().contains("组织机构代码已存在")) {
                errorMessage += "组织机构代码已存在";
            } else {
                errorMessage += "系统错误：" + e.getMessage();
            }
            return new Result.CommonResult(false, errorMessage);
        }
    }

    @Override
    public Result.CommonResult deleteUnit(String unitAccNumber) {
        try {
            unitMapper.deleteByUnitAccNumber(unitAccNumber);
            return new Result.CommonResult(true, "单位删除成功");
        } catch (Exception e) {
            if (e.getMessage().contains("单位账号不存在")) {
                return new Result.CommonResult(false, "单位删除失败: 指定的单位账号不存在");
            }
            return new Result.CommonResult(false, "系统错误。单位删除失败: " + e.getMessage());
        }
    }

    @Override
    public Result.CommonResult updateUnit(Unit unit) {
        try {
            unitMapper.update(unit);
            return new Result.CommonResult(true,"单位信息更新成功");
        } catch (Exception e) {
            String errorMessage = "单位信息更新失败: ";

            if (e.getMessage().contains("单位账号不存在")) {
                errorMessage += "指定的单位账号不存在";
            } else if (e.getMessage().contains("组织机构代码已存在")) {
                errorMessage += "组织机构代码已存在";
            } else {
                errorMessage += "系统错误：" + e.getMessage();
            }

            return new Result.CommonResult(false, errorMessage);
        }
    }

    @Override
    public Result.CommonResult updateUnitStatus(String unitAccNumber, String accountStatus) {
        try {
            // 验证状态值是否合法
            if (!isValidAccountStatus(accountStatus)) {
                return new Result.CommonResult(false, "状态更新失败: 无效的账户状态值");
            }

            unitMapper.updateAccountStatus(unitAccNumber, accountStatus);
            return new Result.CommonResult(true,"单位状态更新成功");
        } catch (Exception e) {
            if (e.getMessage().contains("单位账号不存在")) {
                return new Result.CommonResult(false, "状态更新失败: 指定的单位账号不存在");
            }
            return new Result.CommonResult(false, "系统错误。状态更新失败: " + e.getMessage());
        }
    }

    /**
     * 验证账户状态是否合法
     * @param status 待验证的状态值
     * @return 是否合法
     */
    private boolean isValidAccountStatus(String status) {
        return Unit.AccountStatus.NORMAL.equals(status) ||
                Unit.AccountStatus.SEALED.equals(status) ||
                Unit.AccountStatus.CLOSED.equals(status);
    }

    @Override
    public Unit getUnitByAccNumber(String unitAccNumber) {
        try {
            return unitMapper.findByUnitAccNumber(unitAccNumber);
        } catch (Exception e) {
            // 记录日志
            System.err.println("查询单位信息失败: " + e.getMessage());
            return null;
        }
    }

    @Override
    public List<Unit> searchUnitsByName(String unitName) {
        try {
            return unitMapper.findByUnitName(unitName);
        } catch (Exception e) {
            // 记录日志
            System.err.println("按名称查询单位失败: " + e.getMessage());
            return List.of(); // 返回空列表
        }
    }
}