package com.gjj.mapper;

import com.gjj.entity.Personal;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * 个人公积金账户信息 Mapper 接口
 */
@Mapper
public interface PersonalMapper {

    /**
     * 新增个人公积金账户信息
     * @param personal 个人账户信息对象
     * @return 影响的行数
     * @throws org.apache.ibatis.exceptions.PersistenceException 数据库操作异常
     */
    int insert(Personal personal);

    /**
     * 根据主键删除个人账户信息
     * @param personalAccNumber 个人公积金账号
     * @return 影响的行数
     * @throws org.apache.ibatis.exceptions.PersistenceException 数据库操作异常
     */
    int deleteByPersonalAccNumber(@Param("personalAccNumber") String personalAccNumber);

    /**
     * 更新个人账户完整信息
     * @param personal 个人账户信息对象
     * @return 影响的行数
     * @throws org.apache.ibatis.exceptions.PersistenceException 数据库操作异常
     */
    int update(Personal personal);

    /**
     * 更新个人账户状态
     * @param personalAccNumber 个人公积金账号
     * @param accountStatus 新的账户状态
     * @return 影响的行数
     * @throws org.apache.ibatis.exceptions.PersistenceException 数据库操作异常
     */
    int updateAccountStatus(
            @Param("personalAccNumber") String personalAccNumber,
            @Param("accountStatus") String accountStatus);

    /**
     * 根据主键查询个人账户信息
     * @param personalAccNumber 个人公积金账号
     * @return 个人账户信息对象，未找到时返回null
     */
    Personal findByPersonalAccNumber(
            @Param("personalAccNumber") String personalAccNumber);

    /**
     * 根据单位账号查询关联的个人账户列表
     * @param unitAccNumber 单位公积金账号
     * @return 个人账户信息列表，未找到时返回空列表
     */
    List<Personal> findByUnitAccNumber(
            @Param("unitAccNumber") String unitAccNumber);
}