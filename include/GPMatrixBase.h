#ifndef GPMATRIX_BASE_H
#define GPMATRIX_BASE_H

#include "macros.h"
#include "ForwardDeclarations.h"

//! Base class for any matrix
template <class Derived>
class GPMatrixBase {

   public:
    // typedefs
    typedef typename traits<Derived>::scalar_type scalar_type;
    typedef typename traits<Derived>::result_type result_type;

   public:
    // allow implicit casting to Derived
    operator Derived&() { return static_cast<Derived&>(*this); }
    operator const Derived&() const {
        return static_cast<const Derived&>(*this);
    }

    template <class Lhs, class Rhs>
    friend const GPMatrixSum<Lhs, Rhs> operator+(const GPMatrixBase<Lhs>& lhs,
                                                 const GPMatrixBase<Rhs>& rhs) {
        return GPMatrixSum<Lhs, Rhs>(lhs, rhs);
    }
};

#endif /* GPMATRIX_BASE_H */
