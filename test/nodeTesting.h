#pragma once

#include "../graph/graph.h"



//non lamda functions
//bool testNonLambdaFunction();



//no return
bool testNoReturn()
{
	graph g;
	g.add_process_node("1", std::tuple("key1"), [](int i)
		{
			return false;
		}
	);


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

bool testNoReturnKeyFilter()
{
	graph g;
	g.add_process_node("1", std::tuple("key1"), [](int i)
		{
			return false;
		}
	);


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
