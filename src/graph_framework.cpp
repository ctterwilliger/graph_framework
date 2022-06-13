// graph_framework.cpp : Defines the entry point for the application.
//

#include "graph_framework.h"
#include "oneapi/tbb.h"
#include "../graph/framework.hpp"
#include "../graph/graph.h"
#include <tuple>
#include "../graph/type_config.h"
using namespace std;
using namespace oneapi::tbb;

int main()
{
	graph grph;
	grph.add_filter_node("1",true, flow::unlimited );
	cout << "Hello CMake." << endl;
	return 0;
}
