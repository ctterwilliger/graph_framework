// graph_framework.cpp : Defines the entry point for the application.
//

#include "graph_framework.h"
#include "oneapi/tbb.h"
#include "../graph/framework.hpp"
#include <tuple>
using namespace std;
using namespace oneapi::tbb;
using data_b = std::tuple<int>;
using data_t = std::tuple<data_ptr, data_b>;
using data_pt = std::tuple<data_ptr, std::tuple<data_b, data_b>>;
int main()
{

	oneapi::tbb::flow::graph g;




	auto spliter_node = make_splitter_node<data_pt, std::tuple<data_t, data_t>>(g, flow::unlimited, [](const data_pt& data, auto& outputs) {
		auto const& [ptr, tupoTup] = data;
		auto& [tup1, tup2] = tupoTup;

		get<0>(outputs).try_put({ ptr, tup1 });
		get<1>(outputs).try_put({ ptr,tup2 });
		return outputs;
		}
	);



	cout << "Hello CMake." << endl;
	return 0;
}
