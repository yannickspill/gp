#ifndef GPMATRIX_H
#define GPMATRIX_H

#include "macros.h"

#include <Eigen/Dense>
#include <iostream>
#include <memory>

// declare traits class
template<typename Derived> struct traits;

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
  typedef typename traits<GPMatrix<EigenType> >::result_type result_type;

 public:
  //! Construct directly from Eigen expression
  GPMatrix(const EigenType& data) : data_(data) {}

  //! Construct from GP matrix expression, convert if needed
  template <class OtherDerived>
  GPMatrix(const GPMatrixBase<OtherDerived>& expr)
      : data_(expr.eigen()) {}

  //! Return bare Eigen type
  // Use with precaution as this loses track of any dependent Scalars.
  EigenType eigen() const { return data_; }
};

// specialize traits for GPMatrix
template<typename EigenType>
struct traits<GPMatrix<EigenType> > {
  typedef EigenType result_type;
};

//! \addtogroup Matrix sum, difference, product and division templates @{
template <typename Lhs, typename Rhs>
class GPMatrixSum : public GPMatrixBase<GPMatrixSum<Lhs, Rhs> > {
 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  typedef decltype(lhs_+rhs_) result_type;

 public:
  GPMatrixSum(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}
  result_type eigen() const { return lhs_ + rhs_; }
};
// specialize traits for GPMatrixSum
template<typename Lhs, typename Rhs>
struct traits<GPMatrixSum<Lhs, Rhs> > {
  typedef typename GPMatrixSum<Lhs, Rhs>::result_type result_type;
};

//
template <typename Lhs, typename Rhs>
class GPMatrixDifference : public GPMatrixBase<GPMatrixDifference<Lhs, Rhs> > {
 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  typedef decltype(lhs_ - rhs_) result_type;

 public:
  GPMatrixDifference(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}
  result_type eigen() const { return lhs_ - rhs_; }
};
//
template <typename Lhs, typename Rhs>
class GPMatrixProduct : public GPMatrixBase<GPMatrixProduct<Lhs, Rhs> > {
 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  typedef decltype(lhs_* rhs_) result_type;

 public:
  GPMatrixProduct(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}
  result_type eigen() const { return lhs_ * rhs_; }
};
//
template <typename Lhs, typename Rhs>
class GPMatrixDivision : public GPMatrixBase<GPMatrixDivision<Lhs, Rhs> > {
 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  typedef decltype(lhs_* rhs_) result_type;

 public:
  GPMatrixDivision(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}
  result_type eigen() const { return lhs_ / rhs_; }
};
//! @}

//! \addtogroup Matrix sum, difference, product and division operators @{
template <typename Lhs, typename Rhs>
const GPMatrixSum<Lhs, Rhs> operator+(const GPMatrixBase<Lhs>& lhs,
                                      const GPMatrixBase<Rhs>& rhs) {
  return GPMatrixSum<Lhs, Rhs>(lhs, rhs);
}
//
template <typename Lhs, typename Rhs>
const GPMatrixDifference<Lhs, Rhs> operator-(const GPMatrixBase<Lhs>& lhs,
                                             const GPMatrixBase<Rhs>& rhs) {
  return GPMatrixDifference<Lhs, Rhs>(lhs, rhs);
}
//
template <typename Lhs, typename Rhs>
const GPMatrixProduct<Lhs, Rhs> operator*(const GPMatrixBase<Lhs>& lhs,
                                          const GPMatrixBase<Rhs>& rhs) {
  return GPMatrixProduct<Lhs, Rhs>(lhs, rhs);
}
//
template <typename Lhs, typename Rhs>
const GPMatrixDivision<Lhs, Rhs> operator/(const GPMatrixBase<Lhs>& lhs,
                                           const GPMatrixBase<Rhs>& rhs) {
  return GPMatrixDivision<Lhs, Rhs>(lhs, rhs);
}
//! @}

//! \addtogroup Common matrix typedefs @{
typedef GPMatrix<Eigen::MatrixXd> GPMatrixXd;
typedef GPMatrix<Eigen::VectorXd> GPVectorXd;
//! @}

#endif /* GPMATRIX_H */
