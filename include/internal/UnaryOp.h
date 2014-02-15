#ifndef UNARY_OP_H
#define UNARY_OP_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"

namespace GP {
namespace internal {

//expression for unary operations
template <template <class> class UnaryOperator, class Operand>
class UnaryOp : public UnaryOperator<Operand>::template Parent
                <UnaryOp<UnaryOperator, Operand> > {

  typedef UnaryOperator<Operand> Op;

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
  UnaryOp(const Operand& in) : in_(in) {}
  const result_type& get() const {
    data_ = std::make_shared<Data>(in_.get());
    return data_->out_val_;
  }
  unsigned get_version() const { return in_.get_version(); }
};
}
}
#endif /* UNARY_OP_H */
