#ifndef INTERNAL_MAKE_FUNCTION_H
#define INTERNAL_MAKE_FUNCTION_H

#include <functional>

namespace GP {
namespace internal {


//TODO make_function should get/set old values
// convert an expression into a function
template <class OutExpr, class... InExprs>
std::function<typename OutExpr::result_type(typename InExprs::result_type...)>
make_function(const OutExpr& out_expr, InExprs&... in_exprs) {
  return[&](const typename InExprs::result_type&... in_vals) {
    // see http://stackoverflow.com/a/12515637/958110
    int dummy[]{0, (in_exprs.set(in_vals),0)...};
    return out_expr.get();
};

template <class OutExpr, class... InExprs>
class Functor {

    public:
        typedef typename OutExpr::scalar_type scalar_type;
        typedef typename OutExpr::result_type result_type;
        typedef typename OutExpr::RowsAtCompileTime RowsAtCompileTime;
        typedef typename OutExpr::ColsAtCompileTime ColsAtCompileTime;

    private:
     std::function
         <typename OutExpr::result_type(typename InExprs::result_type...)> fn_;

    public:
     Functor(const OutExpr& out_expr, InExprs&... in_exprs)
         : fn_(make_function(out_expr, in_exprs...)) {}

     operator()(const typename InExprs::result_type&... in_vals) const {
         return const_cast<Functor*>(this)->fn_(in_vals...);
     }

     unsigned get_version() const {

     }
};


}
}
}
#endif /* INTERNAL_MAKE_FUNCTION_H */
