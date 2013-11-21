cmake_minimum_required (VERSION 2.6)

project (GaussianProcess)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CmakeModules/")
ADD_DEFINITIONS("-std=c++0x") 

#dependencies
find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})
find_package(Eigen3 3.2.0 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DLOGGING)
endif()

include_directories(include)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bench)


