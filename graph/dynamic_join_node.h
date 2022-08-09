#pragma once
#include "combine_node.h"
#include "oneapi/tbb.h"
#include "type_config.h"

class JOIN_NODE {
public:
	JOIN_NODE(size_t numOfJoins, oneapi::tbb::flow::graph& graph);
	~JOIN_NODE();
	oneapi::tbb::flow::receiver<data_t>& nextPort();
	std::shared_ptr<join_base>& EndNode();
	std::string listJoins(std::string ID);
	std::string innerJoins(std::string ID);

	int nextNodeToDraw();
private:
	std::vector<std::shared_ptr<join_base>> joins;
	size_t curPort;
	size_t curNode;
	const size_t MAXNUMOFJOINS = 10;
	void add_node();
	void add_node(size_t node_size);
	int drawplace;
	oneapi::tbb::flow::graph& g;
};
#include "dynamic_join_node.cpp"