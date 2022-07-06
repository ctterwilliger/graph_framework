#pragma once
#include "oneapi/tbb.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include "type_config.h"
#include <iostream>


oneapi::tbb::flow::input_node<data_t>
make_start_node(oneapi::tbb::flow::graph & g, std::vector<base_data> F) {
	return oneapi::tbb::flow::input_node<data_t>(g, [count = 0, F](oneapi::tbb::flow_control& fc) mutable
		{
		/*std::cout << "test"; 

		if (count == F.size()-1)
		{
			fc.stop(); 
		}
		
		
		data_t output;
		std::shared_ptr<base_data> out;
		auto  temp = F.at(count);
		*out = temp; 


		auto& [ID, data] = output;

		data = out;

		ID = count;
		count++;
		return output; */

		if (count == 9)
		{
			fc.stop();
		}
		count++;
		data_t  output;
		auto& [ID, data] = output;
		std::shared_ptr<std::tuple<int, int>> temp(new std::tuple<int, int>);
		data = temp;
		ID = count;
		auto& [int1, int2] = *data;
		int1 = count;
		int2 = -count;
		return output;
		}
	);
}
