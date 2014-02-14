#ifndef INTERNAL_MATRIX_BUILTIN_PRODUCT_H
#define INTERNAL_MATRIX_BUILTIN_PRODUCT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>
#include <utility>

namespace GP {
namespace internal {

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinProduct : public MatrixBase
                             <MatrixBuiltinProduct<MatrixExpression> > {
 public:
  // typedefs
  typedef typename MatrixExpression::scalar_type scalar_type;
  typedef decltype(2. * std::declval
                   <typename MatrixExpression::result_type>()) result_type;
  enum {
    RowsAtCompileTime = MatrixExpression::RowsAtCompileTime,
    ColsAtCompileTime = MatrixExpression::ColsAtCompileTime
  };
 private:
  double lhs_;
  MatrixExpression rhs_;
  struct Data {
      double lhs_;
      typename MatrixExpression::result_type rhs_;
      result_type val_;
      Data(double lhs, const typename MatrixExpression::result_type& rhs) :
          lhs_(lhs), rhs_(rhs), val_(lhs_*rhs_) {}
  };
  mutable std::shared_ptr<Data> data_;

 public:
  // constructor
  MatrixBuiltinProduct(double lhs, const MatrixExpression& rhs)
      : lhs_(lhs), rhs_(rhs), data_(nullptr) {}

  // actual computation
  const result_type& get() const {
      data_ = std::make_shared<Data>(lhs_,rhs_.get());
      return data_->val_;
  }

  unsigned get_version() const { return rhs_.get_version(); }
};
}
}
#endif /* INTERNAL_MATRIX_BUILTIN_PRODUCT_H */
