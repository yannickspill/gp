#include "Scalar.h"
#include "Matrix.h"
#include "internal/MatrixFromThinAir.h"

#include <Eigen/Dense>
#include <iostream>

using namespace GP;

int main(int, char*[]){
    auto m2 = internal::MatrixFromThinAir();
    std::cout << "=== GET MAT " << std::endl;
    auto mat = m2.get();
    std::cout << mat << std::endl;
    std::cout << "=== PRODUCT " << std::endl;
    auto prod = 3*m2;
    std::cout << "prod at " << &prod << std::endl;
    std::cout << "=== COUT " << std::endl;
    std::cout << prod.get() << std::endl;
    std::cout << "=== OBSERVED " << std::endl;
    auto observed = prod.get();
    std::cout << "prod.get() at " << &observed << std::endl;
    std::cout << "=== TEST " << std::endl;
    std::cout << observed << std::endl;
    return 0;
}
