#pragma once

#include <cstddef>
#include <memory>
#include <tuple>
#include <iostream>
#include "type_config.h"
#include "oneapi/tbb.h"
oneapi::tbb::flow::input_node<data_t>
make_start_node(oneapi::tbb::flow::graph& g, std::vector<base_data> & F) {
	return oneapi::tbb::flow::input_node<data_t>(g, [count = 0, F](oneapi::tbb::flow_control& fc) mutable
	{
		

		if (count == F.size())
		{
			fc.stop();
		}
		data_t output(new data_obj);
		if (count < F.size())
		{
			
			auto temp = F.at(count);
			


			auto& [ID, data] = *output;

			data = temp;

			ID = count;
		}
		count++;
		return output; 

		/*if (count == 1)
		{
			fc.stop();
		}
		count++;
	

		data_t output (new data_obj);
		auto & [ID, data] = *output;
		auto& [int1, int2] = data;
	
		ID = count;
		
		int1 = count;
		int2 = -count;
		return output;*/
	}
	);
}
