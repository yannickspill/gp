#ifndef INTERNAL_MAKE_FUNCTION_H
#define INTERNAL_MAKE_FUNCTION_H

#include <functional>

namespace GP {
namespace internal {
// convert an expression into a function
template <class OutExpr, class... InExprs>
std::function<typename OutExpr::result_type(typename InExprs::result_type...)>
make_function(const OutExpr& out_expr, InExprs&... in_exprs) {
  return[&](typename InExprs::result_type... in_vals) {
    // see http://stackoverflow.com/a/12515637/958110
    int dummy[]{0, (in_exprs.set(in_vals),0)...};
    return out_expr.get();
};
}
}
}
#endif /* INTERNAL_MAKE_FUNCTION_H */
