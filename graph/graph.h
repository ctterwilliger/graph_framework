#pragma once
#include <utility>
#include <vector>
#include "node.h"

class graph
{
public:
	
	void refresh_graph();
	

	template<typename Func>
	void add_node(typename Func);

	template<typename Func>
	void add_node(typename Func, size_t const & concurency);


	template<typename Func1, typename Func2>
	void add_edge(typename Func1, typename Func1);

	template<typename Func1, typename Func2>
	bool is_edge(typename Func1, typename Func1);


	template<typename Func1, typename Func2>
	void remove_edge(typename Func1, typename Func2);
	
	graph(); 
	~graph();



private:
	std::vector<node*> nodes;
	std::vector<std::pair<node*, node*>> edges;
	oneapi::tbb::flow::graph g; 
};
