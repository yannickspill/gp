#ifndef INTERNAL_MATRIX_BUILTIN_QUOTIENT_H
#define INTERNAL_MATRIX_BUILTIN_QUOTIENT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>
#include <utility>

namespace GP {
namespace internal {

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinQuotient : public MatrixBase
                             <MatrixBuiltinQuotient<MatrixExpression> > {
 public:
  // typedefs
  typedef typename MatrixExpression::scalar_type scalar_type;
 typedef decltype(
      std::declval<typename MatrixExpression::result_type>()/2.) result_type;
   enum {
    RowsAtCompileTime = MatrixExpression::RowsAtCompileTime,
    ColsAtCompileTime = MatrixExpression::ColsAtCompileTime
  };
 private:
  MatrixExpression lhs_;
  double rhs_;

 public:
  // constructor
  MatrixBuiltinQuotient(const MatrixExpression& lhs, double rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() / rhs_; }

  unsigned get_version() const { return lhs_.get_version(); }
};
}
}
#endif /* INTERNAL_MATRIX_BUILTIN_QUOTIENT_H */
