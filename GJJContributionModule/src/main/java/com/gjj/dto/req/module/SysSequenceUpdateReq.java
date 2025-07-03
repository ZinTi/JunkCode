package com.gjj.dto.req.module;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class SysSequenceUpdateReq {
    private long currentVal;
    private long maxVal;
    private String description;
    private String remark;

    public SysSequenceUpdateReq() {
    }

    public SysSequenceUpdateReq(long currentVal, long maxVal, String description, String remark) {
        this.currentVal = currentVal;
        this.maxVal = maxVal;
        this.description = description;
        this.remark = remark;
    }
}
