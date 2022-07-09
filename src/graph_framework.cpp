// graph_framework.cpp : Defines the entry point for the application.
//
#include <atomic>


#include "graph_framework.h"
#include "oneapi/tbb.h"
#include "../graph/framework.hpp"
#include "../graph/graph.h"
#include "../graph/type_config.h"
#include <tuple>
#include <fstream>
using namespace std;
using namespace oneapi::tbb;


// FOR TESTING PURPOSES ONLY!!!!!!!!!!!!!!!!






int main()
{
	
	graph g; 
	

	g.add_filter_node("a1", 1, [](const data_t& data) {
		return false;

		});

	
	g.add_proccess_node("a4", flow::unlimited, [](const data_t& data) {


		});


	g.add_filter_node("0", flow::unlimited, [](const data_t& data) {
	  
		return false;
		}
	
	
	);
	g.add_edge(to_string(0), "a4");
	g.add_edge("a1", to_string(0));
	for(int i = 1; i < 1001; i++)
	{
		g.add_proccess_node(to_string(i), flow::unlimited, [](const data_t& data) {


			});
		g.add_edge(to_string(i), "a4"); 
		g.add_edge("a1", to_string(i)); 
	}
	
	

	std::vector<base_data> V;
	for (int i = 0; i < 1; i++)
	{
		
		V.push_back(std::make_tuple(i, -i));
	}

	g.add_start_node("a1",V);


	std::vector<std::vector<base_data>> O;


	
	g.build_graph(); 
	g.draw_user_graph("test.dot"); 
	g.draw_frame_graph("frame.dot");
	g.run_graph();
	g.wait_graph(); 
	g.get_output(O);
	for (auto& j : O)
	{
		for (auto& n : j)
		{
			auto& [int1, int2] = n; 
			cout << int1 << " " << int2 << endl;;
		}
	}
	
	
	return 0; 
}
