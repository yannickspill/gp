#ifndef GPMATRIX_SUM_H
#define GPMATRIX_SUM_H

#include "macros.h"
#include "GPMatrixBase.h"
#include "GPMatrix.h"

// specialize traits for GPMatrixSum
template <class Lhs, class Rhs>
struct traits<GPMatrixSum<Lhs, Rhs> > {
    typedef typename Lhs::scalar_type scalar_type;
    typedef typename Lhs::result_type result_type;
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
    const result_type eigen() const {
        result_type ret, left(lhs_.eigen()), right(rhs_.eigen());
        for (unsigned i = 0; i < left.size(); i++)
            ret.push_back(left[i] + right[i]);
        return ret;
    }
};

#endif /* GPMATRIX_SUM_H */
