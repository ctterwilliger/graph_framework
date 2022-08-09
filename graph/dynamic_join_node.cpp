#include "dynamic_join_node.h"
#include "combine_node.h"
#include "type_config.h"
#include <tuple>
#include <math.h>
#include <iostream>
#include <oneapi/tbb.h>

//join of size 10, used to clean up code
using joinNode10 = join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t>;


// used to determine complete layers
int log_baseB(size_t num, size_t base)
{
	return std::log(num) / std::log(base);
}

// takes a number for size of join node
JOIN_NODE::JOIN_NODE(size_t numOfJoins, oneapi::tbb::flow::graph& graph) :g(graph)
{
	// count for portNum and nodeNum
	curPort = 0;
	curNode = 0;

	//if the join node is of size <= MAXNUMOFJOINS(currently set to 10)
	if (numOfJoins <= MAXNUMOFJOINS)
	{

		add_node(numOfJoins);
	}
	else
	{
		//creates the base node(node 0)
		add_node();

		//counts the number of complete layers()
		size_t numOfCompleteLayers = log_baseB(numOfJoins, MAXNUMOFJOINS);

		//creates the number of complete nodes layer by layer
		for (int i = 1; i < numOfCompleteLayers; i++)
		{
			for (int j = 0; j < pow(MAXNUMOFJOINS, i); j++)
			{
				add_node(MAXNUMOFJOINS);
			}
		}

		// calculates the current num of availible ports
		size_t curNumOfJoins = pow(MAXNUMOFJOINS, numOfCompleteLayers);

		// if the join node would not be complete(most of the time)
		if (curNumOfJoins != numOfJoins)
		{
			//adds a join node to the next avaible port, until there is the amount of required ports 
			while (curNumOfJoins < (numOfJoins - MAXNUMOFJOINS))
			{
				add_node(MAXNUMOFJOINS);

				//adds MaxNumOfJoins - 1 to current num of availble ports(one was used to add another join node)
				curNumOfJoins += (MAXNUMOFJOINS - 1);
			}

			//adds the last join node
			add_node(numOfJoins - curNumOfJoins + 1);
		}

	}
	drawplace = curPort + curNode * 10 - 1;
}



//this adds a join node to of size 10 to
void JOIN_NODE::add_node()
{
	//adds to join
	joins.push_back(std::make_shared<joinNode10>(g));

	//connects curNode to next avaible port
	auto& curNode = joins.at(joins.size() - 1);
	if (joins.size() != 1) {

		curNode->connect_to_node(nextPort());
	}
}


//adds join node of size 2-10
void JOIN_NODE::add_node(size_t  nodeSize)
{

	switch (nodeSize) {
	default:
		throw 1; //not caught
	case 1:
		throw 1; //not caught 
		break;
	case 2:

		joins.push_back(std::make_shared<join_and_combine<data_t, data_t>>(g));
		break;
	case 3:
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t>>(g));
		break;
	case 4:
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t>>(g));
		break;
	case 5:
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t>>(g));
		break;
	case 6:
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t>>(g));
		break;
	case 7:
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t>>(g));
		break;
	case 8:
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t>>(g));
		break;
	case 9:
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t>>(g));
		break;
	case 10:
		joins.push_back(std::make_shared<join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t>>(g));
		break;
	}

	//if this is not the first node, connect to next avaible port
	if (joins.size() != 1) {
		auto& lastNode = joins.at(joins.size() - 1);
		lastNode->connect_to_node(nextPort());
	}

}

// returns the next port
//used to contruct combine_node, and to connect to it
oneapi::tbb::flow::receiver<data_t>& JOIN_NODE::nextPort() {
	auto& node = joins.at(curNode);
	auto& port = node->get_join_port(curPort);

	curPort++;
	if ((curPort % MAXNUMOFJOINS) == 0) {
		curPort = 0;
		curNode++;
	}
	return port;
}

//returns Node 0
std::shared_ptr<join_base>& JOIN_NODE::EndNode() {
	return joins.at(0);
}

//defualt deconstructor
JOIN_NODE::~JOIN_NODE() {

}


// used to create a 
std::string JOIN_NODE::listJoins(std::string ID)
{
	std::string output = "";
	for (int count = 0; count < joins.size(); count++)
	{
		output += "j";
		output += ID;

		output += std::to_string(count);
		output += "[lable =\"";
		output += ID + "join" + std::to_string(count);
		output += +"\"];\n";
	}
	return output;
}

std::string JOIN_NODE::innerJoins(std::string ID)
{
	std::string output = "j" + ID + "0 -> " + ID + ";\n";
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