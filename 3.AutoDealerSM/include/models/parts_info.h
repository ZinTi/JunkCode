#ifndef PARTS_INFO_H
#define PARTS_INFO_H

#include <string>

class PartsInfo {
public:
    PartsInfo();
    PartsInfo(const std::string& id, const std::string& model,
              const std::string& name, int number,
              const std::string& classification, double price,
              const std::string& remarks);

    // Getter methods
    std::string get_id() const;
    std::string get_model() const;
    std::string get_name() const;
    int get_number() const;
    std::string get_classification() const;
    double get_price() const;
    std::string get_remarks() const;

    // Setter methods
    void set_id(const std::string& id);
    void set_model(const std::string& model);
    void set_name(const std::string& name);
    void set_number(int number);
    void set_classification(const std::string& classification);
    void set_price(double price);
    void set_remarks(const std::string& remarks);

    std::string to_json() const;

private:
    std::string id_;
    std::string model_;
    std::string name_;
    int number_;
    std::string classification_;
    double price_;
    std::string remarks_;
};

#endif // PARTS_INFO_H
