#include "Matrix.h"
#include "Scalar.h"
#include <iostream>

int main(){
    std::cout << "* Construct A" << std::endl;
    GP::MatrixXd A(Eigen::MatrixXd::Random(5,5));
    std::cout << "* Construct B" << std::endl;
    GP::MatrixXd B(Eigen::MatrixXd::Random(5,5));
    std::cout << "* Construct transpose" << std::endl;
    auto sum((A+B).transpose());
    std::cout << "* Construct cache" << std::endl;
    auto csum(sum.cache());
    std::cout << "* get cache" << std::endl;
    std::cout << csum.get() << std::endl;
    std::cout << "* get cache again" << std::endl;
    std::cout << csum.get() << std::endl;

    return 0;
}
