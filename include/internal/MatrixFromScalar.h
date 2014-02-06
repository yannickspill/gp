#ifndef INTERNAL_MATRIX_FROM_SCALAR_H
#define INTERNAL_MATRIX_FROM_SCALAR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

template <class ScalarExpr, int CRows, int CCols>
struct traits<MatrixFromScalar<ScalarExpr, CRows, CCols> > {
  typedef typename ScalarExpr::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = CRows,
    ColsAtCompileTime = CCols
  };
  typedef typename Eigen::Matrix
      <scalar_type, CRows, CCols>::ConstantReturnType result_type;
};

//Build a matrix by putting ScalarExpr in every coefficient
template <class ScalarExpr, int CRows, int CCols>
class MatrixFromScalar
    : public MatrixBase
      <MatrixFromScalar<ScalarExpr, CRows, CCols> > {

 public:
  typedef typename traits<MatrixFromScalar>::scalar_type scalar_type;
  typedef typename traits<MatrixFromScalar>::result_type result_type;
  enum {
      RowsAtCompileTime=traits<MatrixFromScalar>::RowsAtCompileTime,
      ColsAtCompileTime=traits<MatrixFromScalar>::ColsAtCompileTime,
  };

 private:
  ScalarExpr scal_;
  unsigned nrows_, ncols_;

 public:
  explicit MatrixFromScalar(const ScalarExpr& scal, unsigned nrows,
                            unsigned ncols)
      : scal_(scal), nrows_(nrows), ncols_(ncols) {}

  const result_type get() const {
    return result_type(nrows_, ncols_, scal_.get());
  }
  result_type get() { return result_type(nrows_, ncols_, scal_.get()); }

  unsigned get_version() const {
      return scal_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_FROM_SCALAR_H */
