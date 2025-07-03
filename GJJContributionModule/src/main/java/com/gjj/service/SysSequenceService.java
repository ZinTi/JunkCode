package com.gjj.service;

import com.gjj.entity.SysSequence;

public interface SysSequenceService {
    /**
     * 获取下一条序列信息
     * @param seqName 序列名
     * @return 序列信息
     */
    String getNextSequenceValue(final String seqName);

    /**
     * 获取系统序列值
     * @param seqName 序列名
     * @return 序列信息
     */
    SysSequence getSysSequence(final String seqName);

    /**
     * 根据 seqName 更新 sysSequence 信息
     * @param sysSequence 数据
     * @return 结果
     */
    Result.CommonResult updateSysSequence(final SysSequence sysSequence);

    /**
     * 新增 sysSequence 信息
     * @param sysSequence 数据
     * @return 结果
     */
    Result.CommonResult addSysSequence(final SysSequence sysSequence);

    /**
     * 删除 sysSequence 记录
     * @param seqName seqName 序列名
     * @return 结果
     */
    Result.CommonResult removeSysSequence(final String seqName);
}
