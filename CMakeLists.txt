cmake_minimum_required (VERSION 2.6)

project (GaussianProcess)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DLOGGING)
endif()

include_directories(src bin)

add_library(MVN
    src/MultivariateFNormalSufficient.h src/MultivariateFNormalSufficient.cpp)

#test MVN
add_executable(TestMVN bin/test_MultivariateFNormalSufficient.cpp)
target_link_libraries(TestMVN MVN)

#MVN benchmark
add_executable(MVNBench bin/bench_mvn.cpp)
target_link_libraries(MVNBench MVN)

#debugging w/ or w/o valgrind
enable_testing()
set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")
set(CTEST_MEMORYCHECK_COMMAND_OPTIONS, "--trace-children=yes --leak-check=full")
add_test(BasicTest TestMVN)
add_test(BenchTest MVNBench data/toy.dat data/fitting_protocol2.dat)

