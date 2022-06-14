#include "graph.h"
#include "splitter_node.h"
#include "filter_node.h"
#include <iostream>
#include "type_config.h"
#include "combine_node.h"
#include "graph_end_node.h"



graph::graph() {

}



graph::~graph() {

}




template<typename FT>
void graph::add_filter_node(data_nodeID name, size_t const& concurrency, FT  Func)
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
void graph::add_proccess_node(data_nodeID name, size_t const& concurrency, Func f)
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

void graph::add_edge(data_nodeID node1, data_nodeID node2)
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

	//finds and creates EoG nodes
	find_EoG();
	//find_start_node(); 
	make_edge(start_node, (user_nodes.at("start")).first) ;

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

void graph::create_join_nodes() {
	for (const auto& n : user_nodes)
	{
		const auto& [name, pair] = n;
		const auto& [node, num] = pair;
		if (num == 2)
		{

			create_join(name);
		}
	}
}


void graph::connect_nodes()
{

	for (const auto& n : edges)
	{
		auto& [name1, name2] = n;
		auto& [node2, num2] = user_nodes.at(name2);
		auto& [node1, num1] = user_nodes.at(name1);
		//std::cout << num2 << std::endl; 
		if (num2 == 0) {

			// donothing

		}
		else if (num2 == 1)
		{
			oneapi::tbb::flow::make_edge(node1, node2);
		}
		else if (num2 == 2)
		{
			auto& [joinNode, combineNode, num] = join_nodes.at(name2);
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
}

void graph::find_EoG()
{
	// VERY SLOW n^3 logn runtime 
	for (auto& n : user_nodes)
	{
		auto& [node, NODEANDNUM] = n; 
		find_end(node); 
	}
}
void graph::find_end(data_nodeID node)
{
	bool atEnd = true;
	for (auto& n : edges)
	{
		auto& [node1, node2] = n;
		if (node1 == node)
		{
			atEnd = false; 
			find_end(node2); 
		}
	}
	if (atEnd)
	{
		create_EoG_node(node); 
		auto& [nodeAtEnd, num] = user_nodes.at(node);
		auto& EoG = end_graph_nodes.at(node);
		make_edge(nodeAtEnd, EoG); 
	}
}

void graph::create_EoG_node(data_nodeID node)
{
	end_graph_nodes.emplace(node, make_end_of_graph_node(g));
}




void graph::find_start_node() {
	
	
}

void graph::wait_graph()
{
	g.wait_for_all(); 
}


bool graph::is_node_in_graph(data_nodeID  node) {

	if (user_nodes.count(node) == 0)
		return false;

	return true;
}

bool graph::is_edge_in_graph(data_nodeID node1, data_nodeID node2) {
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





auto graph::add_join_node()
{
	return make_join_node<data_t,data_t>(g);

}

 auto graph::add_combine_node()
{
	 return make_combine_node<data_t,data_t>(g);
}

void graph::create_join(data_nodeID node)
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
		std::cout << "node: " << n.first << " " << n.second.second <<std::endl;
	}
}

void graph::print_edges()
{
	for (const auto& n : edges)
	{
		std::cout << "edge: " << n.first << " " << n.second << std::endl;
	}
}





//TODOOOOOOOOOOO
//
//

void graph::run_graph() {
	start_node.activate();
}

void graph::print_EoG_nodes()
{
	for (auto& n : end_graph_nodes)
	{
		std::cout << "EoG: " << n.first << std::endl;
	}
}

void graph::print_join_nodes() {
	for (auto &  n : join_nodes)
	{
		std::cout << "joins: " << n.first << std::endl;
	}
}