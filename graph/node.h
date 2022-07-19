#pragma once
#include "type_config.h"
#include <functional>



class base_node
{
public:
    virtual ~base_node() = default;
    virtual oneapi::tbb::flow::function_node<data_t, data_t> &  get_node() = 0; 
};

// CURRENT ISSUES 
// cannot handle non-lamda functions
// Cannot have no return
// cannot have no input
// filter node needs a second useless key
// cannot handle multiple outputs
// cannot handle multiple inputs

//requires a return type for accessing outside graph


template <typename R, typename... Arg>
class deducing_node  : public base_node
{
public:
    template <typename FT>
    explicit deducing_node(FT f, std::string const& input_key, std::string const& output_key, oneapi::tbb::flow::graph & g) :
        func_{ f },
        inkey{input_key},
        outkey{output_key},
        node_{ g, oneapi::tbb::flow::unlimited, [&input_key,& output_key, this](data_t const& data) {
            auto const& [id, store] = data;
            if (isValidID(id))
            {
                if constexpr (std::is_same_v<R, void>)
                {
                    store->getData<Arg>(inkey);
                }
                //if constexpr (std::is_same_v<Arg,void>)
                //{
                //    if constexpr (std::is_same_v<R, bool>)
                //    {
                //        if (func_())
                //        {
                //            data_t newOutput = data;
                //            auto& [newid, newstore] = newOutput;
                //            newid = negateID(newid);
                //            return newOutput;

                //        }
                //        else
                //        {

                //        }
                //    }
                //    else
                //    {
                //        //td::cout << "test";
                //        auto output = func_();
                //        store->addData(outkey, output);
                //    }
                //}
                
                    if constexpr (std::is_same_v<R, bool>)
                    {
                        if (func_(store->getData<Arg>(inkey)))
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
                        //td::cout << "test";
                        auto output = func_(store->getData<Arg>(inkey));
                        store->addData(outkey, output);
                    }
            }
            return data;
         } }
    {}
         oneapi::tbb::flow::function_node<data_t, data_t>& get_node() 
         {
             return node_;
         }
private:
    std::function<R(Arg)> func_;
    tbb::flow::function_node<data_t, data_t> node_;
    std::string inkey;
    std::string outkey;
};

//template <typename R, typename Arg>
//class filter_node : public base_node
//{
//public:
//    template <typename FT>
//    explicit filter_node(FT f, std::string const& input_key, std::string const& output_key, oneapi::tbb::flow::graph& g) :
//        func_{ f },
//        inkey{ input_key },
//        outkey{ output_key },
//        node_{ g, oneapi::tbb::flow::unlimited, [&input_key,&output_key, this](data_t const& data) {
//            auto const& [id, store] = data;
//            if constexpr (std::is_same_v<R, void>)
//            {
//                store->getData<Arg>(inkey);
//            }
//            else
//            {
//                //td::cout << "test";
//                 auto output = func_(store->getData<Arg>(inkey));
//                 store->addData(outkey, output);
//             }
//             return data;
//          } }
//    {}
//          oneapi::tbb::flow::function_node<data_t, data_t>& get_node()
//          {
//              return node_;
//          }
//private:
//    std::function<R(Arg)> func_;
//    tbb::flow::function_node<data_t, data_t> node_;
//    std::string inkey;
//    std::string outkey;
//};



template<typename R, typename Arg, typename Func>
R return_type_impl(R(Func::*)(Arg) const);

template<typename R, typename Arg, typename Func>
Arg arg_type_impl(R(Func::*)(Arg) const );


template<typename Func>
using return_type = decltype(return_type_impl(&Func::operator()));

template<typename Func>
using arg_type = decltype(arg_type_impl(&Func::operator()));

template<typename Func>
deducing_node(Func)->deducing_node<return_type<Func>, arg_type<Func>>;


//template<typename Func>
//filter_node(Func)->filter_node<bool, arg_type<Func>>; 