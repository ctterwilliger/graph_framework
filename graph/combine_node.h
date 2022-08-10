#pragma once
#include <vector>
#include "oneapi/tbb.h"
#include "type_config.h"
#include <tuple>




// This file is the composite node of a join_node, and function node that checks each input for a flagged data.
// Is used by dynamic join nodes, as the base node. 




template <std::size_t I, typename Tuple>
data_t get_entry_for_index(Tuple const& inData);


template <typename... T>
data_t get_entry(std::tuple<T...> const& inData);

template<typename T>
auto matcher_for();


//base class for join node
class join_base {
public:
	//join_base();
	virtual ~join_base() = default;

	// connects current node and next node
	virtual void connect_to_node(oneapi::tbb::flow::receiver<data_t>& port) = 0;

	//get the join port
	virtual oneapi::tbb::flow::receiver<data_t>& get_join_port(size_t& num) = 0;

	//takes a node and connects it to the combine node
	virtual void connect_to_combine(oneapi::tbb::flow::function_node<data_t, data_t>& node) = 0;

};





template <typename... T>
class join_and_combine : public join_base {
public:
	join_and_combine(oneapi::tbb::flow::graph& gr);

	//connects 
	void connect_to_node(oneapi::tbb::flow::receiver<data_t>& port) override;

	//get the join port based on number
	oneapi::tbb::flow::receiver<data_t>& get_join_port(size_t& num) override;

	//connects combine node to given node
	void connect_to_combine(oneapi::tbb::flow::function_node<data_t, data_t>& node) override;
private:
	oneapi::tbb::flow::graph& g;
	oneapi::tbb::flow::join_node<std::tuple<T...>, oneapi::tbb::flow::tag_matching> join = make_join_node<T...>(g);
	oneapi::tbb::flow::function_node<std::tuple<T...>, data_t> combine = make_combine_node<T...>(g);

};


// internal join node
template<typename ...T>
oneapi::tbb::flow::join_node<std::tuple<T...>, oneapi::tbb::flow::tag_matching> 
make_join_node(oneapi::tbb::flow::graph& g)
{
	//checks to make sure all inputs are data_t
	static_assert(std::conjunction_v<std::is_same<T, data_t>...>);

		
	return oneapi::tbb::flow::join_node<std::tuple<T...>, oneapi::tbb::flow::tag_matching>(g, matcher_for<T>()...);
		
}

// interal function node, called combine node. Takes input of tuple of data_ts, 
//outputs data_t, that is flagged as bad if any of the data_ts are flagged as bad
template<typename ...T>
oneapi::tbb::flow::function_node<std::tuple<T...>, data_t>
make_combine_node(oneapi::tbb::flow::graph& g) {
	static_assert(std::conjunction_v<std::is_same<T, data_t>...>);
	return oneapi::tbb::flow::function_node< std::tuple <T...>, data_t>(g, oneapi::tbb::flow::unlimited,
		[](const auto& inData) {
			auto output = std::get<0>(inData);
			
			output = get_entry(inData);
			//std::cout << (int) std::get<0>(*output) << std::endl;
			return output; 

		});
}








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
