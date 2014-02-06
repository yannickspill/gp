#ifndef INTERNAL_MATRIX_BUILTIN_SUM_H
#define INTERNAL_MATRIX_BUILTIN_SUM_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>

namespace GP {
namespace internal {

// specialize traits for MatrixBuiltinSum
template <class MatrixExpression>
struct traits<MatrixBuiltinSum<MatrixExpression> > {
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef scalar_type result_type;
  static_assert(MatrixExpression::RowsAtCompileTime == Eigen::Dynamic
                || MatrixExpression::RowsAtCompileTime == 1,
                "Input matrix must have one row!");
  static_assert(MatrixExpression::ColsAtCompileTime == Eigen::Dynamic
                || MatrixExpression::ColsAtCompileTime == 1,
                "Input matrix must have one column!");
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
};

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinSum : public ScalarBase
                             <MatrixBuiltinSum<MatrixExpression> > {
 public:
  // typedefs
  typedef typename traits<MatrixBuiltinSum>::scalar_type scalar_type;
  typedef typename traits<MatrixBuiltinSum>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<MatrixBuiltinSum>::RowsAtCompileTime,
    ColsAtCompileTime = traits<MatrixBuiltinSum>::ColsAtCompileTime
  };

 private:
  MatrixExpression lhs_;
  double rhs_;

 public:
  // constructor
  MatrixBuiltinSum(const MatrixExpression& lhs, double rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const {
      typename MatrixExpression::result_type mat(lhs_.get());
      assert(mat.rows() == mat.cols() && mat.rows() == 1
                  && "Matrix + Scalar only works on a 1x1 matrix!");
      return mat(0,0) + rhs_;
  }

  unsigned get_version() const { return lhs_.get_version(); }
};
}
}
#endif /* INTERNAL_MATRIX_BUILTIN_SUM_H */
