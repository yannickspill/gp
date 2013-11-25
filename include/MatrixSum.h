#ifndef MATRIX_SUM_H
#define MATRIX_SUM_H

#include "Scalar.h"
#include "macros.h"

#include <Eigen/Dense>

//! Sum two Eigen objects
template <class LEFTTYPE, class RIGHTTYPE>
class MatrixSum {
    LEFTTYPE l_;
    RIGHTTYPE r_;
    unsigned vleft_, vright_, version_;

   public:
    //! constructor
    MatrixSum(LEFTTYPE l, RIGHTTYPE r)
        : l_(l),
          r_(r),
          vleft_(l_.update()),
          vright_(r_.update()),
          version_(0) {}

    typedef decltype(l_.get() + r_.get()) result_type;
    result_type get() const { return l_.get() + r_.get(); }

    unsigned update() {
        unsigned vleft = l_.update();
        unsigned vright = r_.update();
        if (vleft != vleft_ || vright != vright_) version_++;
        vleft_ = vleft;
        vright_ = vright;
        return version_;
    }
};

#endif /* MATRIX_SUM_H */
