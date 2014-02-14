#ifndef INTERNAL_MATRIX_SCALAR_QUOTIENT_H
#define INTERNAL_MATRIX_SCALAR_QUOTIENT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace GP {
namespace internal {

template <typename Mat, typename Scal>
class MatrixScalarQuotient : public MatrixBase
                             <MatrixScalarQuotient<Mat, Scal> > {
  public:
  // typedefs
  static_assert(std::is_same
                <typename Mat::scalar_type, typename Scal::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Mat::scalar_type scalar_type;
  typedef decltype(std::declval<typename Mat::result_type>() / std::declval
                   <typename Scal::result_type>()) result_type;
    enum {
    RowsAtCompileTime = Mat::RowsAtCompileTime,
    ColsAtCompileTime = Mat::ColsAtCompileTime
  };

 private:
  Mat lhs_;
  Scal rhs_;

 public:
  // constructor
  MatrixScalarQuotient(const Mat& lhs, const Scal& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() / rhs_.get(); }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_SCALAR_QUOTIENT_H */
