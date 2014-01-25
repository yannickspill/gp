#ifndef INTERNAL_MATRIX_SUM_H
#define INTERNAL_MATRIX_SUM_H

#include "macros.h"
#include "internal/MatrixBase.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

template <class Lhs, class Rhs> struct traits<MatrixSum<Lhs, Rhs> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef decltype(std::declval<typename Lhs::result_type>() + std::declval
                   <typename Rhs::result_type>()) result_type;
};

template <typename Lhs, typename Rhs>
class MatrixSum : public MatrixBase<MatrixSum<Lhs, Rhs> > {
 public:
  typedef typename traits<MatrixSum<Lhs, Rhs> >::scalar_type scalar_type;
  typedef typename traits<MatrixSum<Lhs, Rhs> >::result_type result_type;

 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  // constructor
  MatrixSum(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {
      std::cout << "Construct MatrixSum" << std::endl;
  }

  // actual computation
  result_type get() const { 
      std::cout << "Get MatrixSum" << std::endl;
      return lhs_.get() + rhs_.get(); }

  unsigned get_version() const {
      std::cout << "get MatrixSum version" << std::endl;
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* INTERNAL_MATRIX_SUM_H */
