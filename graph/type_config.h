#pragma once
#include <vector>
#include <memory>
using data_ID = long;



class base_data {
public:
    virtual ~base_data() = default;
};

template <typename T>
class user_data : public base_data {
public:
    explicit user_data(T const& t) : t_{ t } {}

private:
    T t_;
};


class user_data_store {
public:

    template <typename T>
    void add_data(T const& t)
    {
        data_.push_back(std::make_unique<user_data<T>>(t));
    }

private:
    std::vector<std::unique_ptr<base_data>> data_;
};

using data_obj = std::shared_ptr<user_data_store >;
using data_t = std::tuple< data_ID, data_obj>;
// TODO: Reference additional headers your program requires here.

bool isValidID(data_ID ID) {
    if (ID >= 0)
    {
        return true;
    }
    return false; 
}