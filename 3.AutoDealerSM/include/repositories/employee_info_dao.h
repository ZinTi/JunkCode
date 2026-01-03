/**
 * @file employee_info_dao.h
 * @brief 职员信息数据访问对象
 * @date 2025
 */

#ifndef EMPLOYEE_INFO_DAO_H
#define EMPLOYEE_INFO_DAO_H

#include "models/employee_info.h"
#include "repositories/base_dao.h"
#include <vector>

/**
 * @class EmployeeInfoDAO
 * @brief 职员信息数据访问对象类
 *
 * 提供职员信息的CRUD操作接口
 */
class EmployeeInfoDAO : public BaseDAO {
public:
    /**
     * @brief 插入职员信息
     * @param employee 职员信息对象
     * @return bool 插入是否成功
     */
    bool insert(const EmployeeInfo& employee);

    /**
     * @brief 根据ID删除职员信息
     * @param id 职员ID
     * @return bool 删除是否成功
     */
    bool remove(const std::string& id);

    /**
     * @brief 更新职员信息
     * @param employee 职员信息对象
     * @return bool 更新是否成功
     */
    bool update(const EmployeeInfo& employee);

    /**
     * @brief 根据ID查询职员信息
     * @param id 职员ID
     * @return EmployeeInfo 职员信息对象
     */
    EmployeeInfo find_by_id(const std::string& id);

    /**
     * @brief 查询所有职员信息
     * @return std::vector<EmployeeInfo> 职员信息列表
     */
    std::vector<EmployeeInfo> find_all();

    /**
     * @brief 根据姓名查询职员信息
     * @param name 职员姓名
     * @return std::vector<EmployeeInfo> 职员信息列表
     */
    std::vector<EmployeeInfo> find_by_name(const std::string& name);

    /**
     * @brief 根据性别查询职员信息
     * @param gender 职员性别
     * @return std::vector<EmployeeInfo> 职员信息列表
     */
    std::vector<EmployeeInfo> find_by_gender(char gender);

    /**
     * @brief 根据班组ID查询职员信息
     * @param group_id 班组ID
     * @return std::vector<EmployeeInfo> 职员信息列表
     */
    std::vector<EmployeeInfo> find_by_group_id(const std::string& group_id);

    /**
     * @brief 根据工资范围查询职员信息
     * @param min_salary 最低工资
     * @param max_salary 最高工资
     * @return std::vector<EmployeeInfo> 职员信息列表
     */
    std::vector<EmployeeInfo> find_by_salary_range(double min_salary, double max_salary);

    /**
     * @brief 根据工时范围查询职员信息
     * @param min_hours 最低工时
     * @param max_hours 最高工时
     * @return std::vector<EmployeeInfo> 职员信息列表
     */
    std::vector<EmployeeInfo> find_by_hours_range(double min_hours, double max_hours);

    /**
     * @brief 根据邮箱查询职员信息
     * @param email 邮箱地址
     * @return std::vector<EmployeeInfo> 职员信息列表
     */
    std::vector<EmployeeInfo> find_by_email(const std::string& email);

    /**
     * @brief 根据电话号码查询职员信息
     * @param phone 电话号码
     * @return std::vector<EmployeeInfo> 职员信息列表
     */
    std::vector<EmployeeInfo> find_by_phone(const std::string& phone);
};

#endif // EMPLOYEE_INFO_DAO_H
