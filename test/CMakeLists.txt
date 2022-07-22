# CMakeList.txt : CMake project for graph_framework, include source and define
# project specific logic here.
set (CMAKE_BUILD_TYPE RelwithDebInfo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable("testing" test.cpp)

target_compile_definitions("testing" PRIVATE CATCH_CONFIG_MAIN)


include(CTest)
find_package(Catch2 REQUIRED)
find_package (TBB REQUIRED)
#add_library("tests" "filter_testing.cpp" "filter_testing.h" 
	#						"test_contruction.cpp" "test_contruction.h")
#target_link_libraries("tests" INTERFACE TBB::tbb)
target_link_libraries("testing" PRIVATE TBB::tbb Catch2::Catch2 )
#target_link_libraries("testing" INTERFACE TBB:tbb graph_framework)





#include(Catch)
#target_compile_definitions(foo PRIVATE CATCH_CONFIG_MAIN)

#catch_discover_tests(foo)