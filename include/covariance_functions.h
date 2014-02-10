#ifndef COVARIANCE_FUNCTIONS_H
#define COVARIANCE_FUNCTIONS_H

#include "Scalar.h"
#include "Matrix.h"
#include "Functor.h"
#include "macros.h"

#include <Eigen/Core>
#include <Eigen/Dense>

namespace GP {
namespace covariance {

///\defgroup CovFunctions Covariance Functions
///@{

/// \defgroup SECov Squared exponential
///@{

/**General case, takes a covariance matrix as input
 * \f[k(\mathbf{x}, \mathbf{y}; \mathbf{M}) =
 * \exp\left( -\frac{1}{2} (\mathbf{x}-\mathbf{y})^T \mathbf{M}
 * (\mathbf{x}-\mathbf{y}) \right) \f]
 */
template <class SymMat>
auto squared_exponential(const SymMat& M)
    -> internal::Functor
    <typename std::remove_const<decltype(
         (-0.5 * (std::declval
                  <GP::Matrix<Eigen::Matrix<typename SymMat::scalar_type,
                                            SymMat::RowsAtCompileTime, 1> > >()
                  - std::declval
                  <GP::Matrix<Eigen::Matrix<typename SymMat::scalar_type,
                                            SymMat::RowsAtCompileTime, 1> > >())
                     .transpose() * M
          *(std::declval
            <GP::Matrix<Eigen::Matrix<typename SymMat::scalar_type,
                                      SymMat::RowsAtCompileTime, 1> > >()
            - std::declval
            <GP::Matrix<Eigen::Matrix<typename SymMat::scalar_type,
                                      SymMat::RowsAtCompileTime, 1> > >()))
             .exp())>::type,
     GP::Matrix<Eigen::Matrix
                <typename SymMat::scalar_type, SymMat::RowsAtCompileTime, 1> >,
     GP::Matrix<Eigen::Matrix<typename SymMat::scalar_type,
                              SymMat::RowsAtCompileTime, 1> > > {
  typedef Eigen::Matrix
      <typename SymMat::scalar_type, SymMat::RowsAtCompileTime, 1> in_base_type;
  typedef Matrix<in_base_type> in_type;
  in_type x(in_base_type(M.rows()));
  in_type y(in_base_type(M.rows()));
  return GP::internal::make_functor(
      (-0.5 * (x - y).transpose() * M * (x - y)).exp(), x, y);
}

/** Simplfied case in which the input is the persistence length and the
 * dimension inputs of the functor should be vectors, even in the 1D case
 * \f[k(\mathbf{x}, \mathbf{y}; \lambda) =
 * \exp\left( -\frac{\|\mathbf{x}-\mathbf{y}\|^2}{2\lambda^2} \right) \f]
 */
template <int DimsAtCompileTime = Eigen::Dynamic>
auto squared_exponential(Scalar len, unsigned dims)
    -> internal::Functor
    <typename std::remove_const<decltype((
         -0.5
         * (std::declval<GP::Matrix<Eigen::Matrix<typename Scalar::scalar_type,
                                                  DimsAtCompileTime, 1> > >()
            - std::declval
            <GP::Matrix<Eigen::Matrix<typename Scalar::scalar_type,
                                      DimsAtCompileTime, 1> > >()).transpose()
         * GP::Matrix
           <Eigen::Matrix<typename Scalar::scalar_type, DimsAtCompileTime,
                          1> >::Broadcast(1. / (len* len), dims, 1).asDiagonal()
         * (std::declval<GP::Matrix<Eigen::Matrix<typename Scalar::scalar_type,
                                                  DimsAtCompileTime, 1> > >()
            - std::declval
            <GP::Matrix<Eigen::Matrix<typename Scalar::scalar_type,
                                      DimsAtCompileTime, 1> > >()))
                                             .exp())>::type,
     GP::Matrix
     <Eigen::Matrix<typename Scalar::scalar_type, DimsAtCompileTime, 1> >,
     GP::Matrix
     <Eigen::Matrix<typename Scalar::scalar_type, DimsAtCompileTime, 1> > > {
  assert(dims>0);
  typedef Eigen::Matrix
      <typename Scalar::scalar_type, DimsAtCompileTime, 1> in_base_type;
  typedef Matrix<in_base_type> in_type;
  in_type x((in_base_type(dims)));
  in_type y((in_base_type(dims)));
  auto M = in_type::Broadcast(1. / (len * len), dims, 1).asDiagonal();
  return GP::internal::make_functor(
      (-0.5 * (x - y).transpose() * M * (x - y)).exp(), x, y);
}

///simplified constructor for 1-D squared exponential
///\f[k(x, y; \lambda) =\exp\left( -\frac{(x-y)^2}{2\lambda^2} \right) \f]
auto squared_exponential(Scalar len)
    -> internal::Functor
    <decltype((-0.5 * SQUARE((std::declval<Scalar>() - std::declval<Scalar>())
                             / len)).exp()),
     Scalar, Scalar> {
    Scalar x(1.0),y(1.0);
    return GP::internal::make_functor((-0.5*SQUARE((x-y)/len)).exp(), x, y);
}
///@}

///@}
}
}
#endif /* COVARIANCE_FUNCTIONS_H */
