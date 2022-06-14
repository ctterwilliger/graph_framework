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

using data_obj = std::shared_ptr<std::tuple<int,int>>;
using data_t = std::tuple< data_ID, data_obj>;
// TODO: Reference additional headers your program requires here.

bool isValidID(const data_ID & ID) {
    if (ID >= 0)
    {
        return true;
    }
    return false; 
}

size_t ID_castToSizeT(const data_ID& ID)
{
    if (ID < 0)
    {
        return (size_t)-ID;
    }
    return (size_t)ID;
}

size_t getSizeT_ID(const data_t& data)
{
    return ID_castToSizeT(std::get<0>(data));
}

bool isEquivID(data_ID ID1, data_ID ID2)
{
    if (ID1 == ID2 || ID1 == -ID2)
    {
        return true;
    }
    return false; 
}
