#include "graph.h"
#include "splitter_node.h"
#include "filter_node.h"
#include <iostream>
#include "type_config.h"
#include "combine_node.h"
#include "combine_node.cpp"
#include "graph_end_node.h"
#include <fstream>
#include <math.h> 
// default constructor(currently no arguments)
graph::graph()  {

	

}

void graph::draw_user_graph(std::string file)
{
	
	std::ofstream f(file);
	if (f.is_open())
	{
		f << "digraph graphname {\n";
		f << "start [lable =\"start\"]\n";
		for (const auto& n : user_nodes)
		{
			f << n.first << " ["<< "lable = \""<< n.first<<"\"]\n"; 
		}

		for (const auto& n : edges)
		{
			f << n.first << " -> " << n.second << ";\n";
		}
		f << "start -> " << firstNode << "\n"; 
		f << "}";
	}
	
	//if (file.is_open())
	//{
	//	file << "graph graphname {\n";
	//	/*for (auto &  n:user_nodes) {
	//		file << n.first << '\n'; 
	//	}*/
	//}

}

void graph::draw_frame_graph(std::string file)
{

	std::ofstream f(file);
	if (f.is_open())
	{
		f << "digraph graphname {\n";
		f << "start [lable =\"start\"];\n";
		f << "EoG [lable =\"EoG\"];\n";
		for (const auto& n : user_nodes)
		{
			f << n.first << " [" << "lable = \"" << n.first << "\"];\n";
		}
		for (auto & n:joins)
		{
			f << n.second->listJoins(n.first);
			f << n.second->innerJoins(n.first);
		}
		for (const auto& [edge1, edge2] : edges)
		{
			
			auto& [node, num] = user_nodes.at(edge2);
			
			if (num == 1)
			{
				f << edge1 << " -> " << edge2 << ";\n";
			}
			else
			{
				
				auto & node = joins.at(edge2); 
				f << edge1 << " -> j" << edge2  <<
					(node->nextNodeToDraw()) / 10 << ";\n";
				

			}
			
		}
		f << "start -> " << firstNode << "\n";

		for (const auto& n : EoGs)
		{
			f << n << " -> EoG\n"; 
		}
		f << "}";
	}
	f.close();
}

//defualt deconstructor
graph::~graph() {

}



// runs the graph by activating the start node
void graph::run_graph() {
	if(start_node.size() != 0)
	start_node.at(0).activate();
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
void graph::add_edge( const  data_nodeID nodeID1, const data_nodeID  nodeID2) //THIS IS WHERE THE ISSUE IS CANNOT LINK BY REFERANCE
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
	
	if ((start_node.size() != 0) && (user_nodes.count(firstNode) != 0))
	{
		make_edge(start_node.at(0), (user_nodes.at(firstNode)).first);
	}
}


void graph::add_start_node(const data_nodeID Node, std::vector<base_data> F) {
	if(user_nodes.count(Node) != 0){
		inputs = F;
		firstNode = Node;
		start_node.push_back( make_start_node(g, inputs));
	}
	else
	{
		throw 7; 
	}
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
	if (firstNode.size() != 0)
	{
		find_end(firstNode);
	}
	
	//for (auto& n : user_nodes)
	//{
	//	auto& [nodeID, pair] = n; 
	//	find_end(nodeID); 
	//}
}



//FINDS the end of graph for the given node(Currently very slow)
void graph::find_end(const data_nodeID & curNodeID)
{
	bool atEnd = true;
	for (auto& n : edges)
	{
		if (n.first == curNodeID)
		{
			find_end(n.second);
			atEnd = false;
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
			EoGs.push_back(curNodeID);
		}
	}

	//bool atEnd = true;
	//for (auto& n : edges)
	//{
	//	auto& [nodeID1, nodeID2] = n;
	//	if (nodeID1 == curNodeID)
	//	{
	//		atEnd = false; 
	//		find_end(nodeID2); 
	//	}
	//}
	//if (atEnd)
	//{
	//	// creates a node if it hasnt been found
	//	if (end_graph_nodes.contains(curNodeID) == 0)
	//	{
	//		create_EoG_node(curNodeID);
	//		auto& [nodeAtEnd, num] = user_nodes.at(curNodeID);
	//		auto& EoG = end_graph_nodes.at(curNodeID);
	//		make_edge(nodeAtEnd, EoG);
	//		EoGs.push_back(curNodeID); 
	//	}
	//}
}



