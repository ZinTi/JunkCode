#ifndef ADMIN_INFO_H
#define ADMIN_INFO_H

#include <string>

class AdminInfo {
public:
    AdminInfo();
    AdminInfo(const std::string& id, const std::string& name,
              const std::string& salt, const std::string& pwd);

    // Getter methods
    std::string get_id() const;
    std::string get_name() const;
    std::string get_salt() const;
    std::string get_pwd() const;

    // Setter methods
    void set_id(const std::string& id);
    void set_name(const std::string& name);
    void set_salt(const std::string& salt);
    void set_pwd(const std::string& pwd);

    // JSON格式化方法
    std::string to_json() const;

private:
    std::string id_;
    std::string name_;
    std::string salt_;
    std::string pwd_;
};

#endif // ADMIN_INFO_H
