#ifndef INTERNAL_DIAGONAL_MATRIX_FROM_VECTOR_H
#define INTERNAL_DIAGONAL_MATRIX_FROM_VECTOR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>
#include <utility>

namespace GP {
namespace internal {

template <class VecExpr> struct traits<DiagonalMatrixFromVector<VecExpr> > {
  typedef typename VecExpr::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = VecExpr::result_type::RowsAtCompileTime,
    ColsAtCompileTime = VecExpr::result_type::RowsAtCompileTime
  };
  typedef const decltype(std::declval
                   <typename VecExpr::result_type>().asDiagonal()) result_type;
};

// specialize for matrix, which needs to be constd
// TODO: avoid this, and write const and non-const versions of operators
// and get() everywhere
template <class EigenType>
struct traits<DiagonalMatrixFromVector<Matrix<EigenType> > > {
  typedef typename Matrix<EigenType>::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = EigenType::RowsAtCompileTime,
    ColsAtCompileTime = EigenType::RowsAtCompileTime
  };
  typedef decltype(std::declval<const EigenType>().asDiagonal()) result_type;
};

// Build a matrix by putting ScalarExpr in every coefficient
template <class VecExpr>
class DiagonalMatrixFromVector : public MatrixBase
                                 <DiagonalMatrixFromVector<VecExpr> > {

 public:
  typedef typename traits<DiagonalMatrixFromVector>::scalar_type scalar_type;
  typedef typename traits<DiagonalMatrixFromVector>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<DiagonalMatrixFromVector>::RowsAtCompileTime,
    ColsAtCompileTime = traits<DiagonalMatrixFromVector>::ColsAtCompileTime,
  };

 private:
  VecExpr vec_;

 public:
  explicit DiagonalMatrixFromVector(const VecExpr& vec) : vec_(vec) {}

  result_type get() { return vec_.get().asDiagonal(); }

  unsigned get_version() const { return vec_.get_version(); }
};
}
}
#endif /* INTERNAL_DIAGONAL_MATRIX_FROM_VECTOR_H */
