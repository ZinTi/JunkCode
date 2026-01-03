package com.gjj.mapper;

import com.gjj.entity.Unit;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * 单位基本信息 Mapper 接口
 */
@Mapper
public interface UnitMapper {

    /**
     * 新增单位信息
     * @param unit 单位信息实体
     * @return 影响的行数
     * @throws org.apache.ibatis.exceptions.PersistenceException 数据库操作异常
     */
    int insert(Unit unit);

    /**
     * 根据主键删除单位信息
     * @param unitAccNumber 单位账号
     * @return 影响的行数
     * @throws org.apache.ibatis.exceptions.PersistenceException 数据库操作异常
     */
    int deleteByUnitAccNumber(@Param("unitAccNumber") String unitAccNumber);

    /**
     * 更新单位信息（不更新主键）
     * @param unit 单位信息实体
     * @return 影响的行数
     * @throws org.apache.ibatis.exceptions.PersistenceException 数据库操作异常
     */
    int update(Unit unit);

    /**
     * 更新单位账号状态
     * @param unitAccNumber 单位账号
     * @param accountStatus 账号状态
     * @return 影响的行数
     * @throws org.apache.ibatis.exceptions.PersistenceException 数据库操作异常
     */
    int updateAccountStatus(
            @Param("unitAccNumber") String unitAccNumber,
            @Param("accountStatus") String accountStatus
    );

    /**
     * 根据主键查询单位信息
     * @param unitAccNumber 单位账号
     * @return 单位信息实体
     */
    Unit findByUnitAccNumber(@Param("unitAccNumber") String unitAccNumber);

    /**
     * 根据单位名称模糊查询
     * @param unitName 单位名称
     * @return 单位信息列表
     */
    List<Unit> findByUnitName(@Param("unitName") String unitName);
}