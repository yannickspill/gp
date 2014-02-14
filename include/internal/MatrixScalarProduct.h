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
  struct Data {
      typename Mat::result_type lhs_;
      typename Scal::result_type rhs_;
      result_type val_;
      Data(const typename Mat::result_type& lhs,
           const typename Scal::result_type& rhs)
          : lhs_(lhs), rhs_(rhs), val_(lhs_ * rhs_) {}
  };
  mutable std::shared_ptr<Data> data_;

 public:
  // constructor
  MatrixScalarProduct(const Mat& lhs, const Scal& rhs)
      : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const {
      data_ = std::make_shared<Data>(lhs_.get(),rhs_.get());
      return data_->val_;
  }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_SCALAR_PRODUCT_H */
