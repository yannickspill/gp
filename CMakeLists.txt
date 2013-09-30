cmake_minimum_required (VERSION 2.6)

project (GaussianProcess)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DLOGGING)
endif()

include_directories(src test)

add_library(MVN
    src/MultivariateFNormalSufficient.h src/MultivariateFNormalSufficient.cpp)

#test MVN
add_executable(TestMVN test/test_MultivariateFNormalSufficient.cpp)
target_link_libraries(TestMVN MVN)

