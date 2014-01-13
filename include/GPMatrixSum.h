#ifndef GPMATRIX_SUM_H
#define GPMATRIX_SUM_H

#include "macros.h"
#include "GPMatrixBase.h"
#include "GPMatrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

// specialize traits for GPMatrixSum
template <class Lhs, class Rhs>
struct traits<GPMatrixSum<Lhs, Rhs> > {
    static_assert(std::is_same<typename Lhs::scalar_type,
                               typename Rhs::scalar_type>::value,
                  "cannot mix matrices of different scalar types");
    typedef typename Lhs::scalar_type scalar_type;
    typedef typename Lhs::result_type result_type;
    /*typedef const typename Eigen::CwiseBinaryOp<
        Eigen::internal::scalar_sum_op<scalar_type>,
        typename std::add_const<typename Lhs::result_type>::type,
        typename std::add_const<typename Rhs::result_type>::type>
        result_type;
    typedef const typename Eigen::CwiseBinaryOp<Eigen::internal::scalar_sum_op<double>, const Eigen::MatrixXd, const Eigen::MatrixXd> result_type;
    */
};

//! \addtogroup Matrix sum, difference, product and division templates @{
template <typename Lhs, typename Rhs>
class GPMatrixSum : public GPMatrixBase<GPMatrixSum<Lhs, Rhs> > {
   private:
    Lhs lhs_;
    Rhs rhs_;

   public:
    typedef typename traits<GPMatrixSum<Lhs, Rhs> >::scalar_type scalar_type;
    typedef typename traits<GPMatrixSum<Lhs, Rhs> >::result_type result_type;

   public:
    // constructor
    GPMatrixSum(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

    // actual computation
    result_type eigen() const { return lhs_.eigen() + rhs_.eigen(); }
};

#endif /* GPMATRIX_SUM_H */
