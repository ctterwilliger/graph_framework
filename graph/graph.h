#pragma once
#include <utility>
#include <vector>
#include "type_config.h"
#include <map>
#include <string>
using data_nodeID = std::string; 
class graph
{
public:

	graph();
	~graph();
	void run_graph(); 
	// adds a filter node to user_nodes
	void add_filter_node( data_nodeID name, size_t const& concurrency, bool  Func);

	// adds a proccess node to user_nodes
	template<typename Func>
	void add_proccess_node(data_nodeID name, size_t const& concurrency,  Func f );

	// adds an edge between two nodes in graph
	void add_edge(data_nodeID node1, data_nodeID node2);

	void refresh_graph();
	
	void print_nodes();
	void print_edges();
	

	/*template<typename Func1, typename Func2>
	void remove_edge(typename Func1, typename Func2);*/
	
	



private:
	bool is_node_in_graph(data_nodeID node);

	bool is_edge_in_graph(data_nodeID node1, data_nodeID node2);

	void count_predecessors();

	// creates a join node to proceed a node
	void create_join(data_nodeID node);

	// returns a join_node
	auto add_join_node();

	//returns a combine_node
	auto add_combine_node();

	//connects all the user_nodes
	void connect_nodes();

	//creates a join_node to precceed each user_node that needs one and addes it to join_nodes map
	void create_join_nodes(); 

	// find the end of the graph to place EoG nodes
	void find_EoG();

	// finds the end of graph given a node
	void find_end(data_nodeID); 



	void find_start_node(); 
	
	//creates a EoG_node for a given node
	void create_EoG_node(data_nodeID node); 


	
	
	// hold all the assocated nodes
	// Map(name, pair(user node, numPredessors))
	
	std::map<data_nodeID, 
		std::pair<oneapi::tbb::flow::function_node<data_t, data_t>,size_t>> user_nodes;

	// MaP(nameOfNodetoPrecess, tuple<join_node, combine_node, numOfCurConnectedNode))
	std::map <data_nodeID, 
		std::tuple<oneapi::tbb::flow::join_node< std::tuple < data_t, data_t>, oneapi::tbb::flow::tag_matching>, 
		 oneapi::tbb::flow::function_node<std::tuple<data_t, data_t>, data_t> ,
		size_t >> join_nodes;

	// Map(nameOfNodetoProceed, EoGNode), 
	std::map < data_nodeID, 
		oneapi::tbb::flow::multifunction_node < data_t, data_t, std::tuple < data_t, data_t>>> end_graph_nodes;
	

	// currently prints
	std::vector < oneapi::tbb::flow::function_node<data_t>> output_nodes;
	std::vector<std::pair<data_nodeID,data_nodeID>> edges; 
	oneapi::tbb::flow::graph g; 
};

#include "graph.cpp" //review