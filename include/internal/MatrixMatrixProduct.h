#ifndef INTERNAL_MATRIX_MATRIX_PRODUCT_H
#define INTERNAL_MATRIX_MATRIX_PRODUCT_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>

namespace GP {
namespace internal {

template <class Lhs, class Rhs> struct traits<MatrixMatrixProduct<Lhs, Rhs> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef decltype(std::declval<typename Lhs::result_type>() * std::declval
                   <typename Rhs::result_type>()) result_type;
};

template <typename Lhs, typename Rhs>
class MatrixMatrixProduct : public MatrixBase<MatrixMatrixProduct<Lhs, Rhs> > {

 public:
  typedef typename traits
      <MatrixMatrixProduct<Lhs, Rhs> >::scalar_type scalar_type;
  typedef typename traits
      <MatrixMatrixProduct<Lhs, Rhs> >::result_type result_type;

 private:
  Lhs lhs_;
  Rhs rhs_;
  mutable std::shared_ptr<result_type> ret_;

 public:
  // constructor
  MatrixMatrixProduct(const Lhs& lhs, const Rhs& rhs)
      : lhs_(lhs), rhs_(rhs), ret_(nullptr) {}

  MatrixMatrixProduct(Lhs&& lhs, Rhs&& rhs)
      : lhs_(lhs), rhs_(rhs), ret_(nullptr) {
          std::cout << "second called" << std::endl;
      }

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
