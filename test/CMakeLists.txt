# CMakeList.txt : CMake project for graph_framework, include source and define
# project specific logic here.
include(CTest)
set(CMAKE_BUILD_TYPE DEBUG)
find_package(Catch2 REQUIRED)

add_executable("testing" test.cpp)
#target_link_libraries(foo Catch2 Catch2WithMain)
target_link_libraries("testing" PRIVATE Catch2::Catch2WithMain TBB::tbb)


#include(Catch)
#target_compile_definitions(foo PRIVATE CATCH_CONFIG_MAIN)

#catch_discover_tests(foo)