#ifndef INTERNAL_MATRIX_BUILTIN_QUOTIENT_H
#define INTERNAL_MATRIX_BUILTIN_QUOTIENT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>

namespace GP {
namespace internal {

// specialize traits for MatrixBuiltinQuotient
template <class MatrixExpression>
struct traits<MatrixBuiltinQuotient<MatrixExpression> > {
  typedef typename MatrixExpression::scalar_type scalar_type;
 typedef decltype(
      std::declval<typename MatrixExpression::result_type>()/2.) result_type;
};


// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinQuotient : public MatrixBase
                             <MatrixBuiltinQuotient<MatrixExpression> > {
 public:
  typedef typename traits
      <MatrixBuiltinQuotient<MatrixExpression> >::scalar_type scalar_type;
  typedef typename traits
      <MatrixBuiltinQuotient<MatrixExpression> >::result_type result_type;

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
