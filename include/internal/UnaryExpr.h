#ifndef UNARY_EXPR_H
#define UNARY_EXPR_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"

namespace GP {
namespace internal {

// expression for unary operations
template <template <class> class UnaryExprerator, class Operand>
class UnaryExpr : public UnaryExprerator<Operand>::template Parent
                  <UnaryExpr<UnaryExprerator, Operand> > {

  typedef UnaryExprerator<Operand> Op;

 public:
  typedef typename Op::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = Op::RowsAtCompileTime,
    ColsAtCompileTime = Op::ColsAtCompileTime
  };
  typedef typename Op::input_type input_type;
  typedef typename Op::result_type result_type;

 private:
  Operand in_;
  struct Data {
    input_type in_val_;
    result_type out_val_;
    Data(const input_type& in_val)
        : in_val_(in_val), out_val_(Op::apply(in_val_)) {}
  };
  mutable std::shared_ptr<Data> data_;

 public:
  UnaryExpr(const Operand& in) : in_(in) {}
  const result_type& get() const {
    data_ = std::make_shared<Data>(in_.get());
    return data_->out_val_;
  }
  unsigned get_version() const { return in_.get_version(); }
};
}
}
#endif /* UNARY_EXPR_H */
