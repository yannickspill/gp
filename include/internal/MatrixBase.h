#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

#include "macros.h"
#include "ForwardDeclarations.h"

namespace GP {
namespace internal {

//! Base class for any matrix
template <class Derived>
class MatrixBase {

   protected:
    MatrixBase() {}  // only children can instantiate it

   public:
    // typedefs
    typedef typename traits<Derived>::scalar_type scalar_type;
    typedef typename traits<Derived>::result_type result_type;

   public:
    // allow implicit casting to Derived
    const Derived asDerived() const {
        return static_cast<const Derived>(*this);
    }
    result_type eval() const {
        return static_cast<const Derived*>(this)->eval();
    }

    // difference
    template <class Lhs, class Rhs>
    friend const MatrixDifference<Lhs, Rhs> operator-(
        const MatrixBase<Lhs>& lhs, const MatrixBase<Rhs>& rhs) {
        return MatrixDifference<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
    }

    // product
    template <class Lhs, class Rhs>
    friend const MatrixMatrixProduct<Lhs, Rhs> operator*(
        const MatrixBase<Lhs>& lhs, const MatrixBase<Rhs>& rhs) {
        return MatrixMatrixProduct<Lhs, Rhs>(lhs.asDerived(),
                                               rhs.asDerived());
    }
};
// sum
template <class Lhs, class Rhs>
const MatrixSum<Lhs, Rhs> operator+(const MatrixBase<Lhs>& lhs,
                                      const MatrixBase<Rhs>& rhs) {
    return MatrixSum<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}
}
}
#endif /* MATRIX_BASE_H */
