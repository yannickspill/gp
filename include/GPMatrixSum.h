#ifndef GPMATRIX_SUM_H
#define GPMATRIX_SUM_H

#include "macros.h"
#include "GPMatrixBase.h"

#include <Eigen/Core>

// specialize traits for GPMatrixSum
template <typename Lhs, typename Rhs>
struct traits<GPMatrixSum<Lhs, Rhs> > {
  typedef typename Eigen::CwiseBinaryOp<Eigen::internal::scalar_sum_op<double>,
                                        Lhs, Rhs> result_type;
  typedef typename result_type::Scalar scalar_type;
};

//! \addtogroup Matrix sum, difference, product and division templates @{
template <typename Lhs, typename Rhs>
class GPMatrixSum : public GPMatrixBase<GPMatrixSum<Lhs, Rhs> > {
 private:
  Lhs lhs_;
  Rhs rhs_;

 public:
  typedef typename GPMatrixBase<GPMatrixSum>::result_type result_type;

 public:
  // constructor
  GPMatrixSum(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

  // actual computation
  const result_type eigen() const { return lhs_.eigen() + rhs_.eigen(); }

  /*
  template <class Left, class Right>
  friend const GPMatrixSum<Left, Right> operator+(
      const GPMatrixBase<Left>& lhs, const GPMatrixBase<Right>& rhs) {
    return GPMatrixSum<Left, Right>(lhs, rhs);
  }
  */
};

/*
template <typename Lhs>
template <typename Rhs>
const GPMatrixSum<Lhs, Rhs> GPMatrixBase<Lhs>::operator+=(
    const GPMatrixBase<Rhs>& rhs) {
  return GPMatrixSum<Lhs, Rhs>(lhs, rhs);
}
*/
/*
  template <class Left, class Right>
  GPMatrixSum<Left, Right> operator+(
      const GPMatrixBase<Left>& lhs, const GPMatrixBase<Right>& rhs) {
    return GPMatrixSum<Left, Right>(lhs, rhs);
  }
*/
#endif /* GPMATRIX_SUM_H */
