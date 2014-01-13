#ifndef GPMATRIX_BASE_H
#define GPMATRIX_BASE_H

#include "macros.h"
#include "ForwardDeclarations.h"

//! Base class for any matrix
template <class Derived>
class GPMatrixBase {

    protected:
        GPMatrixBase() {} //only children can instantiate it

   public:
    // typedefs
    typedef typename traits<Derived>::scalar_type scalar_type;
    typedef typename traits<Derived>::result_type result_type;

   public:
    // allow implicit casting to Derived
    const Derived& asDerived() const {
        return static_cast<const Derived&>(*this);
    }
    result_type eigen() const {
        return static_cast<const Derived*>(this)->eigen();
    }

    template <class Lhs, class Rhs>
    friend GPMatrixSum<Lhs, Rhs> operator+(const GPMatrixBase<Lhs>& lhs,
                                           const GPMatrixBase<Rhs>& rhs) {
        return GPMatrixSum<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
    }
};

#endif /* GPMATRIX_BASE_H */
