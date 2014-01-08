#ifndef GPI_H
#define GPI_H

#include "SymmetricMatrixFromFunction.h"
#include "VectorFromFunction.h"
#include "Scalar.h"
#include "ConstEigenObject.h"
#include "MVN.h"

#include <Eigen/Dense>
//! Gaussian Process Interpolation factory
/** This class provides methods to perform bayesian interpolation/smoothing of
 * data using a gaussian process prior on the function to be interpolated.
 * Inputs are the dataset and the types of the prior mean and covariance
 * functions.
 * \tparam MeanFunctionType the prior mean function type to use.
 * \tparam CovarianceFunctionType prior covariance function type.
 * \tparam InputMatrixType Eigen type of the input matrix
 * \tparam InputVectorType Eigen type of the input vector
 * \tparam d The number of dimensions of the inputs (independent variables).
 *  You can use Eigen::Dynamic if unknown.
 */
template<class MeanFunctionType, class CovarianceFunctionType,
         class InputMatrixType, class InputVectorType,
         unsigned d = Eigen::Dynamic>
class GPI {

    //compile-time checks
    static_assert(d == Eigen::Dynamic
                  || d == MeanFunctionType::input_dimensions,
                  "MeanFunctionType has the wrong number of input dimensions");
    static_assert(d == Eigen::Dynamic
                  || d == CovarianceFunctionType::input_dimensions,
                  "CovarianceFunctionType has the wrong number of input dimensions");
    static_assert(MeanFunctionType::output_dimensions == 1,
                  "MeanFunctionType should have an output dimension of 1");
    static_assert(CovarianceFunctionType::output_dimensions == 1,
                  "CovarianceFunctionType should have an output dimension of 1");
    static_assert(InputMatrixType::ColsAtCompileTime == Eigen::Dynamic
                  || InputMatrixType::ColsAtCompileTime == d,
                  "InputMatrixType has the wrong number of columns");
    static_assert(
        InputVectorType::RowsAtCompileTime == Eigen::Dynamic
        || InputMatrixType::RowsAtCompileTime == Eigen::Dynamic
        || InputMatrixType::RowsAtCompileTime == InputMatrixType::RowsAtCompileTime,
        "InputVectorType is incompatible with InputMatrixType");

  public:
    /** Constructor for the gaussian process
     * \param [in] X a \f$M\times d\f$ matrix of coordinates (e.g. independent
     * variables, inputs). Each row is a \f$d\f$-dimensional input,
     * corresponding to the abscissa of each of the \f$M\f$ observations.
     * \param [in] sample_mean \f$I\f$ vector of mean observations (e.g.
     * dependent variables) at each of the previously defined coordinates
     * \param [in] sample_std \f$s\f$ vector of sample standard deviations
     * \param [in] n_obs \f$N\f$ sample size for mean and std
     */
    GPI(InputMatrixType X, InputVectorType sample_mean,
        InputVectorType sample_std, unsigned n_obs);

};

#endif /* GPI_H */
