#ifndef INTERNAL_MATRIX_BUILTIN_PRODUCT_H
#define INTERNAL_MATRIX_BUILTIN_PRODUCT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>
#include <utility>

namespace GP {
namespace internal {

// specialize traits for MatrixBuiltinProduct
template <class MatrixExpression>
struct traits<MatrixBuiltinProduct<MatrixExpression> > {
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef typename std::remove_const
      <decltype(2. * std::declval
                <typename MatrixExpression::result_type>())>::type result_type;
};

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinProduct : public MatrixBase
                             <MatrixBuiltinProduct<MatrixExpression> > {
 public:
  typedef typename traits
      <MatrixBuiltinProduct<MatrixExpression> >::scalar_type scalar_type;
  typedef typename traits
      <MatrixBuiltinProduct<MatrixExpression> >::result_type result_type;

 private:
  double lhs_;
  const MatrixExpression& rhs_;

 public:
  // constructor
  MatrixBuiltinProduct(double lhs, const MatrixExpression& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_ * rhs_.get(); }

  unsigned get_version() const { return rhs_.get_version(); }
};
}
}
#endif /* INTERNAL_MATRIX_BUILTIN_PRODUCT_H */
