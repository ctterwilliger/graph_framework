
#include "filter_testing.h"
#include "../graph/graph.h"
#include <oneapi/tbb.h>
#include <math.h>
bool testSingleFilter() {
	graph g;



	g.add_proccess_node("1", "key1", "key2", [](int i) {
		//auto& [ID, inData] = *data;
		return true;
		});

	std::vector<data_obj> inputs;
	int total = 1000;
	for (int i = 0; i < total; i++)
	{
		auto temp = std::make_shared<base_data>();

		temp->addData<int>("key1", 1);
	}
	g.add_start_node("1", inputs);

	g.build_graph();
	g.run_graph();
	g.wait_graph();
	std::vector<std::vector <data_obj>> O;
	std::vector<std::vector <data_obj>> T;

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



	g.add_proccess_node("1", "key1", "key2", [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return true;
		});
	g.add_proccess_node("2", "key1", "key2", [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return false;
		});
	g.add_proccess_node("3", "key1", "key2", [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return 1;
		});
	g.add_proccess_node("4", "key1", "key2", [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return 1;
		});
	g.add_proccess_node("0", "key1", "key2", [](const data_t& data) {
		//auto& [ID, inData] = *data;
		return false;
		});
	
	g.add_edge("0", "1");
	g.add_edge("0", "2");
	g.add_edge("0", "3");
	g.add_edge("1", "4");
	g.add_edge("2", "4");
	g.add_edge("3", "4");

	std::vector<data_obj> inputs;
	int total = 1000;
	for (int i = 0; i < total; i++)
	{
		auto temp = std::make_shared<base_data>();

		temp->addData<int>("key1", 1);
	}
	g.add_start_node("1", inputs);

	g.build_graph();
	g.run_graph();
	g.wait_graph();
	

	std::vector<std::vector <data_obj>> O;
	std::vector<std::vector <data_obj>> T;

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



g.add_proccess_node("1", "key1", "key2", [](int i) {
	
	return !(isEven(i));
	});

std::vector<data_obj> inputs;
int total = 1000;
for (int i = 0; i < total; i++)
{
	auto temp = std::make_shared<base_data>();

	temp->addData<int>("key1", 1);
}
g.add_start_node("1", inputs);

g.build_graph();
g.run_graph();
g.wait_graph();


std::vector<std::vector <data_obj>> O;
std::vector<std::vector <data_obj>> T;
g.get_output(O);
g.get_trash(T);

for (auto& t : T)
{
	for (auto& n : t)
	{
		if (((n->getData<int>("key1")) % 2) == 0)
		{
			return false;
		}
	}
}

for (auto& o : O)
{
	for (auto& n : o)
	{
		if (((n->getData<int>("key1")) % 2) == 1)
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



	g.add_proccess_node("1", "key1", "key2", [](int i) {
		
		return !(isPrime(i));
		});

	std::vector<data_obj> inputs;
	int total = 1000;
	for (int i = 0; i < total; i++)
	{
		auto temp = std::make_shared<base_data>();

		temp->addData<int>("key1", 1);
	}
	g.add_start_node("1", inputs);

	g.build_graph();
	g.run_graph();
	g.wait_graph();


	std::vector<std::vector <data_obj>> O;
	std::vector<std::vector <data_obj>> T;
	g.get_output(O);
	g.get_trash(T);

	for (auto& t : T)
	{
		for (auto& n : t)
		{
			if (isPrime((n->getData<int>("key1"))))
			{
				return false;
			}
		}
	}

	for (auto& o : O)
	{
		for (auto& n : o)
		{
			if (!isPrime(n->getData<int>("key1")))
			{
				return false;
			}
		}
	}
}

bool testMultipleFiltersAndOutputs()
{

	graph g;

	g.add_proccess_node("0", "key1", "key2", [](int i) {
		return 1;
		});

	g.add_proccess_node("1", "key1", "key2", [](int i) {
		
		return !(isEven(i));
		});
	g.add_proccess_node("2", "key1", "key2", [](int i) {
		
		return !(isPrime(i));
		});
	g.add_edge("0", "1");
	g.add_edge("0", "2");

	std::vector<data_obj> inputs;
	int total = 1000;
	for (int i = 0; i < total; i++)
	{
		auto temp = std::make_shared<base_data>();

		temp->addData<int>("key1", 1);
	}
	g.add_start_node("0", inputs);

	g.build_graph();
	g.run_graph();
	g.wait_graph();


	std::vector<std::vector <data_obj>> O;
	std::vector<std::vector <data_obj>> T;

	g.get_output(O);
	g.get_trash(T);

	std::cout << O.size() << " " << O.at(0).size() << " " << T.size() << " " << T.at(0).size();
	
	for (auto& n : T.at(0))
	{
		
		if (isEven(n->getData<int>("key1")))
		{
			
			return false;
		}
	}
	
	for (auto& n : O.at(0))
	{
		
		if (!isEven(n->getData<int>("key1")))
		{
			return false;
		}
	}
	
	for (auto& n : T.at(1))
	{
		
		if (isPrime(n->getData<int>("key1")))
		{
			return false;
		}
	}
	
	for (auto& n : O.at(1))
	{
		
		if (!isPrime(n->getData<int>("key1")))
		{
			return false;
		}
	}
	return true; 
}