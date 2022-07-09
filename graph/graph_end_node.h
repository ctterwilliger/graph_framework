#pragma once
#include <iostream>



oneapi::tbb::flow::multifunction_node<data_t, std::tuple<data_t, data_t>>
make_end_of_graph_node(oneapi::tbb::flow::graph& g, std::vector<base_data> & O, std::vector<base_data>& T) {
	return oneapi::tbb::flow::multifunction_node<data_t, std::tuple<data_t, data_t>> (g, oneapi::tbb::flow::unlimited, 
		[&O,&T](const data_t& data, auto& outputs) {
			auto const & [ID, OutData] = *data;
			auto const& [int1, int2] = OutData; 
			if (isValidID(ID))
			{
				//std::cout << "Ouput: " << int1 << " " << int2 <<" " << ID << std::endl;
				O.push_back(OutData);
				std::get<1>(outputs).try_put(data);
			}
			else
			{
				//std::cout << "Trash: " << int1 << " " << int2 <<" " << ID<< std::endl;
				T.push_back(OutData);
				std::get<0>(outputs).try_put(data);
			}

	});
}
//
//flow::multifunction_node<data_pt, std::tuple<std::tuple<data_b, data_b>, std::tuple<data_b, data_b>>> end_graph_node(g, flow::unlimited, [&](const data_pt& data, auto& outputs) {
//	std::tuple<data_b, data_b> output;
//
//	auto& [ptr, tupoTup] = data;
//	auto& [tup1, tup2] = output;
//	auto& [data1, data2] = tupoTup;
//	tup1 = data1;
//	tup2 = data2;
//
//	if (*ptr)
//	{
//		get<1>(outputs).try_put(output);
//	}
//	else
//	{
//		get<0>(outputs).try_put(output);
//	}