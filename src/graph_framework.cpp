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




int trun(double num)
{
	return (int)num;
}

int main()
{

	graph g;

	
	g.add_proccess_node("1", input_keys("key1-1", "key1-2"), std::tuple("key2-1", "key2-2"), [](int i, double num) {
		//ut << num << "test" << std::endl;
		//cout << i << " " << num << endl;
		return std::tuple(i , num);
		});
	std::vector<data_obj> V;

	for (int i = 0; i <10;i++)
	{
		
		auto temp = std::make_shared<base_data>();

		temp->addData<double>("key1-2", i);
		temp->addData<int>("key1-1", i);
		//cout << temp->getData<double>("key1");
		V.push_back(temp);
	}



	g.add_start_node("1", V);
	
	

	

	//g.add_start_node("a1",V);


	std::vector<std::vector<data_obj>> O;
	std::vector<std::vector<data_obj>> T;

	
	g.build_graph(); 
	g.draw_user_graph("test.dot"); 
	g.draw_frame_graph("frame.dot");
	//cout << "test";
	g.run_graph();
	g.wait_graph(); 
	g.get_output(O);


	g.get_trash(T); 
	for (auto& j : O)
	{
		for (auto& n : j)
		{
			std::cout << n->getData<int>("key2-1") << " " << n->getData<double>("key2-2") << endl; 
			
			
			//auto& [int1, int2] = n; 
			//cout << int1 << " " << int2 << endl;;
		}
	}
	//
	
	return 0; 
}
