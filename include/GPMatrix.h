#ifndef GPMATRIX_H
#define GPMATRIX_H

#include "macros.h"

#include <Eigen/Dense>

// declare traits class
template <class Derived> struct traits;

//! Base class for any matrix
template <class Derived>
class GPMatrixBase {

 public:
  // typedefs
  typedef typename traits<Derived>::result_type result_type;
  static const unsigned RowsAtCompileTime = result_type::RowsAtCompileTime;
  static const unsigned ColsAtCompileTime = result_type::ColsAtCompileTime;

 public:
  // allow casting to Derived
  operator Derived&() { return static_cast<Derived&>(*this); }
  operator const Derived&() { return static_cast<const Derived&>(*this); }
};

//! Use this to represent any constant or Scalar-dependent matrix/vector
template <class EigenType>
class GPMatrix : public GPMatrixBase<GPMatrix<EigenType> > {

 private:
  EigenType data_;

 public:
  typedef typename GPMatrixBase<GPMatrix>::result_type result_type;

 public:
  //! Construct directly from Eigen expression
  GPMatrix(const EigenType& data) : data_(data) {}

  //! Construct from GP matrix expression, convert if needed
  template <class OtherDerived>
  explicit GPMatrix(const GPMatrixBase<OtherDerived>& expr)
      : data_(expr.eigen()) {}

  //! Return bare Eigen type
  // Use with precaution as this loses track of any dependent Scalars.
  EigenType eigen() const { return data_; }
};

// specialize traits for GPMatrix
template <class EigenType>
struct traits<GPMatrix<EigenType> > {
  typedef EigenType result_type;
};

//! \addtogroup Common matrix typedefs @{
typedef GPMatrix<Eigen::MatrixXd> GPMatrixXd;
typedef GPMatrix<Eigen::VectorXd> GPVectorXd;
//! @}

#endif /* GPMATRIX_H */
