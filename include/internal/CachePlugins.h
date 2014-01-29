#ifndef CACHE_PLUGINS_H
#define CACHE_PLUGINS_H

#include "ForwardDeclarations.h"

namespace GP{
namespace internal{

//! Plugins for caching mechanism 
// useful if you want the Cache object to mimick the cached object's methods 
template<class Derived> class CachePlugins {};

// implement solve and logdet for cached Decomposition objects
template<class Derived, template<class> class Policy>
class CachePlugins<Decomposition<Derived, Policy> > {
    private:
        typedef Cache<Decomposition<Derived, Policy> > Child;

    public:
  // log-determinant, can only be applied to LDLT decomposition for now
  // write another specialization in LogDeterminant.h if you need more
  LogDeterminant<Child> logdet() const {
      LOG("   Cache Plugin logdet" << std::endl);
    return LogDeterminant<Child>(static_cast<const Child&>(*this));
  }

  // solve AX=B system, and yield X
  template <class OtherDerived>
  Solve<Child, OtherDerived> solve(const OtherDerived& B) const {
      LOG("   Cache Plugin solve" << std::endl);
    return Solve<Child, OtherDerived>(static_cast<const Child&>(*this), B);
  }

};

}
}
#endif /* CACHE_PLUGINS_H */
