#include "graph.h"
#include "splitter_node.h"
#include "filter_node.h"
#include <iostream>
#include "type_config.h"
#include "combine_node.h"
#include "graph_end_node.h"


// default constructor(currently no arguments)
graph::graph() {

}



//defualt deconstructor
graph::~graph() {

}



// runs the graph by activating the start node
void graph::run_graph() {
	start_node.activate();
}



// Allows the user to create a filter node that is added to user_nodes
template<typename FT>
void graph::add_filter_node(const data_nodeID  & nodeID, size_t const& concurrency, FT  Func)
{
	if ((is_node_in_graph(nodeID)))
	{
		throw 1; /// IS NOT CAUGHT YET
	}
	else
	{
		user_nodes.emplace(nodeID, std::make_pair(make_filter_node(g, concurrency, Func), 0));
	}
}



// Allow the user to creat a proccess node that is added to user_nodes
template<typename Func>
void graph::add_proccess_node(const data_nodeID & nodeID, size_t const& concurrency, Func f)
{
	if ((is_node_in_graph(nodeID)))
	{
		throw 1; /// IS NOT CAUGHT YET
	}
	else
	{
		user_nodes.emplace(nodeID, std::make_pair(make_proccess_node(g, concurrency, f), 0));
	}
}



// Allows the user to add an edge to the graphs set of edges(DOES NOT LINK THE EDGES)
void graph::add_edge( const  data_nodeID  nodeID1, const data_nodeID  nodeID2) //THIS IS WHERE THE ISSUE IS CANNOT LINK BY REFERANCE
{
	if (is_edge_in_graph(nodeID1, nodeID2))
	{
		throw 1; // NOT HANDLED YET
	}
	else if (!(is_node_in_graph(nodeID1)))
	{
		throw 2; //NOT HANDLED YET
	}
	else if (!(is_node_in_graph(nodeID2)))
	{
		throw 3; //NOT HANDLED YET
	}
	else
	{
		edges.push_back(std::make_pair(nodeID1, nodeID2));
	}
}



// Builds the graph given the user input
// currently has N^3(logN) run time due to find_EoG
void graph::build_graph() {
	//TODO clear_data();

	count_predecessors();


	create_join_nodes();
	
	// iterate through edges connects all nodes 
	connect_nodes(); 

	//finds and creates EoG nodes
	find_EoG();



	//TODO GIVE a PROPER START NODE
	//find_start_node(); 

	
	make_edge(start_node, (user_nodes.at("start")).first) ;
}



// counts the number of DIRECT predesessors a given node has
void graph::count_predecessors()
{
	// sets all to 0 then counts if it is in edges as the second term
	for (auto& n : user_nodes)
	{
		auto & [nodeID, pair] = n;
		auto & [node, num] = pair; 
	
		num = 0;
		for (const auto& m : edges)
		{
			if (m.second == n.first)
			{
				num++;
			}
		}
	}
}



//creates a join node to preceed every node that has two or more Direct predessesors
void graph::create_join_nodes() {
	for (const auto& n : user_nodes)
	{
		const auto& [nodeID, pair] = n;
		const auto& [node, num] = pair;

		if (num >= 2)
		{
			create_join(nodeID,num);
		}
	}
}



//connects two nodes, changes to connect to join if needed
void graph::connect_nodes()
{
	for (const auto& n : edges)
	{
		auto& [nodeID1, nodeID2] = n;
		auto& [node2, num2] = user_nodes.at(nodeID2);
		auto& [node1, num1] = user_nodes.at(nodeID1);

		//std::cout << num2 << std::endl; 
		if (num2 == 0) {

			// donothing

		}
		else if (num2 == 1)
		{
			oneapi::tbb::flow::make_edge(node1, node2);
		}
		else
		{
			connect_to_join(nodeID1, nodeID2, num2);
		}
	}
}



//finds the end of graph for EVERYNODE (can be more than one), then places a node
void graph::find_EoG()
{
	// VERY SLOW n^3 logn runtime 
	for (auto& n : user_nodes)
	{
		auto& [nodeID, pair] = n; 
		find_end(nodeID); 
	}
}



