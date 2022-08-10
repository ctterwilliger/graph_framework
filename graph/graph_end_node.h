#pragma once
#include <iostream>



oneapi::tbb::flow::multifunction_node<data_t, std::tuple<data_t, data_t>>
make_end_of_graph_node(oneapi::tbb::flow::graph& g, oneapi::tbb::concurrent_vector<data_obj> & O, oneapi::tbb::concurrent_vector<data_obj>& T) {
	return oneapi::tbb::flow::multifunction_node<data_t, std::tuple<data_t, data_t>> (g, oneapi::tbb::flow::unlimited, 
		[&O,&T](const data_t& data, auto& outputs) {
			auto const & [ID, OutData] = data;
			
			if (isValidID(ID))
			{
				//std::cout << "Ouput: " << int1 << " " << int2 <<" " << ID << std::endl;
				O.push_back(OutData);
				//std::get<1>(outputs).try_put(data);
			}
			else
			{
				//std::cout << "Trash: " << int1 << " " << int2 <<" " << ID<< std::endl;
				T.push_back(OutData);
				//std::get<0>(outputs).try_put(data);
			}

	});
}






