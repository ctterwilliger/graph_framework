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

bool graph::is_edge_in_graph(std::string node1, std::string node2) {
	for (auto& n : edges)
	{
		const auto& [edgeNode1, edgeNode2] = n;
		if (node1 == edgeNode1 && node2 == edgeNode2)
		{
			return true;
		}
	}
	return false;
}



void graph::add_filter_node(std::string name, size_t const& concurrency, bool  Func)
{
	if ((is_node_in_graph(name)))
	{
		throw 1; /// IS NOT CAUGHT YET
	}
	else
	{
		user_nodes.emplace(name, std::make_pair(make_filter_node(g, concurrency, Func), 0));
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

		user_nodes.emplace(name, std::make_pair(make_proccess_node(g, concurrency, f), 0));
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
	}
}

void graph::refresh_graph() {
	count_predecessors();

	create_join_nodes();

	// iterate through edges connects all nodes 
	connect_nodes(); 

	find_EoG();
	find_start_node(); 


}

void graph::find_EoG()
{

}
void graph::find_end()
{

}
void graph::find_start_node() {

}

void graph::create_join_nodes() {
	for (const auto& n : user_nodes)
	{
		const auto& [name, pair] = n;
		const auto& [node, num] = pair;
		if (num== 2)
		{
			
			create_join(name); 
		}
	}
}

void graph::connect_nodes()
{

	for (const auto & n : edges)
	{
		 auto& [name1, name2] = n;
		 auto& [node2, num2] = user_nodes.at(name2);
		 auto& [node1, num1] = user_nodes.at(name1);
		
		if (num2 == 0) {

			// donothing

		}
		else if (num2 == 1)
		{
			oneapi::tbb::flow::make_edge(node1,node2);
		}
		else if (num2 == 2)
		{
			auto [joinNode, combineNode, num] = join_nodes.at(name2);
			


			if (num == 0)
			{
				oneapi::tbb::flow::make_edge(node1, std::get<0>(joinNode.input_ports())); 
			}
			else if (num == 1)
			{
				oneapi::tbb::flow::make_edge(node1, std::get<1>(joinNode.input_ports()));
			}
			else {
				throw 1; 
			}
		}
		else
		{
			throw 1; 
		}

	}
}

auto graph::add_join_node()
{
	return make_join_node(g);
}

 auto graph::add_combine_node()
{
	 return make_combine_node(g);
}

void graph::create_join(std::string node)
{

	join_nodes.emplace(node, std::make_tuple(add_join_node(), add_combine_node(), 0) );

	auto& [joinNode, combineNode, num] = join_nodes.at(node); 
	auto& [proNode, num2] = user_nodes.at(node);

	oneapi::tbb::flow::make_edge(joinNode, combineNode);

	oneapi::tbb::flow::make_edge(combineNode, proNode); 
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