//FINDS the end of graph for the given node(Currently very slow)
void graph::find_end(const data_nodeID & curNodeID)
{
	bool atEnd = true;
	for (auto& n : edges)
	{
		auto& [nodeID1, nodeID2] = n;
		if (nodeID1 == curNodeID)
		{
			atEnd = false; 
			find_end(nodeID2); 
		}
	}
	if (atEnd)
	{
		// creates a node if it hasnt been found
		if (end_graph_nodes.contains(curNodeID) == 0)
		{
			create_EoG_node(curNodeID);
			auto& [nodeAtEnd, num] = user_nodes.at(curNodeID);
			auto& EoG = end_graph_nodes.at(curNodeID);
			make_edge(nodeAtEnd, EoG);
		}
	}
}



//creates an EoG node to proceed a given node
void graph::create_EoG_node(const data_nodeID & node)
{
	end_graph_nodes.emplace(node, make_end_of_graph_node(g));
}



// allows users to use wait_for_all on the graph
void graph::wait_graph()
{
	g.wait_for_all(); 
}



//checks if the node is in the user_nodes
bool graph::is_node_in_graph(const data_nodeID  & nodeID) {

	if (user_nodes.count(nodeID) == 0)
		return false;

	return true;
}



//checks if edge is in graph
bool graph::is_edge_in_graph(const data_nodeID & nodeID1,const  data_nodeID & nodeID2) {
	for (auto& n : edges)
	{
		const auto& [edgeNode1, edgeNode2] = n;
		if (nodeID1 == edgeNode1 && nodeID2 == edgeNode2)
		{
			return true;
		}
	}
	return false;
}



// returns a join node
auto graph::add_join_node()
{
	return make_join_node<data_t,data_t>(g);

}



// returns a combine node
 auto graph::add_combine_node()
{
	 return make_combine_node<data_t,data_t>(g);
}



//creats a join node to precess a given node
void graph::create_join(const data_nodeID & node,const size_t & JOINS)
{

	join_nodes.emplace(node, std::make_tuple(add_join_node(), add_combine_node(), 0) );

	auto& [joinNode, combineNode, num] = join_nodes.at(node); 
	auto& [proNode, num2] = user_nodes.at(node);


	oneapi::tbb::flow::make_edge(joinNode, combineNode);
	oneapi::tbb::flow::make_edge(combineNode, proNode); 
}



//prints nodes
void graph::print_nodes() {
	for (const auto& n : user_nodes)
	{
		std::cout << "node: " << n.first << " " << n.second.second <<std::endl;
	}
}



//prints edge
void graph::print_edges()
{
	for (const auto& n : edges)
	{
		std::cout << "edge: " << n.first << " " << n.second << std::endl;
	}
}



//prints EoG nodes
void graph::print_EoG_nodes()
{
	for (auto& n : end_graph_nodes)
	{
		std::cout << "EoG: " << n.first << std::endl;
	}
}



//prints joins
void graph::print_join_nodes() {
	for (auto &  n : join_nodes)
	{
		std::cout << "joins: " << n.first << std::endl;
	}
}



// connects the join node
void graph::connect_to_join(const data_nodeID& nodeID1, const data_nodeID & nodeID2, size_t JOINS)
{
	
	auto& [node2, num2] = user_nodes.at(nodeID2);
	auto& [node1, num1] = user_nodes.at(nodeID1);

	if (JOINS == 2)
	{
		auto& [joinNode, combineNode, num] = join_nodes.at(nodeID2);
		std::cout << num << std::endl;
		if (num == 0)
		{
			oneapi::tbb::flow::make_edge(node1, std::get<0>(joinNode.input_ports()));
			num++;
		}
		else if (num == 1)
		{
			oneapi::tbb::flow::make_edge(node1, std::get<1>(joinNode.input_ports()));
			num++;
		}
		else {
			throw 1;
		}
	}
	else
	{
		//throw 1;
	}
}




// TODO
//void graph::find_start_node() {
//
//
//}