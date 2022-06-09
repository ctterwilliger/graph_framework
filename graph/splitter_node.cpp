#include "splitter_node.h"


template <typename Input, typename Outputs, typename Func>
auto make_splitter_node(tbb::flow::graph& g, size_t  concurrency, Func f) {
	return tbb::flow::multifunction_node<Input, Outputs>(g, concurrency, [](const Input & data, auto& Outputs) {
		auto const& [ptr, tupoTup] = data;
		auto& [tup1, tup2] = tupoTup;

		get<0>(outputs).try_put({ ptr, tup1 });
		get<1>(outputs).try_put({ ptr,tup2 });
		return outputs;
		});
}

