#ifndef INTERNAL_SCALAR_FROM_MATRIX_H
#define INTERNAL_SCALAR_FROM_MATRIX_H

#include "internal/ForwardDeclarations.h"
#include "internal/ScalarBase.h"

#include <memory>
#include <Eigen/Core>

namespace GP {
namespace internal {

// non-modifiable Scalar, used as wrapper to pass 1x1 matrices around using
// implicit conversion
template<class Derived>
class ScalarFromMatrix : public ScalarBase<ScalarFromMatrix<Derived> > {
 public:
  static_assert(Derived::RowsAtCompileTime == Eigen::Dynamic ||
                Derived::RowsAtCompileTime == 1,
                "Matrix should have one row");
  static_assert(Derived::ColsAtCompileTime == Eigen::Dynamic ||
                Derived::ColsAtCompileTime == 1,
                "Matrix should have one column");
  // typedefs
  typedef double scalar_type;
  typedef double result_type;  // passed as value in get()
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };

 private:
   Derived mat_;
   mutable double val_;

 public:
  ScalarFromMatrix(const MatrixBase<Derived>& mat) : mat_(mat.asDerived()) {}

  // pass as value because it is a double
  result_type get() const {
      auto mat = mat_.get();
      CHECK(mat.rows() == 1 && mat.cols() == 1, "Matrix should be 1x1!");
      val_ = mat_.get()(0, 0);
      return val_;
  }

  unsigned get_version() const { return mat_.get_version(); }

  bool operator==(const ScalarFromMatrix& other) const {
    return mat_ == other.mat_;
  }
  bool operator!=(const ScalarFromMatrix& other) const {
    return !(*this == other);
  }
};
}
}
#endif /* INTERNAL_SCALAR_FROM_MATRIX_H */
