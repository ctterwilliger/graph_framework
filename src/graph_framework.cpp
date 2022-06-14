// graph_framework.cpp : Defines the entry point for the application.
//

#include "graph_framework.h"
#include "oneapi/tbb.h"
#include "../graph/framework.hpp"
#include "../graph/graph.h"
#include <tuple>

using namespace std;
using namespace oneapi::tbb;


// FOR TESTING PURPOSES ONLY!!!!!!!!!!!!!!!!
int A =0;
int B =0; 
int main()
{
	graph grph;
	grph.add_filter_node("1", flow::unlimited, true );
	grph.add_proccess_node("2", flow::unlimited, []() {
		A = B + 1; 
		});
	grph.print_nodes();
	grph.add_edge("1", "2"); 
	grph.print_edges(); 
	grph.refresh_graph(); 
	return 0;
}
