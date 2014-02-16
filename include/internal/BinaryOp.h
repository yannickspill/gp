#ifndef BINARY_OP_H
#define BINARY_OP_H

#include "macros.h"
#include "internal/ForwardDeclarations.h"

namespace GP {
namespace internal {

//! expression for binary operations
template <template <class,class> class BinaryOperator, class Lhs, class Rhs>
class BinaryOp : public BinaryOperator<Lhs, Rhs>::template Parent
                 <BinaryOp<BinaryOperator, Lhs, Rhs> > {

  typedef BinaryOperator<Lhs, Rhs> Op;

 public:
  typedef typename Op::scalar_type scalar_type;
  enum {
    RowsAtCompileTime = Op::RowsAtCompileTime,
    ColsAtCompileTime = Op::ColsAtCompileTime
  };
  typedef typename Op::lhs_type lhs_type;
  typedef typename Op::rhs_type rhs_type;
  typedef typename Op::result_type result_type;

 private:
  Lhs lhs_;
  Rhs rhs_;
  struct Data {
    lhs_type lhs_val_;
    rhs_type rhs_val_;
    result_type out_val_;
    Data(const lhs_type& lhs_val, const rhs_type& rhs_val)
        : lhs_val_(lhs_val), rhs_val_(rhs_val),
          out_val_(Op::apply(lhs_val_, rhs_val_)) {}
  };
  mutable std::shared_ptr<Data> data_;

 public:
  BinaryOp(const Lhs& lhs, const Rhs& rhs) : lhs_(lhs), rhs_(rhs) {}
  const result_type& get() const {
    data_ = std::make_shared<Data>(lhs_.get(), rhs_.get());
    return data_->out_val_;
  }
  unsigned get_version() const {
    return lhs_.get_version() + rhs_.get_version();
  }
};
}
}
#endif /* BINARY_OP_H */
