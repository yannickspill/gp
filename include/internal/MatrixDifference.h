#ifndef MATRIX_DIFFERENCE_H
#define MATRIX_DIFFERENCE_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

namespace GP {
namespace internal {

// specialize traits for MatrixDifference
template <class Lhs, class Rhs> struct traits<MatrixDifference<Lhs, Rhs> > {
  static_assert(std::is_same
                <typename Lhs::scalar_type, typename Rhs::scalar_type>::value,
                "cannot mix matrices of different scalar types");
  typedef typename Lhs::scalar_type scalar_type;
  typedef const typename Eigen::CwiseBinaryOp
      <Eigen::internal::scalar_difference_op<scalar_type>,
       const typename Lhs::result_type, const typename Rhs::result_type>
          result_type;
  typedef branch_tag node_type;
};

template <typename Lhs, typename Rhs>
class MatrixDifference : public MatrixBase<MatrixDifference<Lhs, Rhs> > {
 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  typedef typename traits<MatrixDifference<Lhs, Rhs> >::scalar_type scalar_type;
  typedef typename traits<MatrixDifference<Lhs, Rhs> >::result_type result_type;
  typedef typename traits<MatrixDifference<Lhs, Rhs> >::node_type node_type;

 public:
  // constructor
  MatrixDifference(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  result_type get() const { return lhs_.get() - rhs_.get(); }

  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* MATRIX_DIFFERENCE_H */
