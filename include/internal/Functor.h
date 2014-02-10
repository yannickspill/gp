#ifndef INTERNAL_FUNCTOR_H
#define INTERNAL_FUNCTOR_H

#include "ForwardDeclarations.h"

#include <tuple>
#include <utility>
#include <functional>

namespace GP {
namespace internal {

// traits
template <class OutExpr, class... InExprs>
struct traits<Functor<OutExpr, InExprs...> > {
  typedef typename OutExpr::scalar_type scalar_type;
  typedef typename OutExpr::result_type result_type;
  enum {
    RowsAtCompileTime = OutExpr::RowsAtCompileTime,
    ColsAtCompileTime = OutExpr::ColsAtCompileTime
  };
  typedef Functor<OutExpr, InExprs...> type;
};

// Functor: class providing an operator(), some additional traits and a
// get_version() call.
// the InExprs arguments are possibly changed at each call of operator()
// although the function is const
template <class OutExpr, class... InExprs>
class Functor {

 public:
  // typedefs
  typedef typename traits<Functor>::scalar_type scalar_type;
  typedef typename traits<Functor>::result_type result_type;
  enum {
    RowsAtCompileTime = traits<Functor>::RowsAtCompileTime,
    ColsAtCompileTime = traits<Functor>::ColsAtCompileTime
  };
  typedef typename traits<Functor>::type type;

 private:
  std::tuple<InExprs...> in_exprs_;
  OutExpr out_expr_;

 public:
  Functor(const OutExpr& out_expr, InExprs&... in_exprs)
      : in_exprs_{in_exprs...}, out_expr_(out_expr) {}

  template <class... InVals>
  typename OutExpr::result_type operator()(InVals&&... in_vals) const {
    const_cast<Functor*>(this)->set_vals(std::forward<InVals>(in_vals)...);
    typename OutExpr::result_type retval(out_expr_.get());
    return retval;
  }

  unsigned get_version() const { return sum_versions(in_exprs_); }

 private:

  // forward all provided arguments to the corresponding set() methods
  template <typename InVal, typename... InVals>
  void set_vals(InVal&& in_val, InVals&&... in_vals) {
    constexpr unsigned i = sizeof...(InExprs) - (sizeof...(InVals) + 1);
    std::get<i>(in_exprs_).set(std::forward<InVal>(in_val));
    set_vals(in_vals...);
  }
  //
  template <typename InVal>
  void set_vals(InVal&& in_val) {
      constexpr unsigned i = sizeof...(InExprs) - 1;
      std::get<i>(in_exprs_).set(std::forward<InVal>(in_val));
  }

  // sum the versions of all expressions
  // inspired from http://stackoverflow.com/a/6894436
  template <std::size_t I = 0, typename... Tp>
  typename std::enable_if< I<sizeof...(Tp), unsigned>::type
  sum_versions(const std::tuple<Tp...>& exprs) {
    return std::get<I>(exprs).get_version() + sum_versions<I + 1, Tp...>(exprs);
  }
  //
  template <std::size_t I = 0, typename... Tp>
  typename std::enable_if<I == sizeof...(Tp), unsigned>::type
  sum_versions(const std::tuple<Tp...>&) {}
};

//!special case in which Functor is a constant function
//the function accepts any argument, and returns the value provided on
//construction
template<>
class Functor<double> {
    double val_;
 public:
  // typedefs
  typedef double scalar_type;
  typedef double result_type;
  enum {
    RowsAtCompileTime = 1,
    ColsAtCompileTime = 1
  };
  Functor(double val) : val_(val) {}

  template<class... Args>
  double operator()(Args&&...) const { return val_; }
  unsigned get_version() const {return 0; }
};

// convert an expression into a functor, e.g. function with some traits
// the InExprs arguments are possibly changed at each call of operator()
// the function is however conceptually const
template <class OutExpr, class... InExprs>
Functor<OutExpr, InExprs...> make_functor(OutExpr out_expr,
                                          InExprs... in_exprs) {
  return Functor<OutExpr, InExprs...>(out_expr, in_exprs...);
}
//
Functor<double> make_functor(double expr) { return Functor<double>(expr); }
}
}
#endif /* INTERNAL_FUNCTOR_H */
