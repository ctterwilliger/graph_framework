#include "testMath.h"
#include "../graph/graph.h"
#include <math.h>
#include <string>

using namespace oneapi::tbb;


bool testPrimeSieve()
{
	graph primeGraph; 

	primeGraph.add_proccess_node("findStop", flow::unlimited, [](const data_t & data) {
		auto& [ID, inData] = data; 
		auto& [int1, int2] = *inData;
		int2 = sqrt(int1) + 1; 
	});

	primeGraph.add_filter_node("checkBase", flow::unlimited, [](const data_t & data)
	{
		auto& [ID, inData] = data;
		auto& [int1, int2] = *inData;
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
	primeGraph.add_filter_node("checkEven", flow::unlimited, [](const data_t& data) {
		auto& [ID, inData] = data;
		auto& [int1, int2] = *inData;
		if (int1 == 2)
		{
			return false; 
		}
		return (bool)!(int1 & 1);
		});

	primeGraph.add_filter_node("check1s", flow::unlimited, [](const data_t& data) {
		auto& [ID, inData] = data;
		auto& [int1, int2] = *inData;
		for (int i = 11; i < int2; i+=10)
		{
			if (int1 % i == 0)
			{
				return true;
			}
		}
		return false;
		});

	primeGraph.add_filter_node("check3s",flow::unlimited, [](const data_t& data) {
		auto& [ID, inData] = data;
		auto& [int1, int2] = *inData;
		for (int i = 3; i < int2; i += 10)
		{
			if (int1 % i == 0)
			{
				return true;
			}
		}
		return false;
		});

	primeGraph.add_filter_node("check7s", flow::unlimited, [](const data_t& data) {
		auto& [ID, inData] = data;
		auto& [int1, int2] = *inData;
		for (int i = 7; i < int2; i += 10)
		{
			if (int1 % i == 0)
			{
				return true;
			}
		}
		return false;
		});
	primeGraph.add_filter_node("check9s", flow::unlimited, [](const data_t& data) {
		auto& [ID, inData] = data;
		auto& [int1, int2] = *inData;
		for (int i = 9; i < int2; i += 10)
		{
			if (int1 % i == 0)
			{
				return true;
			}
		}
		return false;
		});
	primeGraph.add_proccess_node("end", flow::unlimited, [](const data_t& data) {
		
		});
	std::vector<base_data> inputs; 
	int total = 10'000'000;
	for (int i = 0; i < total; i++)
	{
		inputs.push_back(std::tuple(i, 0));
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
	std::vector<std::vector <base_data>> O;
	std::vector<std::vector <base_data>> T;

	primeGraph.get_output(O);
	primeGraph.get_trash(T);

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
	return true; 
}