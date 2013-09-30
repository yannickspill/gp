cmake_minimum_required (VERSION 2.6)

project (GaussianProcess)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DLOGGING)
endif()

add_library(MVN
    MultivariateFNormalSufficient.h MultivariateFNormalSufficient.cpp)

#test MVN
add_executable(TestMVN test_MultivariateFNormalSufficient.cpp)
target_link_libraries(TestMVN MVN)

