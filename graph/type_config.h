#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <iostream>
using data_ID = int;



class b_data {
public:
    virtual ~b_data() = default;
};

template <typename T>
class user_data : public b_data {
public:
    explicit user_data(T const& t) : t_{ t } {}
    const T& get() const 
    {
        return t_;
    }
private:
    T t_;
};


class user_data_store {
public:

    template <typename T>
    void addData(const std::string& dataID, T const& t)
    {
        if (data_.count(dataID) == 0)
        {
            //std::cout << "emplacing: " << dataID<< std::endl;
            data_.emplace(dataID, std::make_unique<user_data<T>>(t));
        } 
        else
        {
            std::cout << "key already exists" << std::endl;
            throw 7;
        }
    }
    template<typename T>
    const T& getData(const std::string & dataID) const
    {
        auto temp = data_.find(dataID);
        if(temp == data_.end())
        {
            
            std::cout << "key: " << dataID<<" not found";
            throw 7;
        }
        
        //std::cout << dataID;
        auto user_typeptr = dynamic_cast<user_data<T> const *>( temp->second.get());
        if (user_typeptr == nullptr)
        {
            std::cout << "nullptr" << std::endl; 
            throw 8; 
        }
        return user_typeptr->get();
    }
private:
    // maybe going through nodes first and adding all keys to map
    std::map<std::string, std::unique_ptr<b_data>> data_;
};

using base_data = user_data_store; 
using data_obj = std::shared_ptr<base_data>;
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
        return (size_t) (-ID);
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


data_ID negateID(const data_ID& data)
{
    if (isValidID(data))
    {
        return -data;
    }
    return data; 
}



template<typename ...T>
std::tuple<T...> strings_to_tuple(T...)
{
    static_assert(std::conjunction_v<std::is_convertible<T, std::string>...>);
    return std::tuple(T...); 
}

template<typename ...T>
std::tuple<T...> input_keys(T... keys)
{
    static_assert(std::conjunction_v<std::is_convertible<T, std::string>...>);
    return std::tuple<T...>(keys...);
}

template<typename ...T>
std::tuple<T...> output_keys(T... keys)
{
    static_assert(std::conjunction_v<std::is_convertible<T, std::string>...>);
    return std::tuple<T...>(keys...);
}