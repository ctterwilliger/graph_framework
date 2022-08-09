#pragma once
#include <vector>
#include "oneapi/tbb.h"
#include "type_config.h"
#include <tuple>





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
			auto output = std::get<0>(inData);
			
			output = get_entry(inData);
			//std::cout << (int) std::get<0>(*output) << std::endl;
			return output; 

		});
}



class join_base {
public:
	//join_base();
	virtual ~join_base() = default;

	// connects current node and next node
	virtual void connect_to_node(oneapi::tbb::flow::receiver<data_t> & port) = 0;
	
	//get the join port
	virtual oneapi::tbb::flow::receiver<data_t> & get_join_port(size_t & num)= 0;


	virtual void connect_to_combine(oneapi::tbb::flow::function_node<data_t,data_t> & node) = 0;


	/*virtual oneapi::tbb::flow::receiver<data_t>& get_join_inport(size_t num) = 0; 
	virtual oneapi::tbb::flow::sender<data_t>& get_join_outport() = 0;
	virtual oneapi::tbb::flow::receiver<data_t>& get_combine_inport() = 0;
	virtual oneapi::tbb::flow::sender<data_t>& get_combine_outport() =0;*/
};





template <typename... T>
class join_and_combine : public join_base {
public:
	join_and_combine(oneapi::tbb::flow::graph& gr);

	void connect_to_node(oneapi::tbb::flow::receiver<data_t> & port) override;

	//get the join port
	oneapi::tbb::flow::receiver<data_t> & get_join_port(size_t & num) override;


	 void connect_to_combine(oneapi::tbb::flow::function_node<data_t, data_t>& node) override;



	//oneapi::tbb::flow::receiver<data_t>& get_join_inport(size_t num) override;
	//oneapi::tbb::flow::receiver<data_t >& get_combine_inport() override;
	//oneapi::tbb::flow::sender<data_t> & get_combine_outport() override;
	//oneapi::tbb::flow::sender<data_t>& get_join_outport() override;

	// some c'tor
	// maybe other stuff...
private:
	oneapi::tbb::flow::graph& g;
	oneapi::tbb::flow::join_node<std::tuple<T...>, oneapi::tbb::flow::tag_matching> join = make_join_node<T...>(g);
	oneapi::tbb::flow::function_node<std::tuple<T...>, data_t> combine = make_combine_node<T...>(g);
	
};




