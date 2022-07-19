# CMakeList.txt : CMake project for graph_framework, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_BUILD_TYPE RelwithDebugInfo)
find_package(TBB REQUIRED)
add_library(graph INTERFACE
	"framework.cpp" 
	"framework.hpp"
	
	"filter_node.cpp" 
	 
	
	"graph_end_node.cpp"
	"start_node.cpp" 
	"proccessor_node.cpp"
	 "type_config.h"
	 "combine_node.cpp"
	 "combine_node.h"
	 "start_node.h" 
	 "node.cpp")


# Add source to this project's executable.
target_link_libraries(graph INTERFACE TBB:tbb graph_framework)
# TODO: Add tests and install targets if needed.
