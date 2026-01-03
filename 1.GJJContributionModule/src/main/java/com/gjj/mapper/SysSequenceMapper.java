package com.gjj.mapper;

import com.gjj.entity.SysSequence;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.sql.SQLException;
import java.util.Map;

/**
 * 系统序列号Mapper接口
 */
@Mapper
public interface SysSequenceMapper {

    /**
     * 调用存储过程获取下一个序列值（存储过程中会更新序列值）
     * @param paramMap 参数映射（包含seqName输入参数和nextVal输出参数）
     * @throws SQLException SQL异常
     */
    void getNextSequence(Map<String, Object> paramMap) throws SQLException;

    /**
     * 根据主键 seqName 查询序列信息（这个方法查询后不会修改 current_val 值）
     * @param seqName 查询目标序列名（主键）
     * @return SysSequence 序列对象
     * @throws SQLException SQL异常
     */
    SysSequence find(@Param("seqName") String seqName) throws SQLException;

    /**
     * 根据 seqName 更新数据
     * @param newData 新数据对象
     * @return 受影响的行数
     * @throws SQLException SQL异常
     */
    int update(SysSequence newData) throws SQLException;

    /**
     * 新增序列数据
     * @param newData 新数据对象
     * @return 受影响的行数
     * @throws SQLException SQL异常
     */
    int add(SysSequence newData) throws SQLException;

    /**
     * 根据 seqName 删除序列记录
     * @param seqName 序列名
     * @return 受影响的行数
     * @throws SQLException SQL异常
     */
    int remove(@Param("seqName") String seqName) throws SQLException;
}