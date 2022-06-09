#pragma once
#include "oneapi/tbb.h"




//template <typename Input, typename Outputs, typename Func>
//auto make_splitter_node(tbb::flow::graph& g, size_t  concurrency, Func f);

template <typename Input, typename Outputs, typename Func>
auto make_splitter_node(tbb::flow::graph& g, size_t  concurrency, Func f) {
	return tbb::flow::multifunction_node<Input, Outputs>(g, concurrency, f);
		
}