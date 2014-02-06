#ifndef INTERNAL_MATRIX_BUILTIN_DIFFERENCE_H
#define INTERNAL_MATRIX_BUILTIN_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>

namespace GP {
namespace internal {

// specialize traits for MatrixBuiltinDifference
template <class MatrixExpression>
struct traits<MatrixBuiltinDifference<MatrixExpression> > {
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef scalar_type result_type;
  enum {
      RowsAtCompileTime = 1,
      ColsAtCompileTime = 1
  };
};

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinDifference : public ScalarBase
                             <MatrixBuiltinDifference<MatrixExpression> > {
 public:
  // typedefs
  typedef typename traits<MatrixBuiltinDifference>::scalar_type scalar_type;
  typedef typename traits<MatrixBuiltinDifference>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<MatrixBuiltinDifference>::RowsAtCompileTime,
    ColsAtCompileTime = traits<MatrixBuiltinDifference>::ColsAtCompileTime
  };



 private:
  MatrixExpression lhs_;
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
