#include "graph.h"
#include "splitter_node.h"
#include "filter_node.h"
graph::graph() {

}



graph::~graph() {

}

bool graph::is_node_in_graph(std::string){
	return true;
	}
bool graph::is_edge_in_graph(std::string, std::string){
	return true;
}
void graph::add_filter_node(  std::string name, size_t const& concurrency, bool  Func)
{
	if (!(is_node_in_graph(name)))
	{
		throw 1; /// IS NOT CAUGHT YET
	}
	else
	{
		user_nodes.emplace(name, std::make_pair( make_filter_node(g, concurrency, Func), 0));
	}
	
}

template<typename Func>
void graph::add_proccess_node(std::string name, size_t const& concurrency, Func) {
	if (!(is_node_in_graph(name)))
	{
		throw 1; /// IS NOT CAUGHT YET
	}
	else
	{
		user_nodes.emplace(name, std::make_pair( make_proccess_node(g, concurrency, Func), 0 ));
	}
	
}

void graph::add_edge(std::string node1, std::string node2)
{
	if (is_edge_in_graph())
	{
		edges.push_back()
	}
	else
	{

	}
}

//TODOOOOOOOOOOO
//
//void refresh_graph();
//
//
//template<typename Func>
//void graph::add_node(typename Func);
//
//template<typename Func>
//void graph::add_process_node(typename Func, size_t const& concurrency);
//
//
//template<typename Func>
//void add_join_node(typename Func, size_t const& concurrency);
//
//template<typename Func>
//void add_combine_node(typename Func, size_t const& concurrency);
//
//template<typename Func>
//void add_EoG_node(typename Func, size_t const& concurrency);
//
//template<typename Func1, typename Func2>
//void add_edge(typename Func1, typename Func1);
//
//template<typename Func1, typename Func2>
//bool is_edge(typename Func1, typename Func1);
//
//
//template<typename Func1, typename Func2>
//void remove_edge(typename Func1, typename Func2);

