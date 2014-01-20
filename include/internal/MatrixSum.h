#ifndef MATRIX_SUM_H
#define MATRIX_SUM_H

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
  typedef typename Eigen::CwiseBinaryOp
      <Eigen::internal::scalar_sum_op<scalar_type>,
       const typename Lhs::result_type, const typename Rhs::result_type>
          result_type;
};

template <typename Lhs, typename Rhs>
class MatrixSum : public MatrixBase<MatrixSum<Lhs, Rhs> > {
 public:
  typedef typename traits<MatrixSum<Lhs, Rhs> >::scalar_type scalar_type;
  typedef typename traits<MatrixSum<Lhs, Rhs> >::result_type result_type;

 private:
  const Lhs& lhs_;
  const Rhs& rhs_;

 public:
  // constructor
  MatrixSum(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() + rhs_.get(); }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* MATRIX_SUM_H */
