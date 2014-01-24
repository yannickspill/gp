#ifndef INTERNAL_MATRIX_BUILTIN_DIFFERENCE_H
#define INTERNAL_MATRIX_BUILTIN_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>
#include <utility>

namespace GP {
namespace internal {

// specialize traits for MatrixBuiltinDifference
template <class MatrixExpression>
struct traits<MatrixBuiltinDifference<MatrixExpression> > {
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef scalar_type result_type;
};

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinDifference : public ScalarBase
                             <MatrixBuiltinDifference<MatrixExpression> > {
 public:
  typedef typename traits
      <MatrixBuiltinDifference<MatrixExpression> >::scalar_type scalar_type;
  typedef typename traits
      <MatrixBuiltinDifference<MatrixExpression> >::result_type result_type;

 private:
  const MatrixExpression& lhs_;
  double rhs_;

 public:
  // constructor
  MatrixBuiltinDifference(const MatrixExpression& lhs, double rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const {
      typename MatrixExpression::result_type mat(lhs_.get());
      assert(mat.rows() == mat.cols() && mat.rows() == 1
                  && "Matrix + Scalar only works on a 1x1 matrix!");
      return mat(0,0) - rhs_;
  }

  unsigned get_version() const { return lhs_.get_version(); }
};
}
}
#endif /* INTERNAL_MATRIX_BUILTIN_DIFFERENCE_H */
