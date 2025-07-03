package com.gjj.service.impl;

import com.gjj.entity.SysSequence;
import com.gjj.mapper.SysSequenceMapper;
import com.gjj.service.Result;
import com.gjj.service.SysSequenceService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

@Service
public class SysSequenceServiceImpl implements SysSequenceService {
    private final SysSequenceMapper sysSequenceMapper;

    @Autowired
    public SysSequenceServiceImpl(SysSequenceMapper sysSequenceMapper) {
        this.sysSequenceMapper = sysSequenceMapper;
    }

    @Override
    public String getNextSequenceValue(final String seqName) {
        Map<String, Object> paramMap = new HashMap<>();
        paramMap.put("seqName", seqName);

        try {
            sysSequenceMapper.getNextSequence(paramMap);
            return (String) paramMap.get("nextVal");
        } catch (SQLException e) {
            // 处理异常，记录日志等
            return null;
        }
    }

    @Override
    public SysSequence getSysSequence(final String seqName) {
        try {
            return sysSequenceMapper.find(seqName);
        } catch (SQLException e) {
            // 处理异常，记录日志等
            return null;
        }
    }

    @Override
    public Result.CommonResult updateSysSequence(final SysSequence sysSequence) {
        try {
            int row = sysSequenceMapper.update(sysSequence);
            if (row == 1) {
                return new Result.CommonResult(true, "更新完成");
            }
            return new Result.CommonResult(false, "受影响的行数为0");
        } catch (SQLException e) {
            return new Result.CommonResult(false, "更新失败，出现异常");
        }
    }

    @Override
    public Result.CommonResult addSysSequence(final SysSequence sysSequence) {
        try {
            int row = sysSequenceMapper.add(sysSequence);
            if (row == 1) {
                return new Result.CommonResult(true, "新增完成");
            }
            return new Result.CommonResult(false, "受影响的行数为0");
        } catch (SQLException e) {
            return new Result.CommonResult(false, "新增失败，出现异常");
        }
    }

    @Override
    public Result.CommonResult removeSysSequence(final String seqName) {
        try {
            int row = sysSequenceMapper.remove(seqName);
            if (row == 1) {
                return new Result.CommonResult(true, "删除完成");
            }
            return new Result.CommonResult(false, "受影响的行数为0");
        } catch (SQLException e) {
            return new Result.CommonResult(false, "删除失败，出现异常");
        }
    }
}