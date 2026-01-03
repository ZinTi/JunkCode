#ifndef EMPLOYEE_INFO_H
#define EMPLOYEE_INFO_H

#include <string>

class EmployeeInfo {
public:
    EmployeeInfo();
    EmployeeInfo(const std::string& id, const std::string& name,
                 char gender, const std::string& email,
                 const std::string& phone, const std::string& address,
                 const std::string& zip_code, const std::string& group_id,
                 double monthly_working_hours, double monthly_salary);

    // Getter methods
    std::string get_id() const;
    std::string get_name() const;
    char get_gender() const;
    std::string get_email() const;
    std::string get_phone() const;
    std::string get_address() const;
    std::string get_zip_code() const;
    std::string get_group_id() const;
    double get_monthly_working_hours() const;
    double get_monthly_salary() const;

    // Setter methods
    void set_id(const std::string& id);
    void set_name(const std::string& name);
    void set_gender(char gender);
    void set_email(const std::string& email);
    void set_phone(const std::string& phone);
    void set_address(const std::string& address);
    void set_zip_code(const std::string& zip_code);
    void set_group_id(const std::string& group_id);
    void set_monthly_working_hours(double monthly_working_hours);
    void set_monthly_salary(double monthly_salary);

    std::string to_json() const;

private:
    std::string id_;
    std::string name_;
    char gender_;
    std::string email_;
    std::string phone_;
    std::string address_;
    std::string zip_code_;
    std::string group_id_;
    double monthly_working_hours_;
    double monthly_salary_;
};

#endif // EMPLOYEE_INFO_H
