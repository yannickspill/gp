#ifndef INTERNAL_BUILTIN_MATRIX_DIFFERENCE_H
#define INTERNAL_BUILTIN_MATRIX_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>

namespace GP {
namespace internal {

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class BuiltinMatrixDifference : public ScalarBase
                                <BuiltinMatrixDifference<MatrixExpression> > {
 public:
  // typedefs
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef scalar_type result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
 private:
  double lhs_;
  MatrixExpression rhs_;

 public:
  // constructor
  BuiltinMatrixDifference(double lhs, const MatrixExpression& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const {
    typename MatrixExpression::result_type mat(rhs_.get());
    assert(mat.rows() == mat.cols() && mat.rows() == 1
           && "Matrix + Scalar only works on a 1x1 matrix!");
    return lhs_ - mat(0, 0);
  }

  unsigned get_version() const { return rhs_.get_version(); }
};
}
}
#endif /* INTERNAL_BUILTIN_MATRIX_DIFFERENCE_H */
