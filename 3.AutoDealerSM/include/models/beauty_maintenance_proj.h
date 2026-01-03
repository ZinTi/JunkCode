#ifndef BEAUTY_MAINTENANCE_PROJ_H
#define BEAUTY_MAINTENANCE_PROJ_H

#include <string>

class BeautyMaintenanceProj {
public:
    BeautyMaintenanceProj();
    BeautyMaintenanceProj(const std::string& id, const std::string& proj_name,
                          double price, const std::string& group_id);

    // Getter methods
    std::string get_id() const;
    std::string get_proj_name() const;
    double get_price() const;
    std::string get_group_id() const;

    // Setter methods
    void set_id(const std::string& id);
    void set_proj_name(const std::string& proj_name);
    void set_price(double price);
    void set_group_id(const std::string& group_id);

    std::string to_json() const;

private:
    std::string id_;
    std::string proj_name_;
    double price_;
    std::string group_id_;
};

#endif // BEAUTY_MAINTENANCE_PROJ_H
