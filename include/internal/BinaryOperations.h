#ifndef BINARY_OPERATIONS_H
#define BINARY_OPERATIONS_H

#include "ForwardDeclarations.h"
#include "OperationsBase.h"

#include <utility>
#include <type_traits>

namespace GP {
namespace internal {
namespace op {

// Sum
template <class Lhs, class Rhs>
struct Sum : base::BinaryCoeffWiseOperation<Lhs, Rhs> {
  typedef decltype(std::declval<typename Sum::lhs_type>() + std::declval
                   <typename Sum::rhs_type>()) result_type;
  static result_type apply(const typename Sum::lhs_type& lhs,
                           const typename Sum::rhs_type& rhs) {
    return lhs + rhs;
  }
};

// Difference
template <class Lhs, class Rhs>
struct Difference : base::BinaryCoeffWiseOperation<Lhs, Rhs> {
  typedef decltype(std::declval<typename Difference::lhs_type>() - std::declval
                   <typename Difference::rhs_type>()) result_type;
  static result_type apply(const typename Difference::lhs_type& lhs,
                           const typename Difference::rhs_type& rhs) {
    return lhs - rhs;
  }
};

// coeff-wise Product
template <class Lhs, class Rhs>
struct Product : base::SetScalarAndInputs<Lhs, Rhs> {
  // Scalar or double * Matrix -> MatrixBase
  // Scalar or double * Scalar -> ScalarBase
  template <class OtherDerived>
  using Parent = typename std::conditional<
      // if
      std::is_base_of<MatrixBase<Lhs>, Lhs>::value || std::is_base_of
      <MatrixBase<Rhs>, Rhs>::value,
      // then
      MatrixBase<OtherDerived>,
      // else
      ScalarBase<OtherDerived> >::type;

 private:
  typedef typename std::conditional
      <std::is_base_of<MatrixBase<Lhs>, Lhs>::value, Lhs, Rhs>::type MatrixSide;
  typedef typename std::conditional
      <!std::is_base_of<MatrixBase<Lhs>, Lhs>::value, Lhs, Rhs>::type
          ScalarSide;

 public:
  // the non-matrix must be 1x1
  static_assert(ScalarSide::ColsAtCompileTime == 1
                && ScalarSide::RowsAtCompileTime == 1,
                "One of Lhs or Rhs must be 1x1");
  enum {
    RowsAtCompileTime = MatrixSide::RowsAtCompileTime,
    ColsAtCompileTime = MatrixSide::ColsAtCompileTime
  };
  typedef decltype(std::declval<typename Product::lhs_type>() * std::declval
                   <typename Product::rhs_type>()) result_type;
  static result_type apply(const typename Product::lhs_type& lhs,
                           const typename Product::rhs_type& rhs) {
    return lhs * rhs;
  }
};

// matrix Product
template <class Lhs, class Rhs>
struct MatrixProduct : base::SetScalarAndInputs<Lhs, Rhs> {
  //parent is always MatrixBase
  template <class Derived> using Parent = MatrixBase<Derived>;
  // product is feasible
  static_assert(Lhs::ColsAtCompileTime == Eigen::Dynamic
                || Rhs::RowsAtCompileTime == Eigen::Dynamic
                || Lhs::ColsAtCompileTime == Rhs::RowsAtCompileTime,
                "Lhs and Rhs have incompatible shapes for a matrix product");
  enum {
    RowsAtCompileTime = Lhs::RowsAtCompileTime,
    ColsAtCompileTime = Rhs::ColsAtCompileTime
  };
  typedef decltype(std::declval<typename MatrixProduct::lhs_type>()
                   * std::declval
                   <typename MatrixProduct::rhs_type>()) result_type;
  static result_type apply(const typename MatrixProduct::lhs_type& lhs,
                           const typename MatrixProduct::rhs_type& rhs) {
    return lhs * rhs;
  }
};

// Quotient
template <class Lhs, class Rhs>
struct Quotient : base::SetScalarAndInputs<Lhs, Rhs> {
  // parent is MatrixBase if Lhs is matrix
  template <class OtherDerived>
  using Parent = typename std::conditional<
      // if
      std::is_base_of<MatrixBase<Lhs>, Lhs>::value,
      // then
      MatrixBase<OtherDerived>,
      // else
      ScalarBase<OtherDerived> >::type;
  // Rhs must be 1x1
  static_assert(Rhs::ColsAtCompileTime == 1 && Rhs::RowsAtCompileTime == 1,
                "Rhs must be 1x1");
  enum {
    RowsAtCompileTime = Lhs::RowsAtCompileTime,
    ColsAtCompileTime = Lhs::ColsAtCompileTime
  };

  typedef decltype(std::declval<typename Quotient::lhs_type>() / std::declval
                   <typename Quotient::rhs_type>()) result_type;
  static result_type apply(const typename Quotient::lhs_type& lhs,
                           const typename Quotient::rhs_type& rhs) {
    return lhs / rhs;
  }
};
}  // op
}  // internal
}  // GP
#endif /* BINARY_OPERATIONS_H */
