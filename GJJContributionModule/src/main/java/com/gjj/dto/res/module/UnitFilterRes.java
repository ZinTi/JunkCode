package com.gjj.dto.res.module;

import com.gjj.entity.Unit;
import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
public class UnitFilterRes {
    private int totalItems; // 总数据量
    private List<Unit> units;   // 单位列表数据

}
