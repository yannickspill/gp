#ifndef INTERNAL_SCALAR_MATRIX_DIFFERENCE_H
#define INTERNAL_SCALAR_MATRIX_DIFFERENCE_H

#include "macros.h"
#include "internal/ScalarBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Scal, class Mat>
struct traits<ScalarMatrixDifference<Scal, Mat> > {
  static_assert(std::is_same
                <typename Scal::scalar_type, typename Mat::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Scal::scalar_type scalar_type;
  typedef scalar_type result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
};

template <typename Scal, typename Mat>
class ScalarMatrixDifference : public ScalarBase
                               <ScalarMatrixDifference<Scal, Mat> > {
  public:
  // typedefs
  typedef typename traits<ScalarMatrixDifference>::scalar_type scalar_type;
  typedef typename traits<ScalarMatrixDifference>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<ScalarMatrixDifference>::RowsAtCompileTime,
    ColsAtCompileTime = traits<ScalarMatrixDifference>::ColsAtCompileTime
  };

 private:
  Scal lhs_;
  Mat rhs_;

 public:
  // constructor
  ScalarMatrixDifference(const Scal& lhs, const Mat& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const {
    typename Mat::result_type mat(rhs_.get());
    assert(mat.rows() == mat.cols() && mat.rows() == 1
           && "Matrix + Scalar only works on a 1x1 matrix!");
    return lhs_.get() - mat(0, 0);
  }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* INTERNAL_SCALAR_MATRIX_DIFFERENCE_H */
