# CMakeList.txt : CMake project for graph_framework, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("graph_framework")

#sets the standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE RelwithDebugInfo)

#requires TBB to be installed
find_package(TBB REQUIRED)

add_subdirectory("graph")
add_subdirectory(src)
add_subdirectory("test")



# Add source to exceutable 
add_executable ("graph_framework"  
 "src/graph_framework.cpp" "src/graph_framework.h"
 )


target_link_libraries(graph_framework PRIVATE TBB::tbb)
# TODO: Add tests and install targets if needed.

#add_subdirectory(test)