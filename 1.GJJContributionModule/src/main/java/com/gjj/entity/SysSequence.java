package com.gjj.entity;

import lombok.Getter;
import lombok.Setter;

/**
 * 系统序列参数表实体类
 */
@Setter
@Getter
public class SysSequence {
    private String seqName;        // 序列名称（对应业务表名）
    private long currentVal;       // 当前序号值
    private long maxVal;           // 最大允许序号
    private String description;    // 序列描述
    private String remark;         // 备注

    public SysSequence() {
    }

    public SysSequence(String seqName, long currentVal, long maxVal, String description, String remark) {
        this.seqName = seqName;
        this.currentVal = currentVal;
        this.maxVal = maxVal;
        this.description = description;
        this.remark = remark;
    }
}