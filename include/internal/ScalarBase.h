#ifndef SCALAR_BASE_H
#define SCALAR_BASE_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"
#include "internal/GPBase.h"

namespace GP {
namespace internal {

//! Base class for any matrix
template <class Derived>
class ScalarBase : public GPBase<Derived> {

   protected:
    ScalarBase() {}  // only children can instantiate it

   public:
    // typedefs
    typedef typename traits<Derived>::scalar_type scalar_type;
    typedef typename traits<Derived>::result_type result_type;

   public:
    // allow implicit casting to Derived
    const Derived& asDerived() const {
        return static_cast<const Derived&>(*this);
    }
    result_type get() const {
        return static_cast<const Derived*>(this)->get();
    }

    unsigned get_version() const {
        return static_cast<const Derived*>(this)->get_version();
    }
};

}
}
#endif /* SCALAR_BASE_H */
