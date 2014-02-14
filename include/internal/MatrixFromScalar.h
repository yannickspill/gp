#ifndef INTERNAL_MATRIX_FROM_SCALAR_H
#define INTERNAL_MATRIX_FROM_SCALAR_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <memory>
#include <Eigen/Dense>

namespace GP {
namespace internal {

//!Build a matrix by putting ScalarExpr in every coefficient
//\note the derived class is only used for its nested Rows and ColsAtCompileTime
template <class ScalarExpr, class Derived>
class MatrixFromScalar : public MatrixBase
                         <MatrixFromScalar<ScalarExpr, Derived> > {

 public:
  typedef typename ScalarExpr::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = Derived::RowsAtCompileTime,
    ColsAtCompileTime = Derived::ColsAtCompileTime
  };
  typedef typename Eigen::Matrix
      <scalar_type, RowsAtCompileTime, ColsAtCompileTime>::ConstantReturnType
          result_type;

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

  unsigned get_version() const { return scal_.get_version(); }
};
}
}
#endif /* INTERNAL_MATRIX_FROM_SCALAR_H */
