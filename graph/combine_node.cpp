#include "combine_node.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include "type_config.h"
#include <math.h>
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
		size_t numOfCompleteLayers = log_baseB(numOfJoins, MAXNUMOFJOINS);
		for (int i = 0; i < numOfCompleteLayers; i++)
		{
			for (int j = 0; j < pow(MAXNUMOFJOINS, i); j++)
			{
				add_node();
			}
		}
		size_t curNumOfJoins = pow(MAXNUMOFJOINS, numOfCompleteLayers);

		while (curNumOfJoins <= numOfJoins - MAXNUMOFJOINS)
		{
			add_node(); 
			curNumOfJoins += MAXNUMOFJOINS - 1; 
		}
		add_node(numOfJoins - curNumOfJoins + 1);
	}
	drawplace = curPort; 
}



void JOIN_NODE::add_node()
{
	joins.push_back(make_shared<joinNode10>(joinNode10(g)));
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
		auto& lastNode = joins.back();
		lastNode->connect_to_node(nextPort()); 
	}
	
}


oneapi::tbb::flow::receiver<data_t> & JOIN_NODE::nextPort(){
	auto & node = joins.at(curNode);
	auto & port = node->get_join_port(curPort);
	curPort++;
	if (curPort % MAXNUMOFJOINS == 0) {
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
		output += ID;
		output += "-";
		output += std::to_string(count);
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