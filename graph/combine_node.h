#pragma once


//oneapi::tbb::flow::function_node<data_t, data_t>
//make_filter_node(oneapi::tbb::flow::graph& g, size_t  concurrency, bool f) {
//	return oneapi::tbb::flow::function_node<data_t, data_t>(g, concurrency, [&](const data_t& data)
//		{
//			auto output = data;
//			auto & [ID, data_ptr] = output;
//
//			if (isValidID(ID))
//			{
//				if (f) {
//					ID = -ID;
//				}
//			}
//			return output;
//		}
//	);
//}

oneapi::tbb::flow::join_node<std::tuple<data_t, data_t>, oneapi::tbb::flow::tag_matching>
make_join_node(oneapi::tbb::flow::graph& g)
{
	return oneapi::tbb::flow::join_node<std::tuple<data_t, data_t>, oneapi::tbb::flow::tag_matching>(g,
		[](const data_t& data) {
			return getSizeT_ID(data);

		},
		[](const data_t& data) {
			return getSizeT_ID(data);

		});
}

oneapi::tbb::flow::function_node<std::tuple<data_t, data_t>, data_t>
make_combine_node(oneapi::tbb::flow::graph& g) {
	return oneapi::tbb::flow::function_node< std::tuple <data_t, data_t>, data_t>(g, oneapi::tbb::flow::unlimited,
		[&](const std::tuple<data_t, data_t>& inData) {
			data_t output;
			auto& [outID, outData] = output;
			auto const& [inDataT1, inDataT2] = inData;
			auto const& [inID1, indata1] = inDataT1;
			auto const& [inID2, indata2] = inDataT2;

			if (!(isEquivID(inID1, inID2)))
			{
				throw 1; 
			}
			
			outID = inID1;
			outData = indata1; 
			
			return output;
		}); 
}

//// repacks ptr tuple(data_sub1) and ptr tuple(tuple_sub2) into prt and tuple(data)
//flow::function_node< tuple<data_t, data_t>, data_pt> join_combine(g, flow::unlimited, [&](const tuple<data_t, data_t>& data) {
//	data_pt output;
//	auto const& [data1, data2] = data;
//	auto const& [ptr1, base1] = data1;
//	auto const& [ptr2, base2] = data2;
//	auto& [ptr, tupoTup] = output;
//	auto& [tup1, tup2] = tupoTup;
//
//
//	//in case pointers are not equivlent we have somesort of error
//	// not handled yet
//	if (ptr1 != ptr2)
//	{
//		throw 1; // can be changed
//	}
//ptr = ptr1;
//tup1 = base1;
//tup2 = base2;
//return output;
//});