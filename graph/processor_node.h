#pragma once
#include "oneapi/tbb.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include "type_config.h"
#include <functional>












//
//
//template <typename R, typename Arg>
//class deducing_node {
//public:
//    template <typename FT>
//    explicit deducing_node(FT f, std::string const& input_key, std::string const& output_key) :
//        func_{ f },
//        node_{ g, flow::unlimited, [](data_t const& data) {
//          auto const& [id, store] = data;
//          if constexpr (std::is_same_v<R, void>) {
//            store->getData<Arg>(input_key);
//          }
//     else {
//    auto output = func_(store->getData<Arg>(input_key));
//    store->addData(output_key, output);
//  }
//  return data;
//} }
//    {}
//
//private:
//    std::function<R(Args)> func_;
//    tbb::flow::function_node<data_t, data_t> node_;
//};


// tbb::flow::function_node<data_t, data_t> processor{g, flow::unlimited,                                                                                                              
//   [](data_t const& data) {                                                                                                                                                          
//     auto const& [id, store] = data;                                                                                                                                                 
//     auto output = round_down(store->getData<double>(key));                                                                                                                          
//     store->addData(other_key, output);                                                                                                                                              
//     /// ...                                                                                                                                                                         
//     return data;                                                                                                                                                                    
//   });                                                                                                                                                                               
//
//
//int round_down(double const d)
//// Input: user_data<double> (through the product store)                                                                                                                                
//// Output: user_data<int> (through the product store)                                                                                                                                  
//{
//    return static_cast<int>(d);
//}



//
//template<typename Func> 
//base_node* make_process_node(oneapi::tbb::flow::graph& g, size_t  concurrency, std::string const& input_key, std::string const& output_key, Func  f) {
//	return deducing_node<int, double>(f, "temp", "temp2", g);
//}

//
//template<typename Func>
//oneapi::tbb::flow::function_node<data_t, data_t>
//make_process_node(oneapi::tbb::flow::graph& g, size_t  concurrency, Func  f) {
//	return oneapi::tbb::flow::function_node<data_t, data_t>(g, concurrency, [&](const data_t& data)
//		{
//			auto const& [ID, data_ptr] = data;
//
//			if (isValidID(ID))
//			{
//				f(data);
//			}
//			return data;
//		}
//	);
//}

