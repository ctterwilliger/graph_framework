#pragma once
#include "oneapi/tbb.h"
#include <vector>
#include "type_config.h"






template <std::size_t I, typename Tuple>
data_t get_entry_for_index(Tuple const& inData)
{
	if constexpr (I == std::tuple_size_v<Tuple>) {
		return std::get<0>(inData);
	}
	else {
		auto& element = std::get<I>(inData);
		if (!isValidID(std::get<0>(element))) {
			return element;
		}
		return get_entry_for_index<I + 1>(inData);
	}
}

template <typename... T>
data_t get_entry(std::tuple<T...> const& inData)
{
	return get_entry_for_index<0>(inData);
}


template<typename T>
auto matcher_for() {
	return [](const data_t& data) {
		return getSizeT_ID(data);
	};
}






template<typename ...T>
oneapi::tbb::flow::join_node<std::tuple<T...>, oneapi::tbb::flow::tag_matching> 
make_join_node(oneapi::tbb::flow::graph& g)
{
	static_assert(std::conjunction_v<std::is_same<T, data_t>...>);

		
	return oneapi::tbb::flow::join_node<std::tuple<T...>, oneapi::tbb::flow::tag_matching>(g, matcher_for<T>()...);
		
}

template<typename ...T>
oneapi::tbb::flow::function_node<std::tuple<T...>, data_t>
make_combine_node(oneapi::tbb::flow::graph& g) {
	static_assert(std::conjunction_v<std::is_same<T, data_t>...>);
	return oneapi::tbb::flow::function_node< std::tuple <T...>, data_t>(g, oneapi::tbb::flow::unlimited,
		[](const auto& inData) {
			data_t output = std::get<0>(inData);
			
			output = get_entry(inData);
			
			return output; 
		});
}



class join_base {
public:
	//join_base();
	virtual ~join_base() = default;
	 oneapi::tbb::flow::receiver<data_t> &get_join(size_t num);
	 oneapi::tbb::flow::sender<data_t> & get_combine();
};





template <typename... T>
class join_and_combine : public join_base {
public:
	join_and_combine( oneapi::tbb::flow::graph& g);
	~join_and_combine();
	oneapi::tbb::flow::receiver<data_t>& get_join(size_t num);
	oneapi::tbb::flow::sender<data_t >& get_combine();
	// some c'tor
	// maybe other stuff...
private:
	oneapi::tbb::flow::graph & g; 
	oneapi::tbb::flow::join_node<std::tuple<T...>, oneapi::tbb::flow::tag_matching> join = make_join_node<T...>(g);
	oneapi::tbb::flow::function_node<std::tuple<T...>, data_t> combine = make_combine_node<T...>(g);
};


template<typename ...T>
join_and_combine<T...>::join_and_combine( oneapi::tbb::flow::graph& g) :g(g)
{
	make_edge(join, combine);
}


template<typename ...T>
join_and_combine<T...>::~join_and_combine() {

}


class JOIN_NODE  {
public:
	JOIN_NODE(size_t numOfJoins,oneapi::tbb::flow::graph & graph);
	~JOIN_NODE();
	oneapi::tbb::flow::receiver<data_t> & nextPort();
	auto EndNode();
private:
	std::vector<std::shared_ptr<join_base>> joins; 
	size_t curPort;
	size_t curNode; 
	const size_t MAXNUMOFJOINS = 10; 
	void add_node();
	void add_node(size_t node_size);
	oneapi::tbb::flow::graph& g; 
};


template<typename ...T>
oneapi::tbb::flow::receiver<data_t>& join_and_combine<T...>::get_join(size_t num) 
{
	auto& node = join;
	auto& port = std::get<0>(node.inputports());
	switch (num)
	{
	case 0:
		port = std::get<0>(node.inputports());
		break;
	case 1:
		port = std::get<1>(node.inputports());
		break;
	case 2:
		port = std::get<2>(node.inputports());
		break;
	case 3:
		port = std::get<3>(node.inputports());
		break;
	case 4:
		port = std::get<4>(node.inputports());
		break;
	case 5:
		port = std::get<5>(node.inputports());
		break;
	case 6:
		port = std::get<6>(node.inputports());
		break;
	case 7:
		port = std::get<7>(node.inputports());
		break;
	case 8:
		port = std::get<8>(node.inputports());
		break;
	case 9:
		port = std::get<9>(node.inputports());
		break;
	default:
		throw 1; // not caught
		break;
	}
	return port;
}

template<typename ...T>
oneapi::tbb::flow::sender<data_t>& join_and_combine<T...>::get_combine() 
{
	return combine;
}


//oneapi::tbb::flow::function_node<std::tuple<data_t, data_t>, data_t>
//make_combine_node(oneapi::tbb::flow::graph& g) {
//	return oneapi::tbb::flow::function_node< std::tuple <data_t, data_t>, data_t>(g, oneapi::tbb::flow::unlimited,
//		[&](const std::tuple<data_t, data_t>& inData) {
//			data_t output;
//			auto& [outID, outData] = output;
//			auto const& [inDataT1, inDataT2] = inData;
//			auto const& [inID1, indata1] = inDataT1;
//			auto const& [inID2, indata2] = inDataT2;
//
//			if (!(isEquivID(inID1, inID2)))
//			{
//				throw 1; 
//			}
// 
//			if (inID1 < inID2)
//			{
//				outID = inID1;
//			}
//			else
//			{
//				outID = inID2; 
//			}
//			outData = indata1; 
//			
//			return output;
//		}); 
//}

//oneapi::tbb::flow::join_node<std::tuple<data_t, data_t>, oneapi::tbb::flow::tag_matching>
//make_join_node(oneapi::tbb::flow::graph& g)
//{
//	return oneapi::tbb::flow::join_node<std::tuple<data_t, data_t>, oneapi::tbb::flow::tag_matching>(g,
//		[](const data_t& data) {
//			return getSizeT_ID(data);
//
//		},
//		[](const data_t& data) {
//			return getSizeT_ID(data);
//		}
//		);
//}
//// repacks ptr tuple(data_sub1) and ptr tuple(tuple_sub2) into prt and tuple(data)
//flow::function_node< tuple<data_t, data_t>, data_pt> join_combine(g, flow::unlimited, [&](const tuple<data_t, data_t>& data) {
//	data_pt output;
//	auto const& [data1, data2] = data;
//	auto const& [ptr1, base1] = data1;
//	auto const& [ptr2, base2] = data2;
//	auto& [ptr, tupoTup] = output;
//	auto& [tup1, tup2] = tupoTup;
//
//
//	//in case pointers are not equivlent we have somesort of error
//	// not handled yet
//	if (ptr1 != ptr2)
//	{
//		throw 1; // can be changed
//	}
//ptr = ptr1;
//tup1 = base1;
//tup2 = base2;
//return output;
//});


//#include "combine_node.cpp"