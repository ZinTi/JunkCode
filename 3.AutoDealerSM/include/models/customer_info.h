#ifndef CUSTOMER_INFO_H
#define CUSTOMER_INFO_H

#include <string>

class CustomerInfo {
public:
    CustomerInfo();
    CustomerInfo(const std::string& id, const std::string& name, const std::string& salt, const std::string& pwd, char gender, const std::string& email, 
        const std::string& phone, const std::string& address, const std::string& zip_code, const std::string& car_model, const std::string& car_condition, const std::string& car_id);

    // Getter methods
    std::string get_id() const;
    std::string get_name() const;
    std::string get_salt() const;
    std::string get_pwd() const;
    char get_gender() const;
    std::string get_email() const;
    std::string get_phone() const;
    std::string get_address() const;
    std::string get_zip_code() const;
    std::string get_car_model() const;
    std::string get_car_condition() const;
    std::string get_car_id() const;

    // Setter methods
    void set_id(const std::string& id);
    void set_name(const std::string& name);
    void set_salt(const std::string& salt);
    void set_pwd(const std::string& pwd);
    void set_gender(char gender);
    void set_email(const std::string& email);
    void set_phone(const std::string& phone);
    void set_address(const std::string& address);
    void set_zip_code(const std::string& zip_code);
    void set_car_model(const std::string& car_model);
    void set_car_condition(const std::string& car_condition);
    void set_car_id(const std::string& car_id);

    std::string to_json() const;

private:
    std::string id_;
    std::string name_;
    std::string salt_;
    std::string pwd_;
    char gender_;
    std::string email_;
    std::string phone_;
    std::string address_;
    std::string zip_code_;
    std::string car_model_;
    std::string car_condition_;
    std::string car_id_;

};

#endif // CUSTOMER_INFO_H
