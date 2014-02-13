#include "fwd.h"
 
template <typename Derived>
class VecBase {
    protected:
        VecBase() {} //only children can instantiate it
public:
  const Derived& asDerived() const {
      return static_cast<const Derived&>(*this);
  }
};

