cmake_minimum_required (VERSION 2.6)

project (GaussianProcess)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CmakeModules/")

#dependencies
find_package(Eigen3 3.2.0 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
#INCLUDE(FindPkgConfig)
#PKG_CHECK_MODULES(Eigen3 "Eigen >= 3.2.0")
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})
#target_link_libraries(GaussianProcess ${GSL_LIBRARIES})

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

