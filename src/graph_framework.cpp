// graph_framework.cpp : Defines the entry point for the application.
//

#include "graph_framework.h"
#include "oneapi/tbb.h"
#include "../graph/framework.hpp"
#include "../graph/graph.h"
#include "../graph/type_config.h"
#include <tuple>

using namespace std;
using namespace oneapi::tbb;


// FOR TESTING PURPOSES ONLY!!!!!!!!!!!!!!!!


int counter[4] = {0,0,0,0};





int main()
{


	user_data_store DATA;

	graph grph;

	grph.add_proccess_node("start", flow::unlimited, [](const data_t& data) {
		});


	grph.add_filter_node("1", flow::unlimited,
		[](const data_t& data) {
			counter[0] += 1;
			return false; 

		});


	grph.add_proccess_node("2", flow::unlimited, [](const data_t & data) {
		counter[1] += 1;
		});
	grph.add_proccess_node("22", flow::unlimited, [](const data_t& data) {
		counter[1] += 1;
		});
	grph.add_proccess_node("3", flow::unlimited, [] (const data_t & data){
		counter[2] += 1;
		}); 

	grph.add_proccess_node("4", flow::unlimited, [](const data_t& data) {

		counter[3] += 1;
		});
	grph.add_edge("3", "4");


	grph.add_edge("start", "1");
	grph.add_edge("start", "2");
	grph.add_edge("start", "22");
	
	grph.add_edge("22", "3");
	grph.add_edge("2", "3");
	grph.add_edge("1", "3");
	grph.print_nodes();
	grph.print_edges(); 
	grph.add_filter_node("11", flow::unlimited,
		[](const data_t& data) {
			counter[0] += 1;
			return false;

		});
	grph.add_edge("start", "11");
	grph.add_edge("11", "3"); 
	//for (int i = 10; i < 15; i++)
	//{
	//	std::string node = to_string(i);
	//	grph.add_proccess_node(node, flow::unlimited, [](const data_t& data) {});
	//	grph.add_edge("start", node);
	//	grph.add_edge(node, "3");
	//}

	grph.build_graph();


	grph.print_nodes();
	grph.print_edges();

	grph.print_EoG_nodes(); 
	grph.run_graph();
	grph.wait_graph(); 
	for (auto n : counter)
	{
		cout << n << endl; 
	}
	return 0;
}
