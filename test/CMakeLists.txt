# CMakeList.txt : CMake project for graph_framework, include source and define
# project specific logic here.
set (CMAKE_BUILD_TYPE RelwithDebInfo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable("testing" test.cpp  "filter_testing.cpp" "filter_testing.h")

target_compile_definitions("testing" PRIVATE CATCH_CONFIG_MAIN)


include(CTest)
find_package(Catch2 REQUIRED)


target_link_libraries("testing" PRIVATE TBB::tbb Catch2::Catch2)






#include(Catch)
#target_compile_definitions(foo PRIVATE CATCH_CONFIG_MAIN)

#catch_discover_tests(foo)