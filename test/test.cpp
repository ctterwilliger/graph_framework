//#include "../graph/framework.hpp"

//#define NOMINMAX

#include "../graph/framework.hpp"
#include "test_contruction.h"
#include "filter_testing.h"
#include "testMath.h"
//#include "nodeTesting.h"
//#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>



//TODOOOOO
// 



// does not test quality of connections, only makes sure there is no crash
// CONTRUCTION TESTING
TEST_CASE("contruction testing" "[contruction]") {
	CHECK(zeroNodes());
	CHECK(oneNode());
	CHECK(basicGraph());
	CHECK(multiEoGs()); 
	CHECK(complexGraph());
	CHECK(largeJoin());
}



//FLAG TESTING
TEST_CASE("Flag Testing" "[Flags]")
{
	CHECK(testSingleFilter()); 
	CHECK(testFilterAndJoin());
	CHECK(testEvenFilter()); 
	CHECK(testPrimeFilter()); 
	CHECK(testMultipleFiltersAndOutputs());
}


TEST_CASE("MATH Testing" "[MATH]")
{
	CHECK(testPrimeSieve());
}
//

//TEST_CASE("Deducing Node" "[Deduce Node]")
//{
//	CHECK(testNoReturnKeyFilter());
//}

//JOIN NODE TESTING


// Calculation Testing
