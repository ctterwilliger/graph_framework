#pragma once

oneapi::tbb::flow::function_node<data_t, data_t>
make_filter_node(oneapi::tbb::flow::graph& g, size_t  concurrency, bool f) {
	return oneapi::tbb::flow::function_node<data_t, data_t>(g, concurrency, [&](const data_t& data)
		{
			auto output = data;
			auto & [ID, data_ptr] = output;

			if (isValidID(ID))
			{
				if (f) {
					ID = -ID;
				}
			}
			return output;
		}
	);
}