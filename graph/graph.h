#pragma once
#include <utility>
#include <vector>
#include <map>
#include <string>
#include "type_config.h"
#include "start_node.h"
#include "combine_node.h"
#include "node.h"
#include "dynamic_join_node.h"


using data_nodeID = std::string; 
class graph
{
public:

	graph();
	~graph();
	
	//Activates the start Node
	void run_graph(); 


	//Waits until the graph is finished
	void wait_graph();





	// adds a process node to user_nodes
	// Many different definations to support many differnt types of nodes
	template<typename Func>
	void add_process_node(const data_nodeID & nodeID,  std::string const& input_key, std::string const& output_key, Func f );

	template<typename Func, typename ...Inputs>
	void add_process_node(const data_nodeID& nodeID, std::tuple<Inputs...> input_keys, std::string const& output_key, Func f);

	template<typename Func, typename ...Inputs>
	void add_process_node(const data_nodeID& nodeID, std::tuple<Inputs...> input_keys, Func f);

	template<typename Func>
	void add_process_node(const data_nodeID& nodeID, std::string const& output_key, Func f);

	template<typename Func>
	void add_process_node(const data_nodeID& nodeID, Func f);

	void add_process_node(const data_nodeID& nodeID);

	template<typename Func, typename ...Inputs, typename ...Outputs>
	void add_process_node(const data_nodeID& nodeID, std::tuple<Inputs...> input_keys, std::tuple<Outputs...> output_keys, Func f);


	// Allows the user to add an edge to the graphs set of edges(DOES NOT LINK THE EDGES)
	void add_edge( const data_nodeID  nodeID1,const data_nodeID nodeID2);


	//Builds the graph given the users given information
	void build_graph();
	

	//printer functions, curretly for bugg testing 
	void print_nodes();
	void print_edges();
	void print_EoG_nodes(); 
	
	//void tempV(std::vector<data_obj>);
	void add_start_node(const data_nodeID, std::vector<data_obj> F); 


	void draw_frame_graph(std::string file);
	void draw_user_graph(std::string file);
	void get_trash(std::vector<std::vector<data_obj>>& V);
	void get_output(std::vector<std::vector<data_obj>>& V);
	
	


	
	



private:
	oneapi::tbb::flow::graph g;
	bool is_node_in_graph(const data_nodeID & nodeID);

	bool is_edge_in_graph(const data_nodeID & nodeID1, const data_nodeID & nodeID2);

	void count_predecessors();

	// creates a join node to proceed a node
	void create_join( data_nodeID  nodeID,const size_t & JOINS);

	

	//connects all the user_nodes
	void connect_nodes();

	//creates a join_node to precceed each user_node that needs one and addes it to join_nodes map
	void create_join_nodes(); 

	// find the end of the graph to place EoG nodes
	void find_EoG();

	// finds the end of graph given a node
	void find_end(const data_nodeID & nodeID); 

	void connect_to_join(const data_nodeID& nodeID1, const data_nodeID& nodeID2, size_t JOINS);
	
	
	
	//creates a EoG_node for a given node
	void create_EoG_node(const data_nodeID & node);  
	
	


	
	
	
	
	
	
	
	// Holds join node
	std::map<data_nodeID, std::shared_ptr<JOIN_NODE>>joins; 
		
	// hold all the assocated nodes
	// Map(name, pair(user node, numPredessors))
	std::map<data_nodeID, 
		std::pair<std::shared_ptr<base_node>,size_t>> user_nodes;

	// MaP(nameOfNodetoPrecess, tuple<join_node, combine_node, numOfCurConnectedNode))
	// Map(nameOfNodetoProceed, EoGNode), 
	std::map < data_nodeID, 
		oneapi::tbb::flow::multifunction_node <data_t, std::tuple < data_t, data_t>>> end_graph_nodes;


	//std::map<data_nodeID, oneapi::tbb::flow::function_node< data_t>> output_nodes;
	//std::map<data_nodeID, oneapi::tbb::flow::function_node< data_t>> trash_nodes;

	std::vector<std::pair<data_nodeID,data_nodeID>> edges; 

	data_nodeID firstNode; 
	std::vector<data_obj> inputs; 
	std::vector<oneapi::tbb::flow::input_node<data_t>> start_node;
	oneapi::tbb::concurrent_vector < oneapi::tbb::concurrent_vector<data_obj>> valid_outputs;
	oneapi::tbb::concurrent_vector <oneapi::tbb::concurrent_vector<data_obj>> trash_outputs;
	std::vector<data_nodeID> EoGs; 
	
	user_data_store data_template; 
	
};

#include "graph.cpp" //review