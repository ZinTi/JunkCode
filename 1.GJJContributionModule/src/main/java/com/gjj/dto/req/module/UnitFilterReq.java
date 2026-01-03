package com.gjj.dto.req.module;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class UnitFilterReq {
    private String unitName;

    public UnitFilterReq() {
    }

    public UnitFilterReq(String unitName) {
        this.unitName = unitName;
    }
}
