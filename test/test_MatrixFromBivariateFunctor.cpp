#include "Scalar.h"
#include "Matrix.h"

#include <Eigen/Dense>
#include <iostream>

using namespace GP;

int main(int, char*[]){
    //using make_functor
    Scalar x(1.0);
    Scalar y(2.0);
    auto f1 = internal::make_functor(x*y, x, y);
    VectorXd invec1(Eigen::VectorXd::LinSpaced(5,0,1));
    VectorXd invec2(Eigen::VectorXd::LinSpaced(5,5,10));
    auto m1 = MatrixXd::Apply(f1, invec1, invec2);
    if ((m1.get() - invec1.get() * invec2.get().transpose()).norm() > 1e-5)
      return 1;
    return 0;
}
