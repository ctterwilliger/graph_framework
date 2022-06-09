# CMakeList.txt : CMake project for graph_framework, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("graph_framework")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(TBB REQUIRED)
add_subdirectory("graph")
add_subdirectory(src)
# Add source to this project's executable.
add_executable ("graph_framework"  

 "src/graph_framework.cpp" "src/graph_framework.h"
 #"graph/combine_node.cpp" "graph/combine_node.h"
 #"graph/filter_node.cpp" "graph/fliter_node.h" "graph/framework.cpp" 
 #"graph/framework.hpp" 
 #"graph/graph_end_node.cpp" "graph/graph_end_node.h" "graph/multi_node.cpp" "graph/multi_node.h"
 #"graph/proccessor_node.cpp" "graph/proccessor_node.h"
 #"graph/split_node.cpp" "graph/split_node.h" "graph /start_node.cpp" "graph /start_node.h"
 )
target_link_libraries(graph_framework PRIVATE TBB::tbb)
# TODO: Add tests and install targets if needed.
