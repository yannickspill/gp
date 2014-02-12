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
    auto f2 = internal::Functor<Scalar, Scalar>(y,y);
    std::cout << "=== BUILD INPUT VECTOR " << std::endl;
    VectorXd z(Eigen::VectorXd::LinSpaced(5,0,1));
    std::cout << "=== APPLY FUNCTOR " << std::endl;
    auto m2 = internal::MatrixFromUnivariateFunctor<internal::Functor<Scalar,Scalar>, VectorXd>(f2,z);
    std::cout << "=== PRODUCT " << std::endl;
    auto prod = 3*m2;
    std::cout << "=== OBSERVED " << std::endl;
    auto observed = prod.get();
    std::cout << "=== EXPECTED " << std::endl;
    auto expected = 3*z.get();
    std::cout << "=== TEST " << std::endl;
    std::cout << expected << std::endl;
    std::cout << "=== TEST " << std::endl;
    std::cout << observed << std::endl;
    return 0;
}
