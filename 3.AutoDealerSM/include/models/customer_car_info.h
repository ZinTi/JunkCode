#ifndef CUSTOMER_CAR_INFO_H
#define CUSTOMER_CAR_INFO_H

#include <string>

class CustomerCarInfo {
public:
    CustomerCarInfo();
    CustomerCarInfo(const std::string& id, const std::string& condition,
                    const std::string& color, const std::string& cust_name,
                    const std::string& model);

    // Getter methods
    std::string get_id() const;
    std::string get_condition() const;
    std::string get_color() const;
    std::string get_cust_name() const;
    std::string get_model() const;

    // Setter methods
    void set_id(const std::string& id);
    void set_condition(const std::string& condition);
    void set_color(const std::string& color);
    void set_cust_name(const std::string& cust_name);
    void set_model(const std::string& model);

    std::string to_json() const;

private:
    std::string id_;
    std::string condition_;
    std::string color_;
    std::string cust_name_;
    std::string model_;
};

#endif // CUSTOMER_CAR_INFO_H
