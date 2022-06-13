#pragma once
#include <utility>
#include <vector>
#include "type_config.h"
#include <map>
#include <string>

class graph
{
public:

	graph();
	~graph();

	
	void add_filter_node( std::string name, size_t const& concurrency, bool  Func);

	template<typename Func>
	void add_proccess_node( std::string, size_t const& concurrency, typename Func);

	void add_edge(std::string, std::string);

	/*void refresh_graph();
	
	
	

	template<typename Func1, typename Func2>
	void remove_edge(typename Func1, typename Func2);*/
	
	



private:
	bool is_node_in_graph(std::string);

	bool is_edge_in_graph(std::string, std::string);

	//TODO
	//void traverse_graph();
	//void count_predessesors();
	//void create_joins();
	//void add_join_node();
	//void add_combine_node(); 
	
	//std::vector<oneapi::tbb::flow::multifunction_node<data_t, std::tuple<data_t, data_t>>> nodes;
	//std::vector<oneapi::tbb::flow::function_node<data_t, data_t> > nodes; 
	std::map<std::string, std::pair<oneapi::tbb::flow::function_node<data_t, data_t>,int>> user_nodes;
	std::vector < oneapi::tbb::flow::join_node< std::tuple < data_t, data_t>, oneapi::tbb::flow::tag_matching> > join_nodes; 
	std::vector < oneapi::tbb::flow::function_node<std::tuple<data_t, data_t>, data_t> > combine_nodes; 
	std::vector < oneapi::tbb::flow::multifunction_node < data_t,  data_t, std::tuple < data_t, data_t>>> end_graph_nodes; 

	// currently prints
	std::vector < oneapi::tbb::flow::function_node<data_t>> output_nodes;
	std::vector<std::pair<std::string,std::string>> edges; 
	oneapi::tbb::flow::graph g; 
};

#include "graph.cpp" //review