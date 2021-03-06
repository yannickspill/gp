cmake_minimum_required (VERSION 2.6)

project (GaussianProcess)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    ADD_DEFINITIONS("-std=c++0x") 
    ADD_DEFINITIONS("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    ADD_DEFINITIONS("-std=gnu++0x") 
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    ADD_DEFINITIONS("-std=c++11") 
else("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    ADD_DEFINITIONS("-std=c++0x") 
endif()

#dependencies
find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})
find_package(Eigen3 3.2.0 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})
#find_package(GSL REQUIRED)
#include_directories(${GSL_INCLUDE_DIRS})

#build type, add coverage on debug mode
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DLOGGING)
    add_definitions(-DCHECKS)
    #include(CodeCoverage)
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1")
    #SETUP_TARGET_FOR_COVERAGE(coverage ninja coverage test )
endif()

#include dirs
include_directories(include)
include_directories(include/internal)
include_directories(include/test_traits)

#testing
enable_testing()

#build dirs
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bench)

#doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

