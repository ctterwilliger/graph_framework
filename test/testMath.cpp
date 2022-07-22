#include "testMath.h"
#include "../graph/graph.h"
#include <math.h>
#include <string>

using namespace oneapi::tbb;


bool testPrimeSieve()
{
	graph primeGraph; 
	
	primeGraph.add_proccess_node("findStop", "key1", "key2", [](int i) {
		 
		
		return sqrt(i) + 1; 

	});
	
	primeGraph.add_proccess_node("checkBase", std::tuple("key1"),  [](int int1)
	{
		
		if (int1 == 4 || int1 == 6 || int1 == 8 || int1 == 9 || int1 == 10)
		{
			
			return true;
		}
		if (int1 % 5 == 0 && int1 != 5)
		{
			return true;
		}
		return false;
	}); 
	
	primeGraph.add_proccess_node("checkEven", std::tuple("key1"), [](int int1) {
		
		if (int1 == 2)
		{
			return false; 
		}
		return (bool)!(int1 & 1);
		});

	primeGraph.add_proccess_node("check1s", std::tuple("key1","key2"), [](int int1, int int2) {
		
		for (int i = 11; i < int2; i+=10)
		{
			if (int1 % i == 0)
			{
				return true;
			}
		}

		return false;
		});
	
	primeGraph.add_proccess_node("check3s", std::tuple("key1", "key2"), [](int int1, int int2) {
		
		for (int i = 3; i < int2; i += 10)
		{
			if (int1 % i == 0)
			{
				return true;
			}
		}
		return false;
		});

	primeGraph.add_proccess_node("check7s", std::tuple("key1", "key2"), [](int int1, int int2) {
		
		for (int i = 7; i < int2; i += 10)
		{
			if (int1 % i == 0)
			{
				return true;
			}
		}
		return false;
		});
	
	primeGraph.add_proccess_node("check9s", std::tuple("key1", "key2"), [](int int1, int int2) {
		
		for (int i = 9; i < int2; i += 10)
		{
			if (int1 % i == 0)
			{
				return true;
			}
		}
		return false;
		});
	
	primeGraph.add_proccess_node("end", "key6", []() {
		return 1; 
		});
	
	std::vector<data_obj> inputs; 
	int total = 1'000'000;
	for (int i = 0; i < total; i++)
	{
		auto temp = std::make_shared<base_data>();

		temp->addData<int>("key1", i);
		inputs.push_back(temp);
	}
	
	primeGraph.add_start_node("findStop", inputs);
	primeGraph.add_edge("findStop", "checkEven");
	primeGraph.add_edge("findStop", "checkBase");
	primeGraph.add_edge("findStop", "check1s");
	primeGraph.add_edge("findStop", "check3s");
	primeGraph.add_edge("findStop", "check7s");
	primeGraph.add_edge("findStop", "check9s");
	primeGraph.add_edge("checkEven", "end");
	primeGraph.add_edge("checkBase", "end");
	primeGraph.add_edge("check1s", "end");
	primeGraph.add_edge("check3s", "end");
	primeGraph.add_edge("check7s", "end");
	primeGraph.add_edge("check9s", "end");
	
	primeGraph.build_graph();
	
	primeGraph.run_graph();
	primeGraph.wait_graph(); 
	std::cout << "test" << std::endl;
	std::vector<std::vector <data_obj>> O;
	std::vector<std::vector <data_obj>> T;
	
	primeGraph.get_output(O);
	primeGraph.get_trash(T);
	
	for (auto& t : T)
	{
		for (auto& n : t)
		{
			
			if (isPrime(n->getData<int>("key1")))
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
	return true; 
}