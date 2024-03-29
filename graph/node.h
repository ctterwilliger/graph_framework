#pragma once
#include "type_config.h"
#include <functional>


//base node
class base_node
{
public:
    virtual ~base_node() = default;
    virtual oneapi::tbb::flow::function_node<data_t, data_t> &  get_node() = 0; 
};

// CURRENT ISSUES 
// requires a return type for accessing outside graph

using string_for_t = std::string;


// The class for the deducing node
// Deduces what kind of node it is based on output etc, then constructs it
template <typename R, typename... Args>
class deducing_node  : public base_node
{

    //runs the internal function
    template <std::size_t... Is, typename InputKeys>
    auto unpack(const data_obj & store, InputKeys input_keys, std::index_sequence<Is...>)
    {
        return func_(store->getData<Args>(std::get<Is>(input_keys))...);
    }
    
    //stores a output to data store
    template <std::size_t... Is, typename OutputKeys>
    void storepack(const data_obj& store, const R& output, OutputKeys output_keys, std::index_sequence<Is...>)
    {
        (store->addData(std::get<Is>(output_keys), std::get<Is>(output)),...);
    }

public:

    //constuctor with input and output
    template <typename FT, typename ...InKeys, typename ...OutKeys>
    explicit deducing_node(FT f, std::tuple<OutKeys...> output_keys, oneapi::tbb::flow::graph & g, std::tuple<InKeys...> input_keys) :
        func_{ f },
        node_{ g, oneapi::tbb::flow::unlimited, [output_keys, input_keys, this](data_t const& data) {
            static_assert(std::conjunction_v<std::is_convertible<InKeys, std::string>...>);
            auto const& [id, store] = data;
            if (isValidID(id))
            {
                
                if constexpr (std::is_same_v<R, void>)
                {
                    //store->getData<Args>(inkeys)...;
                }
                else
                {
                    auto output = unpack(store, input_keys, std::index_sequence_for<Args...>());
                    if constexpr (is_tuple_v<R>)
                    {
                        static_assert(tuple_size_v<R> == sizeof ...(OutKeys));
                        storepack(store, output, output_keys, std::index_sequence_for<OutKeys...>());


                    }
                    else
                    {
                        store->addData(std::get<0>(output_keys), output);
                    }
                }
                   
                       
                      
                       
                    
            }
            return data;
         } }
    {}

         // constructor with only input
         template <typename FT, typename ...InKeys>
         explicit deducing_node(FT f,  oneapi::tbb::flow::graph& g, std::tuple<InKeys...> input_keys) :
             func_{ f },
             node_{ g, oneapi::tbb::flow::unlimited, [input_keys, this](data_t const& data) {
                 static_assert(std::conjunction_v<std::is_convertible<InKeys, std::string>...>);
                 auto const& [id, store] = data;
                 
                 if (isValidID(id))
                 {
                     if constexpr (std::is_same_v<R,void>)
                     {
                        // unpack(store, input_keys, std::index_sequence_for<Args...>());
                   
                     }
                     else if constexpr (std::is_same_v<R, bool>)
                     {
                         if (unpack(store,input_keys, std::index_sequence_for<Args...>()))
                         {
                             
                              data_t newOutput = data;
                              auto& [newid, newstore] = newOutput;
                              newid = negateID(newid);
                              return newOutput;

                         }
                         else
                         {
                             return data;
                         }
                      }
                      else
                      {

                         auto output = unpack(store, input_keys, std::index_sequence_for<Args...>());
                      }
                 }
                 return data;
              } }
         {}
         oneapi::tbb::flow::function_node<data_t, data_t>& get_node() 
         {
             return node_;
         }

         // contructor with no input or output
         template <typename FT>
         explicit deducing_node(FT f, oneapi::tbb::flow::graph &g) :
             node_{ g, oneapi::tbb::flow::unlimited, [this,f ](data_t const& data) {
                 auto const& [id, store] = data;
                  FT func2_= f;
                 if (isValidID(id))
                 {
                     if constexpr (std::is_same_v<R, bool>)
                     {
                         if (func2_())
                         {
                             data_t newOutput = data;
                             auto& [newid, newstore] = newOutput;
                             newid = negateID(id);
                             return newOutput;

                         }
                         else
                         {
                             return data;
                         }
                     }
                     else
                     {
                         func2_();
                     }
                     
                 }
                 return data;
              } }
         {}
              explicit deducing_node(oneapi::tbb::flow::graph& g) :
                  node_{ g, oneapi::tbb::flow::unlimited, [this](data_t const& data) { 
                      return data;
                   } }
              {}

            


private:
    std::function<R(Args...)> func_;
    tbb::flow::function_node<data_t, data_t> node_;
    

    
    // DEDUCTION GUIDE DOWN FROM HERE ON DOWN(AKA C++ MAGIC)
    template<typename T>
    struct is_tuple_impl : std::false_type
    {

    };

    template<typename ...T>
    struct is_tuple_impl<std::tuple<T...>> : std::true_type
    {

    };

    template<typename T>
    static constexpr bool is_tuple_v = is_tuple_impl<T>::value;
};



template < typename R, typename... Args>
class deducing_node<R, std::tuple<Args...>> : public deducing_node<R, Args...>
{
public:
    using deducing_node<R, Args...>::deducing_node;
};

template<typename R, typename... Args, typename Func>
R return_type_impl(R(Func::*)(Args...) const);

template<typename R, typename... Args, typename Func>
std::tuple<Args...> arg_type_impl(R(Func::*)(Args...) const);

template<typename Func>
struct return_type_struct 
{
    using type = decltype(return_type_impl(&Func::operator()));
};

template<typename R, typename ...Args>
struct return_type_struct<R(*)(Args...)>
{
    using type = R;
};

template<typename Func>
struct arg_type_struct
{
    using type = decltype(arg_type_impl(&Func::operator()));
};

template<typename R, typename ...Args>
struct arg_type_struct<R(*)(Args...)>
{
    using type = std::tuple<Args...>;
};

template<typename Func>
using return_type = typename return_type_struct<Func>::type; 

template<typename Func>
using arg_types = typename arg_type_struct<Func>::type;

template<typename Func>
deducing_node(Func)->deducing_node<return_type<Func>, arg_types<Func>>;


//template<typename Func>
//filter_node(Func)->filter_node<bool, arg_type<Func>>; 