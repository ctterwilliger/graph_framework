#pragma once
#include "oneapi/tbb.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include "type_config.h"
#include <iostream>


oneapi::tbb::flow::input_node<data_t>
make_start_node(oneapi::tbb::flow::graph & g){
	return oneapi::tbb::flow::input_node<data_t>(g, [count = 0](oneapi::tbb::flow_control& fc) mutable
		{
		if (count  == 9)
		{
			fc.stop();
		}
		count++;
			data_t  output; 
			auto& [ID, data] = output; 
			std::shared_ptr<std::tuple<int, int>> temp(new std::tuple<int, int>); 
			data = temp; 
			auto& [int1, int2] = *data;
			std::cout << count + 10; 
			int1 = count;
			int2 = -count; 
			return output; 
		}
	);
}
