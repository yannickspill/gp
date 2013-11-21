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

include_directories(include src bench test)

#build libraries
add_library(Multivariate
    include/MultivariateFNormalSufficient.h src/MultivariateFNormalSufficient.cpp)

#test subject/observer
#add_executable(TestObserver test/test_Observer.cpp)
#test Scalar
add_executable(TestScalar test/test_Scalar.cpp)
#test Linear1DFunction
add_executable(TestLinear1DFunction test/test_Linear1DFunction.cpp)
#test integration of Linear1D with epsilon
add_executable(TestEpsilonMeanIntegration
    test/test_epsilon_mean_integration.cpp)
#test Covariance1DFunction
add_executable(TestCovariance1DFunction test/test_Covariance1DFunction.cpp)
#test VectorFromFunction
add_executable(TestVectorFromFunction
    test/test_VectorFromFunction.cpp)
#test SymmetricMatrixFromFunction
add_executable(TestSymmetricMatrixFromFunction
    test/test_SymmetricMatrixFromFunction.cpp)
#test PosteriorCovarianceMatrix
add_executable(TestPosteriorCovarianceMatrix
    test/test_posterior_covariance_matrix.cpp)
#test LDLT
add_executable(TestLDLT test/test_LDLT.cpp)
#test old MVN
add_executable(TestMultivariate test/test_MultivariateFNormalSufficient.cpp)
target_link_libraries(TestMultivariate Multivariate)
#test basic MVN
add_executable(TestBasicMVN test/test_BasicMVN.cpp)
#test new MVN
add_executable(TestMVN test/test_MVN.cpp)

#old MVN benchmark
add_executable(BenchMultivariate bench/bench_old_mvn.cpp)
target_link_libraries(BenchMultivariate Multivariate)

#basic MVN benchmark
add_executable(BenchBasicMVN bench/bench_basic_mvn.cpp)
#new MVN benchmark
add_executable(BenchMVN bench/bench_mvn.cpp)

#debugging w/ or w/o valgrind
enable_testing()
#set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")
#set(CTEST_MEMORYCHECK_COMMAND_OPTIONS, "--trace-children=yes --leak-check=full")
add_test(BasicTest TestMultivariate TestLDLT TestPosteriorCovarianceMatrix
    TestPriorCovarianceMatrix TestLinearEpsilon TestLinear1DFunction TestEpsilon
    TestScalar)
add_test(TestOldMVN TestMultivariate)
add_test(TestMVN TestMVN)
#add_test(BenchTest MVNBench "${CMAKE_SOURCE_DIR}/data/toy.dat" "${CMAKE_SOURCE_DIR}/data/fitting_protocol2.dat")

