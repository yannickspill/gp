#ifndef INTERNAL_MATRIX_SCALAR_SUM_H
#define INTERNAL_MATRIX_SCALAR_SUM_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>
#include <assert.h>

namespace GP {
namespace internal {

template <class Mat, class Scal>
struct traits<MatrixScalarSum<Mat, Scal> > {
  static_assert(std::is_same
                <typename Mat::scalar_type, typename Scal::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Mat::scalar_type scalar_type;
  typedef scalar_type result_type;
};

// type(matrix + scalar) = type(scalar)
// will check if matrix is 1x1 at runtime
template <typename Mat, typename Scal>
class MatrixScalarSum : public ScalarBase<MatrixScalarSum<Mat, Scal> > {
 public:
  // typedefs
  typedef typename traits<MatrixScalarSum>::scalar_type scalar_type;
  typedef typename traits<MatrixScalarSum>::result_type result_type;

 private:
  Mat lhs_;
  Scal rhs_;

 public:
  // constructor
  MatrixScalarSum(const Mat& lhs, const Scal& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const {
      typename Mat::result_type mat(lhs_.get());
      assert(mat.rows() == mat.cols() && mat.rows() == 1
                  && "Matrix + Scalar only works on a 1x1 matrix!");
      return mat(0,0) + rhs_.get();
  }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_SCALAR_SUM_H */
