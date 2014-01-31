#ifndef INTERNAL_FUNCTOR_H
#define INTERNAL_FUNCTOR_H

#include "ForwardDeclarations.h"

#include <tuple>
#include <utility>
#include <functional>

namespace GP {
namespace internal {

/*
// inspired from http://stackoverflow.com/a/6894436/958110
template <std::size_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp)>::type  // SFINAE
    reset_exprs(std::tuple<Tp...>&,
                const std::tuple<typename Tp::result_type...>& vals) {}
//
template <std::size_t I = 0, typename... Tp>
typename std::enable_if
    < I<sizeof...(Tp)>::type reset_exprs(std::tuple<Tp...>& exprs,
                                         const std::tuple
                                         <typename Tp::result_type...>& vals) {
  std::get<I>(exprs).set(std::get<I>(vals));
  reset_exprs<I + 1, Tp...>(exprs, vals);
}
*/

// same here
// inspired from http://stackoverflow.com/a/6894436/958110
template <std::size_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp), unsigned>::type sum_versions(
    const std::tuple<Tp...>&) {}
//
template <std::size_t I = 0, typename... Tp>
typename std::enable_if
    < I<sizeof...(Tp), unsigned>::type sum_versions(const std::tuple
                                                    <Tp...>& exprs) {
  return std::get<I>(exprs).get_version() + sum_versions<I + 1, Tp...>(exprs);
}

// convert an expression into a function
// for now, the inputs are not reset to their old value
template <class OutExpr, class... InExprs>
std::function<typename OutExpr::result_type(typename InExprs::result_type...)>
make_function(const OutExpr& out_expr, InExprs&... in_exprs) {
  return [&](const typename InExprs::result_type&... in_vals) {
    //std::tuple<InExprs...> exprs{in_exprs...};
    //std::tuple<typename InExprs::result_type...> priorvals{in_exprs.get()...};
    // see http://stackoverflow.com/a/12515637/958110
    int dummy[]{0, (in_exprs.set(in_vals), 0)...};
    typename OutExpr::result_type retval(out_expr.get());
    //reset_exprs(exprs, priorvals);
    return retval;
  };
}

// traits
template <class OutExpr, class... InExprs>
struct traits<Functor<OutExpr, InExprs...> > {
  typedef typename OutExpr::scalar_type scalar_type;
  typedef typename OutExpr::result_type result_type;
};

// Functor: wrapper around make_function, with some additional traits and a
// get_version() call.
template <class OutExpr, class... InExprs> class Functor {

 public:
  typedef typename traits<Functor>::scalar_type scalar_type;
  typedef typename traits<Functor>::result_type result_type;

 private:
  std::tuple<InExprs...> in_exprs_;
  OutExpr out_expr_;
  std::function
      <typename OutExpr::result_type(typename InExprs::result_type...)> fn_;

 public:
  Functor(const OutExpr& out_expr, InExprs&... in_exprs)
      : in_exprs_{in_exprs...}, out_expr_(out_expr),
        fn_(make_function(out_expr, in_exprs...)) {}

  result_type operator()(const typename InExprs::result_type
                         &... in_vals) const {
    return const_cast<Functor*>(this)->fn_(in_vals...);
  }

  unsigned get_version() const { return sum_versions(in_exprs_); }
};

// convert an expression into a functor, e.g. function with some traits
template <class OutExpr, class... InExprs>
Functor<OutExpr,InExprs...> make_functor(const OutExpr& out_expr,
                                                      InExprs&... in_exprs) {
  return Functor<OutExpr, InExprs...>(out_expr, in_exprs...);
}
}
}
#endif /* INTERNAL_FUNCTOR_H */
