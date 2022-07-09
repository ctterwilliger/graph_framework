
#include "test_contruction.h" 
#include "../graph/graph.h"
#include <string>
using namespace oneapi::tbb;
using namespace std;
// tests to make sure the graph can contruct and run a graph of size 0
bool zeroNodes()
{
	graph g; 
	g.build_graph();
	g.run_graph();
	g.wait_graph();
	return true; 

}

bool oneNode()
{
	graph g;
	g.add_proccess_node("1", oneapi::tbb::flow::unlimited, [](const data_t & data) {
	
		}
	);
	g.build_graph();
	g.run_graph();
	g.wait_graph();

	return true;
}

bool basicGraph() {
	graph g;
	g.add_proccess_node("1", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_proccess_node("2", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_filter_node("3", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		return false; 
		}
	);
	g.add_proccess_node("4", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);

	g.add_edge("1", "2");
	g.add_edge("1", "3");
	g.add_edge("3", "4");
	g.add_edge("2", "4");
	std::vector<base_data> V; 
	V.push_back(std::make_tuple(1, 1)); 
	g.add_start_node("1", V);
	g.build_graph();
	g.run_graph();
	g.wait_graph();
	return true; 
}

bool multiEoGs()
{
	graph g; 

	g.add_proccess_node("1", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);

	g.add_proccess_node("2", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);

	g.add_proccess_node("3", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);

	g.add_edge("1", "2");
	g.add_edge("1", "3");

	std::vector<base_data> V;
	V.push_back(std::make_tuple(1, 1));
	g.add_start_node("1", V);
	g.build_graph();
	g.run_graph();
	g.wait_graph();



	return true;
}

bool complexGraph()
{
	graph g;
	g.add_proccess_node("1", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_proccess_node("2", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_filter_node("3", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		return false;
		}
	);
	g.add_proccess_node("4", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_proccess_node("5", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_proccess_node("6", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_proccess_node("7", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_proccess_node("8", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_proccess_node("9", oneapi::tbb::flow::unlimited, [](const data_t& data) {

		}
	);
	g.add_edge("1", "2");
	g.add_edge("1", "3");
	g.add_edge("3", "4");
	g.add_edge("2", "4");
	g.add_edge("1", "5");
	g.add_edge("5", "6");
	g.add_edge("1", "7");
	g.add_edge("4", "9");
	g.add_edge("6", "9");
	g.add_edge("7", "9"); 
	g.add_edge("1", "8"); 
	std::vector<base_data> V;
	V.push_back(std::make_tuple(1, 1));
	g.add_start_node("1", V);
	g.build_graph();
	g.run_graph();
	g.wait_graph();
	return true;
}

bool largeJoin()
{

	graph g;


	g.add_filter_node("a1", flow::unlimited, [](const data_t& data) {
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
	for (int i = 1; i < 101; i++)
	{
		g.add_proccess_node(to_string(i), flow::unlimited, [](const data_t& data) {


			});
		g.add_edge(to_string(i), "a4");
		g.add_edge("a1", to_string(i));
	}



	std::vector<base_data> V;
	for (int i = 0; i < 10; i++)
	{

		V.push_back(std::make_tuple(i, -i));
	}

	g.add_start_node("a1", V);



	g.build_graph();

	g.run_graph();
	g.wait_graph();
	return true; 
}

