#include "combine_node.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include "type_config.h"
#include <math.h>
#include <iostream>
#include <oneapi/tbb.h>
using joinNode10 = join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t>;
int log_baseB(size_t num, size_t base)
{
	return std::log(num) / std::log(base);
}





JOIN_NODE::JOIN_NODE( size_t numOfJoins, oneapi::tbb::flow::graph& graph) :g(graph)
{
	
	curPort = 0; 
	curNode = 0; 
	

	
	if (numOfJoins <= 10)
	{
	
		add_node(numOfJoins);
	}
	else
	{
		add_node(); 
		size_t numOfCompleteLayers = log_baseB(numOfJoins, MAXNUMOFJOINS);
		for (int i = 1; i < numOfCompleteLayers; i++)
		{
			for (int j = 0; j < pow(MAXNUMOFJOINS, i); j++)
			{
				//std::cout << i << " " << j << std::endl;
				add_node(10);
			}
		}
		size_t curNumOfJoins = pow(MAXNUMOFJOINS, numOfCompleteLayers);
		if (curNumOfJoins != numOfJoins)
		{
			
			while (curNumOfJoins < (numOfJoins - MAXNUMOFJOINS))
			{
				add_node(10);
			
				
				curNumOfJoins += (MAXNUMOFJOINS - 1);
			}
			
			
			add_node(numOfJoins - curNumOfJoins + 1);
		}
		
	}
	drawplace = curPort + curNode * 10 -1; 
}



void JOIN_NODE::add_node()
{
	joins.push_back(std::make_shared<joinNode10>(g));
	auto& lastNode = joins.at(joins.size()-1);
	if (joins.size() != 1) {
		
		lastNode->connect_to_node(nextPort());
	}
	
}


void JOIN_NODE::add_node(size_t  nodeSize)
{

	switch (nodeSize) {
	default: 
		throw 1; //not caught
	case 1:
		throw 1; 
		break; 
	case 2:
	
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t>>(g));
		break; 
	case 3: 
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t,data_t>>(g));
		break; 					   
	case 4:						  
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t,data_t>>(g));
		break;				  
	case 5:						  
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t,data_t,data_t>>(g));
		break;					  
	case 6:						  
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t,data_t>>(g));
		break;					  
	case 7:						  
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t,data_t>>(g));
		break;					  
	case 8:					  
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t,data_t>>(g));
		break;					  
	case 9:						  
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t,data_t>>(g));
		break;					  
	case 10:					  
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t,data_t>>(g));
		break;
	}

	if (joins.size() != 1) {
		//std::cout << "Here?" << std::endl; 
		auto& lastNode = joins.at(joins.size()-1);
		lastNode->connect_to_node(nextPort()); 
	}
	
}


oneapi::tbb::flow::receiver<data_t> & JOIN_NODE::nextPort(){
	auto & node = joins.at(curNode);
	auto & port = node->get_join_port(curPort);
	
	curPort++;
	if ((curPort % MAXNUMOFJOINS) == 0) {
		curPort = 0;
		curNode++; 
	}
	return port; 
}

std::shared_ptr<join_base> & JOIN_NODE::EndNode() {
	return joins.at(0);
}

JOIN_NODE::~JOIN_NODE() {

}





template<typename ...T>
join_and_combine<T...>::join_and_combine(oneapi::tbb::flow::graph& gr) : g(gr)
{

	make_edge(join, combine);
}


std::string JOIN_NODE::listJoins(std::string ID)
{
	std::string output = "";
	for(int count = 0; count < joins.size(); count++)
	{
		output += "j";
		output += ID;
		
		output += std::to_string(count);
		output += "[lable =\"";
		output += ID + "join"+ std::to_string(count);
		output += +"\"];\n";
	}
	return output;
}

std::string JOIN_NODE::innerJoins(std::string ID)
{
	std::string output = "j" +ID + "0 -> " + ID + ";\n";
	int count = 0;
	
	for (int i = 1; i < joins.size(); i++)
	{
		output += "j";
		output += ID;
	
		output += std::to_string(i);
		output += "->";
		output += "j";
		output += ID;
		output += std::to_string(count);
		output += ";\n"; 
		if ((i % (10)) == 0)
		{
			count++;
		}
	}

	return output; 
}


int JOIN_NODE::nextNodeToDraw()
{
	
	drawplace++; 
	return drawplace; 
	
}




template <typename... T>
void join_and_combine<T...>::connect_to_node(oneapi::tbb::flow::receiver<data_t>& port) {
	
	make_edge(combine, port);
	//combine.register_successor(port);
}

template <typename... T>
void join_and_combine<T...>::connect_to_combine(oneapi::tbb::flow::function_node<data_t, data_t>& node) {

	make_edge(combine, node);
}




template <typename ...T>
oneapi::tbb::flow::receiver<data_t> & join_and_combine<T...>::get_join_port(size_t& num) {
	auto & node = join;
	switch (num)
	{
	case 0:
		if constexpr (sizeof... (T) >= 1) {

			return std::get<0>(node.input_ports());
		}
		break;
	case 1:
		if constexpr (sizeof... (T) >= 2) {

			return std::get<1>(node.input_ports());
		}
		break;
	case 2:

		if constexpr (sizeof... (T) >= 3) {
			return  std::get<2>(node.input_ports());
		}
		break;
	case 3:
		if constexpr (sizeof... (T) >= 4) {
			return std::get<3>(node.input_ports());
		}
		break;
	case 4:
		if constexpr (sizeof... (T) >= 5) {
			return std::get<4>(node.input_ports());
		}
		break;
	case 5:
		if constexpr (sizeof... (T) >= 6) {
			return std::get<5>(node.input_ports());
		}
		break;
	case 6:
		if constexpr (sizeof... (T) >= 7) {
			return  std::get<6>(node.input_ports());
		}
		break;
	case 7:
		if constexpr (sizeof... (T) >= 8) {
			return std::get<7>(node.input_ports());
		}
		break;
	case 8:
		if constexpr (sizeof... (T) >= 9) {
			return std::get<8>(node.input_ports());
		}
		break;
	case 9:
		if constexpr (sizeof... (T) >= 10) {
			return std::get<9>(node.input_ports());
		}
		break;
	default:
		throw 7;
		return std::get<0>(node.input_ports());
	}
	throw 8; 
	return std::get<0>(node.input_ports());
}
