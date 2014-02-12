#include "Scalar.h"
#include "Matrix.h"

#include <Eigen/Dense>
#include <iostream>

using namespace GP;

int main(int, char*[]){
    //Functor taking row vector as input
    std::cout << "=== BUILD SCALAR " << std::endl;
    Scalar y(2.0);
    /*
    RowVectorXd x(Eigen::RowVectorXd::LinSpaced(5,0,1));
    auto f1 = internal::make_functor(x*y, x);
    MatrixXd inmat(Eigen::MatrixXd::Random(3,5));
    auto m1 = MatrixXd::Apply(f1, inmat);
    if ((m1.get() - y.get()*inmat.get()).norm() >1e-5) return 1;
    //Functor taking scalar as input
    */
    std::cout << "=== BUILD FUNCTOR " << std::endl;
    auto f2 = internal::make_functor(y,y);
    std::cout << "=== BUILD INPUT VECTOR " << std::endl;
    VectorXd z(Eigen::VectorXd::LinSpaced(5,0,1));
    std::cout << "=== APPLY FUNCTOR " << std::endl;
    auto m2 = VectorXd::Apply(f2, z);
    //if (m2.get() != z.get()) return 2;
    //matrix works with subsequent ops
    std::cout << "=== OBSERVED " << std::endl;
    auto observed = (3*m2).get();
    std::cout << "=== EXPECTED " << std::endl;
    auto expected = 3*z.get();
    std::cout << "=== TEST " << std::endl;
    if (observed != expected) return 3;
    //if (((3*m1).get() - 3*y.get()*inmat.get()).norm() >1e-5) return 4;
    return 0;
}
