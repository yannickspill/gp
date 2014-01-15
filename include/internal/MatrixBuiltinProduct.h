#ifndef MATRIX_BUILTIN_PRODUCT_H
#define MATRIX_BUILTIN_PRODUCT_H

#include "macros.h"
#include "internal/MatrixBase.h"
#include "internal/Matrix.h"

#include <Eigen/Core>
#include <type_traits>
#include <memory>

namespace GP {
namespace internal {

// specialize traits for MatrixBuiltinProduct
template <class MatrixExpression>
struct traits<MatrixBuiltinProduct<MatrixExpression> > {
    typedef typename MatrixExpression::scalar_type scalar_type;
    typedef typename MatrixExpression::result_type result_type;
    /*
    typedef const typename Eigen::CwiseUnaryOp<
        Eigen::internal::scalar_multiple_op<double>,
        const typename MatrixExpression::result_type> result_type;
        */
    typedef branch_tag node_type;
};

// expression template for product of a Matrix with anything convertible to a
// double
template <class MatrixExpression>
class MatrixBuiltinProduct
    : public MatrixBase<MatrixBuiltinProduct<MatrixExpression> > {
   public:
    typedef typename traits<
        MatrixBuiltinProduct<MatrixExpression> >::scalar_type scalar_type;
    typedef typename traits<
        MatrixBuiltinProduct<MatrixExpression> >::result_type result_type;
    typedef typename traits<MatrixBuiltinProduct<MatrixExpression> >::node_type
        node_type;

   private:
    double lhs_;
    MatrixExpression rhs_;

   public:
    // constructor
    MatrixBuiltinProduct(double lhs, const MatrixExpression& rhs)
        : lhs_(lhs), rhs_(rhs) {}

    // actual computation
    result_type get() const { return lhs_ * rhs_.get(); }

    unsigned get_version() const { return rhs_.get_version(); }
};

// operator* declared here for use of automatic type conversion
template <class MatrixExpression>
const MatrixBuiltinProduct<MatrixExpression> operator*(
    double lhs, const MatrixBase<MatrixExpression>& rhs) {
    return MatrixBuiltinProduct<MatrixExpression>(lhs, rhs.asDerived());
}
template <class MatrixExpression>
const MatrixBuiltinProduct<MatrixExpression> operator*(
    const MatrixBase<MatrixExpression>& lhs, double rhs) {
    return MatrixBuiltinProduct<MatrixExpression>(rhs, lhs.asDerived());
}
}
}
#endif /* MATRIX_BUILTIN_PRODUCT_H */
