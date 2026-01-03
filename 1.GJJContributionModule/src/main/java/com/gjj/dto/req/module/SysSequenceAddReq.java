package com.gjj.dto.req.module;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class SysSequenceAddReq {
    private String seqName;
    private long currentVal;
    private long maxVal;
    private String description;
    private String remark;

    public SysSequenceAddReq() {
    }

    public SysSequenceAddReq(String seqName, long currentVal, long maxVal, String description, String remark) {
        this.seqName = seqName;
        this.currentVal = currentVal;
        this.maxVal = maxVal;
        this.description = description;
        this.remark = remark;
    }
}
