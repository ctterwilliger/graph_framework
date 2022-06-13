#include "graph.h"
#include "splitter_node.h"
#include "filter_node.h"
#include <iostream>
#include "type_config.h"
graph::graph() {

}



graph::~graph() {

}

bool graph::is_node_in_graph(std::string node) {

	if (user_nodes.count(node) == 0)
		return false;

	return true;
}

bool graph::is_edge_in_graph(std::string node1, std::string node2){
	for (auto & n: edges)
	{
		if (node1 == n.first && node2 == n.second)
		{
			return true;
		}
	}
	return false; 
}



void graph::add_filter_node(  std::string name, size_t const& concurrency, bool  Func)
{
	if ((is_node_in_graph(name)))
	{
		throw 1; /// IS NOT CAUGHT YET
	}
	else
	{
		user_nodes.emplace(name, std::make_pair( make_filter_node(g, concurrency, Func),0));
	}
	
}

template<typename Func> 
void graph::add_proccess_node(std::string name, size_t const& concurrency, Func f)
{
	if ((is_node_in_graph(name)))
	{
		throw 1; /// IS NOT CAUGHT YET
	}
	else
	{
		
		user_nodes.emplace(name, std::make_pair(make_proccess_node(g, concurrency, f),0));
	}
	
	
}

void graph::add_edge(std::string node1, std::string node2)
{
	if (is_edge_in_graph(node1, node2))
	{
		throw 1; // NOT HANDLED YET
	}
	else if (!(is_node_in_graph(node1)))
	{
		throw 2;
	}
	else if (!(is_node_in_graph(node2)))
	{
		throw 3;
	}
	else
	{
		edges.push_back(std::make_pair(node1, node2));
		refresh_graph();
	}
}

void graph::refresh_graph() {
	count_predecessors();
	
	// iterate through edges connects all nodes that are not being joined
	for (auto n : edges)
	{
		if (user_nodes.at(n.second).second == 1)
		{
			oneapi::tbb::flow::make_edge(user_nodes.at(n.first).first, user_nodes.at(n.second).first);
		}
	}



}

void graph::add_join_node()
{

}

void graph::add_combine_node()
{

}
void graph::create_join(std::string node)
{

}
void graph::print_nodes() {
	for (const auto& n : user_nodes)
	{
		std::cout << "node: " << n.first << std::endl;
	}
}

void graph::print_edges()
{
	for (const auto& n : edges)
	{
		std::cout << "edge: " << n.first << " " << n.second << std::endl;
	}
}

void graph::count_predecessors() 
{
	// sets all to 0 then counts if it is in edges as the second term
	for (auto& n : user_nodes) 
	{
		n.second.second = 0; 

		for (const auto& m : edges)
		{
			if (m.second == n.first)
			{
				n.second.second++; 
			}
		}
	}
}



//TODOOOOOOOOOOO
//
//

void graph::run_graph() {

}