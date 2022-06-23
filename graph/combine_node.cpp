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
}



void JOIN_NODE::add_node()
{
	auto node = make_shared<join_base>(joinNode10(g));
	joins.push_back(node);
	make_edge(node->get_combine(), nextPort());

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
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t>(g)));
		break; 
	case 3: 
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t,data_t>(g)));
		break; 
	case 4:
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t, data_t,data_t>(g)));
		break;
	case 5:
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t, data_t,data_t,data_t>(g)));
		break;
	case 6:
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t, data_t, data_t, data_t,data_t>(g)));
		break;
	case 7:
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t,data_t>(g)));
		break;
	case 8:
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t,data_t>(g)));
		break;
	case 9:
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t,data_t>(g)));
		break;
	case 10:
		joins.push_back(make_shared<join_base>(join_and_combine<data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t, data_t,data_t>(g)));
		break;
	}
	make_edge(joins.at(joins.size() - 1) ->get_combine(), nextPort());
}


oneapi::tbb::flow::receiver<data_t> & JOIN_NODE::nextPort(){
	auto & node = joins.at(curNode);
	auto & port = node->get_join(curPort);

	
	curPort++;
	if (curPort % MAXNUMOFJOINS == 0) {
		curPort = 0;
		curNode++; 
	}
	return port; 
}

auto JOIN_NODE::EndNode() {
	return joins.at(0);
}

JOIN_NODE::~JOIN_NODE() {

}
