#ifndef INTERNAL_MATRIX_MATRIX_PRODUCT_H
#define INTERNAL_MATRIX_MATRIX_PRODUCT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>
#include <utility>

namespace GP {
namespace internal {

namespace {
template<class Lhs, class Rhs>
using ProductParent = typename std::conditional<
    Lhs::RowsAtCompileTime == 1 && Rhs::ColsAtCompileTime == 1,
    ScalarBase<MatrixMatrixProduct<Lhs, Rhs> >,
    MatrixBase<MatrixMatrixProduct<Lhs, Rhs> > >::type;
}

template <class Lhs, class Rhs> struct traits<MatrixMatrixProduct<Lhs, Rhs> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  static_assert(Lhs::ColsAtCompileTime == Eigen::Dynamic
                || Rhs::RowsAtCompileTime == Eigen::Dynamic
                || Lhs::ColsAtCompileTime == Rhs::RowsAtCompileTime,
                "Lhs and Rhs have incompatible shapes for a matrix product");
  typedef typename Lhs::scalar_type scalar_type;
  typedef decltype(std::declval<typename Lhs::result_type>() * std::declval
                   <typename Rhs::result_type>()) result_type;
  enum {
    RowsAtCompileTime = Lhs::RowsAtCompileTime,
    ColsAtCompileTime = Rhs::ColsAtCompileTime
  };
};

template <typename Lhs, typename Rhs>
class MatrixMatrixProduct : public ProductParent<Lhs, Rhs> {

 public:
  // typedefs
  typedef typename traits<MatrixMatrixProduct>::scalar_type scalar_type;
  typedef typename traits<MatrixMatrixProduct>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<MatrixMatrixProduct>::RowsAtCompileTime,
    ColsAtCompileTime = traits<MatrixMatrixProduct>::ColsAtCompileTime
  };

 private:
  Lhs lhs_;
  Rhs rhs_;
  mutable std::shared_ptr<result_type> ret_;

 public:
  // constructor
  MatrixMatrixProduct(const Lhs& lhs, const Rhs& rhs)
      : lhs_(lhs), rhs_(rhs), ret_(nullptr) {}

  // actual computation
  const result_type& get() const {
    ret_ = std::make_shared<result_type>(lhs_.get() * rhs_.get());
    return *ret_;
  }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_MATRIX_PRODUCT_H */