//creates an EoG node to proceed a given node
void graph::create_EoG_node(const data_nodeID & node)
{
	oneapi::tbb::concurrent_vector<base_data> trash;
	oneapi::tbb::concurrent_vector<base_data> valid;
	trash_outputs.push_back(trash);
	valid_outputs.push_back(valid);
	end_graph_nodes.emplace(node, make_end_of_graph_node(g,valid_outputs.back(),trash_outputs.back()));
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





//creats a join node to precess a given node




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
//void graph::print_join_nodes() {
//	for (auto &  n : join_nodes)
//	{
//		std::cout << "joins: " << n.first << std::endl;
//	}
//}



// connects the join node


void graph::create_join( data_nodeID nodeID, const size_t& JOINS)
{
	joins.emplace(nodeID, std::make_shared<JOIN_NODE>(JOINS, g));

	auto& [proNode, num2] = user_nodes.at(nodeID); 
	auto& JOIN = joins.at(nodeID); 
	auto &lastNode =JOIN->EndNode();
	lastNode->connect_to_combine(proNode);
	


	/*join_nodes.emplace(nodeID, std::make_tuple(add_join_node(), add_combine_node(), 0));

	auto& [joinNode, combineNode, num] = join_nodes.at(nodeID);
	auto& [proNode, num2] = user_nodes.at(nodeID);


	oneapi::tbb::flow::make_edge(joinNode, combineNode);
	oneapi::tbb::flow::make_edge(combineNode, proNode);*/
}

void graph::get_trash(std::vector<std::vector<base_data>>& V)
{
	V.resize(trash_outputs.size());
	int i = 0;
	for (auto & n: trash_outputs)
	{
		for (auto& v : n)
		{
			V.at(i).push_back(v);
			
		}
		i++;
	}
	
}

void graph::get_output(std::vector<std::vector<base_data>>& V)
{
	V.resize(valid_outputs.size());
	int i =0;
	for (auto& n : valid_outputs)
	{
		for (auto& v : n)
		{
			V.at(i).push_back(v);

		}
		i++;
	}
}

void graph::connect_to_join(const data_nodeID& nodeID1, const data_nodeID & nodeID2, size_t JOINS)
{
	auto & [node1, num1] = user_nodes.at(nodeID1);
	auto & JOIN = joins.at(nodeID2);

	oneapi::tbb::flow::make_edge(node1, JOIN->nextPort()); 



	//auto& [node2, num2] = user_nodes.at(nodeID2);
	//auto& [node1, num1] = user_nodes.at(nodeID1);

	//if (JOINS == 2)
	//{
	//	auto& [joinNode, combineNode, num] = join_nodes.at(nodeID2);
	//	std::cout << num << std::endl;
	//	if (num == 0)
	//	{
	//		oneapi::tbb::flow::make_edge(node1, std::get<0>(joinNode.input_ports()));
	//		num++;
	//	}
	//	else if (num == 1)
	//	{
	//		oneapi::tbb::flow::make_edge(node1, std::get<1>(joinNode.input_ports()));
	//		num++;
	//	}
	//	else {
	//		throw 1;
	//	}
	//}
	//else
	//{
	//	//throw 1;
	//}
}


//
//int log_base(int num, int base)
//{
//	return std::log(num) / std::log(base);
//}
//
//
//const int MAX_NUM_JOINS = 10;
//void graph::big_join(const data_nodeID& nodeID1, size_t& num)
//{
//	int temp = num / MAX_NUM_JOINS;
//	temp *= MAX_NUM_JOINS;
//	
//	int numOfTopJoins = num - temp;
//	int numOfTopCompleteJoins = numOfTopJoins / (MAX_NUM_JOINS-1); 
//	int lastNode = numOfTopJoins % (MAX_NUM_JOINS - 1); 
//	int complete_rows = log_base(num, MAX_NUM_JOINS); 
//
//	int 	
//
//
//
//
//
//
//}



// TODO
//void graph::find_start_node() {
//
//
//}


//big_join\
//
//
//
//// returns a join node
//auto graph::add_join_node()
//{
//	return make_join_node<data_t, data_t>(g);
//
//}
//
//
//
//// returns a combine node
//auto graph::add_combine_node()
//{
//	return make_combine_node<data_t, data_t>(g);
//}
//


