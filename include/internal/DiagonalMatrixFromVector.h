#ifndef INTERNAL_DIAGONAL_MATRIX_FROM_VECTOR_H
#define INTERNAL_DIAGONAL_MATRIX_FROM_VECTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>
#include <utility>

namespace GP {
namespace internal {

// Build a matrix by putting ScalarExpr in every coefficient
template <class VecExpr>
class DiagonalMatrixFromVector : public MatrixBase
                                 <DiagonalMatrixFromVector<VecExpr> > {

 private:
  VecExpr vec_;

 public:
  typedef typename VecExpr::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = VecExpr::RowsAtCompileTime,
    ColsAtCompileTime = VecExpr::RowsAtCompileTime
  };
  typedef decltype(vec_.get().asDiagonal()) result_type;

 public:
  explicit DiagonalMatrixFromVector(const VecExpr& vec) : vec_(vec) {}

  const result_type get() const { return vec_.get().asDiagonal(); }
  result_type get() { return vec_.get().asDiagonal(); }

  unsigned get_version() const { return vec_.get_version(); }
};
}
}
#endif /* INTERNAL_DIAGONAL_MATRIX_FROM_VECTOR_H */
