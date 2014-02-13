#include "fwd.h"

template <class EigenType> class Vec : public VecBase<Vec<EigenType> > {
 public:
  typedef EigenType result_type;

 private:
  result_type data_;

 public:
  Vec(const result_type& n) : data_(n) {}

  result_type get() const { return data_; }
};
