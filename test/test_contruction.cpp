
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
	g.add_proccess_node("1", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.build_graph();
	g.run_graph();
	g.wait_graph();

	return true;
}

bool basicGraph() {
	graph g;
	g.add_proccess_node("1", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("2", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("3", "key1", "key2", [](int i) {
		return false; 
		}
	);
	g.add_proccess_node("4", "key1", "key2", [](int i) {
		return 1;
		}
	);

	g.add_edge("1", "2");
	g.add_edge("1", "3");
	g.add_edge("3", "4");
	g.add_edge("2", "4");
	std::vector<data_obj> V; 
	auto temp = std::make_shared<base_data>();

	temp->addData<int>("key1", 1);
	//cout << temp->getData<double>("key1");
	V.push_back(temp);
	g.add_start_node("1", V);
	g.build_graph();
	
	g.run_graph();
	g.wait_graph();
	return true;
}

bool multiEoGs()
{
	graph g; 

	g.add_proccess_node("1", "key1", "key2", [](int i) {
		return 1;
		}
	);

	g.add_proccess_node("2", "key1", "key2", [](int i) {
		return 1;
		}
	);

	g.add_proccess_node("3", "key1", "key2", [](int i) {
		return 1;
		}
	);
	//std::cout << "Test" << endl; 
	g.add_edge("1", "2");
	g.add_edge("1", "3");

	std::vector<data_obj> V;
	auto temp = std::make_shared<base_data>();

	temp->addData<int>("key1", 1);
	
	V.push_back(temp);
	g.build_graph();
	g.run_graph();
	g.wait_graph();



	return true;
}

bool complexGraph()
{
	graph g;
	g.add_proccess_node("1", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("2", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("3", "key1", "key2", [](int i) {
		return false;
		}
	);
	g.add_proccess_node("4", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("5", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("6", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("7", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("8", "key1", "key2", [](int i) {
		return 1;
		}
	);
	g.add_proccess_node("9", "key1", "key2", [](int i) {
		return 1;
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
	std::vector<data_obj> V;
	auto temp = std::make_shared<base_data>();

	temp->addData<int>("key1", 1);

	V.push_back(temp);
	g.build_graph();
	g.run_graph();
	g.wait_graph();
	return true;
}

bool largeJoin()
{

	graph g;


	g.add_proccess_node("a1", "key1", "key2", [](int i) {
		return false;
		
		});


	g.add_proccess_node("a4", "key1", "key2", [](int i) {
		return 1;

		});
	g.add_proccess_node("0", "key1", "key2", [](int i) {

		return false;
		}


	);
	g.add_edge(to_string(0), "a4");
	g.add_edge("a1", to_string(0));
	for (int i = 1; i < 123; i++)
	{
		g.add_proccess_node(to_string(i), "key1", "key2", [](int i) {
			return 1;

			});
		g.add_edge(to_string(i), "a4");
		g.add_edge("a1", to_string(i));
	}



	std::vector<data_obj> V;
	auto temp = std::make_shared<base_data>();

	temp->addData<int>("key1", 1);

	V.push_back(temp);

	g.add_start_node("a1", V);



	g.build_graph();

	g.run_graph();
	g.wait_graph();
	return true; 
}

