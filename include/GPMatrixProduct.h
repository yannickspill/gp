#ifndef GPMATRIX_PRODUCT_H
#define GPMATRIX_PRODUCT_H

#include "macros.h"
#include "GPMatrixBase.h"
#include "GPMatrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <typeinfo>

// specialize traits for GPMatrixProduct
template <class Lhs, class Rhs>
struct traits<GPMatrixProduct<Lhs, Rhs> > {
    static_assert(std::is_same<typename Lhs::scalar_type,
                               typename Rhs::scalar_type>::value,
                  "cannot mix matrices of different scalar types");
    typedef typename Lhs::scalar_type scalar_type;
    typedef typename Lhs::result_type result_type;
    /*
    typedef typename Eigen::ProductReturnType<
        typename Lhs::result_type, typename Rhs::result_type>::Type result_type;
        */
};

//! \addtogroup Matrix sum, difference, product and division templates @{
template <typename Lhs, typename Rhs>
class GPMatrixProduct : public GPMatrixBase<GPMatrixProduct<Lhs, Rhs> > {
   private:
    Lhs lhs_;
    Rhs rhs_;

   public:
    typedef typename traits<GPMatrixProduct<Lhs, Rhs> >::scalar_type
        scalar_type;
    typedef typename traits<GPMatrixProduct<Lhs, Rhs> >::result_type
        result_type;

   public:
    // constructor
    GPMatrixProduct(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}

    // actual computation
    result_type eval() const { return lhs_.eval() * rhs_.eval(); }
};

#endif /* GPMATRIX_PRODUCT_H */
