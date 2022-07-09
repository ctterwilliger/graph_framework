//#include "../graph/framework.hpp"

//#define NOMINMAX

#include "../graph/framework.hpp"
#include "test_contruction.h"

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

}



//JOIN NODE TESTING


// Calculation Testing
