#ifndef INTERNAL_MATRIX_SCALAR_PRODUCT_H
#define INTERNAL_MATRIX_SCALAR_PRODUCT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace GP {
namespace internal {

template <typename Mat, typename Scal>
class MatrixScalarProduct : public MatrixBase<MatrixScalarProduct<Mat, Scal> > {
 public:
  // typedefs
  static_assert(std::is_same
                <typename Mat::scalar_type, typename Scal::scalar_type>::value,
                "cannot mix different scalar types");
  typedef typename Mat::scalar_type scalar_type;
  typedef decltype(std::declval<typename Mat::result_type>() * std::declval
                   <typename Scal::result_type>()) result_type;
  enum {
    RowsAtCompileTime = Mat::RowsAtCompileTime,
    ColsAtCompileTime = Mat::ColsAtCompileTime
  };
 private:
  Mat lhs_;
  Scal rhs_;
  mutable std::shared_ptr<result_type> ret_;

 public:
  // constructor
  MatrixScalarProduct(const Mat& lhs, const Scal& rhs)
      : lhs_(lhs), rhs_(rhs), ret_(nullptr) {}

  // actual computation
  result_type get() const {
      ret_ = std::make_shared<result_type>(lhs_.get()*rhs_.get());
      return *ret_;
  }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_SCALAR_PRODUCT_H */
