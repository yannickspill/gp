#include "Scalar.h"
#include "Matrix.h"

#include "internal/MatrixFromFunctor.h"

#include <Eigen/Dense>
#include <iostream>

using namespace GP;

int main(int, char*[]){
    //using make_functor
    RowVectorXd x(Eigen::RowVectorXd::LinSpaced(5,0,1));
    Scalar y(2.0);
    auto f1 = internal::make_functor(x*y, x);
    MatrixXd inmat(Eigen::MatrixXd::Random(3,5));
    auto m1 = MatrixXd::Apply(f1, inmat);
    std::cout << f1(x) << std::endl;
    std::cout << "=== " << std::endl;
    std::cout << m1.get() << std::endl;
    std::cout << "=== " << std::endl;
    std::cout << (y.get()*inmat.get()) << std::endl;
    if ((m1.get() - y.get()*inmat.get()).norm() >1e-5) return 1;
    return 0;
}
