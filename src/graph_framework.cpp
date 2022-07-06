// graph_framework.cpp : Defines the entry point for the application.
//

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
int counter[4] = {0,0,0,0};





int main()
{
	
	graph g; 
	std::vector<base_data> V;

	g.add_proccess_node("1", flow::unlimited, [](const data_t& data) {


		});

	g.add_filter_node("2", flow::unlimited, [](const data_t& data) {
		return true; 

		});

	g.add_proccess_node("3", flow::unlimited, [](const data_t& data) {


		});
	g.add_proccess_node("4", flow::unlimited, [](const data_t& data) {


		});

	for (int i = 10; i < 19; i++)
	{
		g.add_proccess_node(to_string(i), flow::unlimited, [](const data_t& data) {


			});
		g.add_edge(to_string(i), "4"); 
		g.add_edge("1", to_string(i)); 
	}
	g.add_edge("1", "2");
	g.add_edge("2", "4");
	g.add_edge("1", "3");
	g.add_edge("3", "4"); 


	for (int i = 0; i < 10; i++)
	{
		base_data temp= std::make_tuple(i, -i); 
		V.push_back(temp);
	}
	g.add_start_node("1",V);
	g.build_graph(); 
	g.draw_user_graph("test.dot"); 
	g.draw_frame_graph("frame.dot");
	g.run_graph();
	g.wait_graph(); 
	return 0; 
}
