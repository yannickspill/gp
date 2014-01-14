#ifndef GPMATRIX_DIFFERENCE_H
#define GPMATRIX_DIFFERENCE_H

#include "macros.h"
#include "GPMatrixBase.h"
#include "GPMatrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

// specialize traits for GPMatrixDifference
template <class Lhs, class Rhs>
struct traits<GPMatrixDifference<Lhs, Rhs> > {
    static_assert(std::is_same<typename Lhs::scalar_type,
                               typename Rhs::scalar_type>::value,
                  "cannot mix matrices of different scalar types");
    typedef typename Lhs::scalar_type scalar_type;
    typedef typename Lhs::result_type result_type;
    /*typedef const typename Eigen::CwiseBinaryOp<
        Eigen::internal::scalar_difference_op<scalar_type>,
        typename std::add_const<typename Lhs::result_type>::type,
        typename std::add_const<typename Rhs::result_type>::type>
        result_type;
    */
};

//! \addtogroup Matrix sum, difference, product and division templates @{
template <typename Lhs, typename Rhs>
class GPMatrixDifference : public GPMatrixBase<GPMatrixDifference<Lhs, Rhs> > {
   private:
    Lhs lhs_;
    Rhs rhs_;

   public:
    typedef typename traits<GPMatrixDifference<Lhs, Rhs> >::scalar_type
        scalar_type;
    typedef typename traits<GPMatrixDifference<Lhs, Rhs> >::result_type
        result_type;

   public:
    // constructor
    GPMatrixDifference(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

    // actual computation
    result_type eval() const { return lhs_.eval() - rhs_.eval(); }
};

#endif /* GPMATRIX_DIFFERENCE_H */
