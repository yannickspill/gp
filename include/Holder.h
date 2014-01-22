#ifndef HOLDER_H
#define HOLDER_H
#include <Eigen/Dense>
#include <Eigen/Cholesky>

template <class Derived> class HolderBase {
 public:
  const Derived& asDerived() const {
    return static_cast<const Derived&>(*this);
  }
};

template <class EigenType>
class Holder : public HolderBase<Holder<EigenType> > {
 public:
  typedef EigenType result_type;

 private:
  result_type in_;

 public:
  Holder(const EigenType& in) : in_(in) {}
  const result_type& get() const { return in_; }
};

template <class Lhs, class Rhs>
class HolderProd : public HolderBase<HolderProd<Lhs, Rhs> > {
 public:
  typedef typename Eigen::ProductReturnType
      <typename Lhs::result_type, typename Rhs::result_type>::Type result_type;

 private:
  Lhs lhs_;
  Rhs rhs_;
  mutable std::shared_ptr<result_type> ret_;

 public:
  HolderProd(const Lhs& lhs, const Rhs& rhs)
      : lhs_(lhs), rhs_(rhs), ret_(nullptr) {}

  const result_type& get() const {
    ret_ = std::make_shared<result_type>(lhs_.get() * rhs_.get());
    return *ret_;
  }
};

template <class Lhs, class Rhs>
const HolderProd<Lhs, Rhs> operator*(const HolderBase<Lhs>& lhs,
                                     const HolderBase<Rhs>& rhs) {
  return HolderProd<Lhs, Rhs>(lhs.asDerived(), rhs.asDerived());
}

typedef Holder<Eigen::MatrixXd> Mat;

#endif /* HOLDER_H */
