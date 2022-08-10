#include "combine_node.h"
#include "dynamic_join_node.h"
#include "type_config.h"
#include <tuple>
#include <math.h>
#include <iostream>
#include <oneapi/tbb.h>



//connects given port to combine
template <typename... T>
void join_and_combine<T...>::connect_to_node(oneapi::tbb::flow::receiver<data_t>& port) {

	make_edge(combine, port);

}

//connects to 
template <typename... T>
void join_and_combine<T...>::connect_to_combine(oneapi::tbb::flow::function_node<data_t, data_t>& node) {

	make_edge(combine, node);
}

//gets a port of join node based on number
template <typename ...T>
oneapi::tbb::flow::receiver<data_t>& join_and_combine<T...>::get_join_port(size_t& num) {
	auto& node = join;
	switch (num)
	{
	case 0:
		if constexpr (sizeof... (T) >= 1) {

			return std::get<0>(node.input_ports());
		}
		break;
	case 1:
		if constexpr (sizeof... (T) >= 2) {

			return std::get<1>(node.input_ports());
		}
		break;
	case 2:

		if constexpr (sizeof... (T) >= 3) {
			return  std::get<2>(node.input_ports());
		}
		break;
	case 3:
		if constexpr (sizeof... (T) >= 4) {
			return std::get<3>(node.input_ports());
		}
		break;
	case 4:
		if constexpr (sizeof... (T) >= 5) {
			return std::get<4>(node.input_ports());
		}
		break;
	case 5:
		if constexpr (sizeof... (T) >= 6) {
			return std::get<5>(node.input_ports());
		}
		break;
	case 6:
		if constexpr (sizeof... (T) >= 7) {
			return  std::get<6>(node.input_ports());
		}
		break;
	case 7:
		if constexpr (sizeof... (T) >= 8) {
			return std::get<7>(node.input_ports());
		}
		break;
	case 8:
		if constexpr (sizeof... (T) >= 9) {
			return std::get<8>(node.input_ports());
		}
		break;
	case 9:
		if constexpr (sizeof... (T) >= 10) {
			return std::get<9>(node.input_ports());
		}
		break;
	default:
		throw 7;
		return std::get<0>(node.input_ports());
	}
	throw 8;
	return std::get<0>(node.input_ports());
}


//makes internal ports
template<typename ...T>
join_and_combine<T...>::join_and_combine(oneapi::tbb::flow::graph& gr) : g(gr)
{
	make_edge(join, combine);
}


