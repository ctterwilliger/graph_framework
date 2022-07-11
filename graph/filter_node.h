#pragma once
#include <iostream>
template<typename FT>
oneapi::tbb::flow::function_node<data_t, data_t>
make_filter_node(oneapi::tbb::flow::graph& g, size_t  concurrency, FT f) {
	return oneapi::tbb::flow::function_node<data_t, data_t>(g, concurrency, [&](const data_t& data)
		{
			auto output = data;
			auto & [ID, data_ptr] = output;

			if (isValidID(ID))
			{
				if (f(data)) {
					//std::cout << "make id neg" << std::endl; 
					ID = -ID;
				}
			}
			return output;
		}
	);
}