#ifndef MATRIX_MATRIX_PRODUCT_H
#define MATRIX_MATRIX_PRODUCT_H

#include "macros.h"
#include "MatrixBase.h"
#include "Matrix.h"

#include <Eigen/Core>
#include <type_traits>

namespace GP {
namespace internal {

// specialize traits for MatrixMatrixProduct
template <class Lhs, class Rhs>
struct traits<MatrixMatrixProduct<Lhs, Rhs> > {
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
class MatrixMatrixProduct
    : public MatrixBase<MatrixMatrixProduct<Lhs, Rhs> > {
   private:
    Lhs lhs_;
    Rhs rhs_;

   public:
    typedef typename traits<MatrixMatrixProduct<Lhs, Rhs> >::scalar_type
        scalar_type;
    typedef typename traits<MatrixMatrixProduct<Lhs, Rhs> >::result_type
        result_type;

   public:
    // constructor
    MatrixMatrixProduct(const Lhs& lhs, const Rhs& rhs)
        : lhs_(lhs), rhs_(rhs) {}

    // actual computation
    result_type eval() const { return lhs_.eval() * rhs_.eval(); }
};
}
}
#endif /* MATRIX_MATRIX_PRODUCT_H */
