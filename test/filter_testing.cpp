
#include "filter_testing.h"
#include "../graph/graph.h"
#include <oneapi/tbb.h>
#include <math.h>
bool testSingleFilter() {
	graph g;



	g.add_filter_node("1", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return true;
		});

	std::vector<base_data> inputs;
	int total = 1000;
	for (int i = 0; i < total; i++)
	{
		inputs.push_back(std::tuple(i, -i));
	}
	g.add_start_node("1", inputs);

	g.build_graph();
	g.run_graph();
	g.wait_graph();
	std::vector<std::vector <base_data>> O;
	std::vector<std::vector <base_data>> T;

	g.get_output(O);
	g.get_trash(T);
	if (O.size() != 1)
	{
		return false;
	}


	if (T.size() != 1 && T.at(0).size() != total)
	{
		return false;
	}
	return true;
}



bool testFilterAndJoin() {
	graph g;



	g.add_filter_node("1", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return true;
		});
	g.add_filter_node("2", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return false;
		});
	g.add_proccess_node("3", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		//auto& [ID, inData] = *data;
		
		});
	g.add_proccess_node("4", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		//auto& [ID, inData] = *data;
		
		});
	g.add_proccess_node("0", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return false;
		});
	
	g.add_edge("0", "1");
	g.add_edge("0", "2");
	g.add_edge("0", "3");
	g.add_edge("1", "4");
	g.add_edge("2", "4");
	g.add_edge("3", "4");

	std::vector<base_data> inputs;
	int total = 1000;
	for (int i = 0; i < total; i++)
	{
		inputs.push_back(std::tuple(i, -i));
	}
	g.add_start_node("1", inputs);

	g.build_graph();
	g.run_graph();
	g.wait_graph();
	std::vector<std::vector <base_data>> O;
	std::vector<std::vector <base_data>> T;

	g.get_output(O);
	g.get_trash(T);
	if (O.size() != 1)
	{
		return false;
	}


	if (T.size() != 1 && T.at(0).size() != total)
	{
		return false;
	}
	return true;
}

bool isEven(int i)
{
	return ((i % 2)==0);
}


bool testEvenFilter()
{

graph g;



g.add_filter_node("1", oneapi::tbb::flow::unlimited, [](const data_t& data) {
	auto& [ID, inData] = data;
	return !(isEven(std::get<0>(*inData)));
	});

std::vector<base_data> inputs;
int total = 1000;
for (int i = 0; i < total; i++)
{
	inputs.push_back(std::tuple(i, -i));
}
g.add_start_node("1", inputs);

g.build_graph();
g.run_graph();
g.wait_graph();
std::vector<std::vector <base_data>> O;
std::vector<std::vector <base_data>> T;

g.get_output(O);
g.get_trash(T);

for (auto& t : T)
{
	for (auto& n : t)
	{
		if ((std::get<0>(n) % 2) == 0)
		{
			return false;
		}
	}
}

for (auto& o : O)
{
	for (auto& n : o)
	{
		if ((std::get<0>(n) % 2) == 1)
		{
			return false;
		}
	}
}
	




return true;
}


bool isPrime(int i)
{
	
	int stop = sqrt(i) + 1;
	if (i == 2)
	{
		return true; 

	}
	if (!(i & 1))
	{
		return false;
	}
	

	for (long j = 3; j < stop; j = j + 2)
	{
		if (i % j == 0)
		{
			return false;
		}
	}
	return true;
}

bool testPrimeFilter()
{

	graph g;



	g.add_filter_node("1", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		auto& [ID, inData] = data;
		return !(isPrime(std::get<0>(*inData)));
		});

	std::vector<base_data> inputs;
	int total = 1000;
	for (int i = 1; i < total; i++)
	{
		inputs.push_back(std::tuple(i, -i));
	}
	g.add_start_node("1", inputs);

	g.build_graph();
	g.run_graph();
	g.wait_graph();
	std::vector<std::vector <base_data>> O;
	std::vector<std::vector <base_data>> T;

	g.get_output(O);
	g.get_trash(T);

	for (auto& t : T)
	{
		for (auto& n : t)
		{
			if (isPrime(std::get<0>(n)))
			{
				return false;
			}
		}
	}

	for (auto& o : O)
	{
		for (auto& n : o)
		{
			if (!isPrime(std::get<0>(n)))
			{
				return false;
			}
		}
	}
}

bool testMultipleFiltersAndOutputs()
{

	graph g;

	g.add_proccess_node("0", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		
		});

	g.add_filter_node("1", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		auto& [ID, inData] = data;
		return !(isEven(std::get<0>(*inData)));
		});
	g.add_filter_node("2", oneapi::tbb::flow::unlimited, [](const data_t& data) {
		auto& [ID, inData] = data;
		return !(isPrime(std::get<0>(*inData)));
		});

	std::vector<base_data> inputs;
	int total = 20;
	for (int i = 0; i < total; i++)
	{
		inputs.push_back(std::tuple(i, -i));
	}
	g.add_start_node("0", inputs);
	g.add_edge("0", "1");
	g.add_edge("0", "2");
	g.build_graph();
	g.run_graph();
	g.wait_graph();
	std::vector<std::vector <base_data>> O;
	std::vector<std::vector <base_data>> T;

	g.get_output(O);
	g.get_trash(T);

	
	for (auto& n : T.at(0))
	{
		
		if (isEven(std::get<0>(n)))
		{
			
			return false;
		}
	}
	
	for (auto& n : O.at(0))
	{
		
		if (!isEven(std::get<1>(n)))
		{
			return false;
		}
	}
	
	for (auto& n : T.at(1))
	{
		
		if (isPrime(std::get<0>(n)))
		{
			return false;
		}
	}
	
	for (auto& n : O.at(1))
	{
		
		if (!isPrime(std::get<0>(n)))
		{
			return false;
		}
	}
	return true; 
}