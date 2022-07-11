#pragma once
#include "oneapi/tbb.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include "type_config.h"

template<typename Func>
oneapi::tbb::flow::function_node<data_t, data_t>
make_proccess_node(oneapi::tbb::flow::graph& g, size_t  concurrency, Func  f) {
	return oneapi::tbb::flow::function_node<data_t, data_t> (g, concurrency, [&](const data_t& data) 
		{
			auto const & [ID, data_ptr] = data;

			if (isValidID(ID))
			{
				f(data);
			}
			return data;
		}
	);
}


