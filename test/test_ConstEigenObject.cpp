#include "ConstEigenObject.h"
#include "Scalar.h"
#include <Eigen/Dense>

int main(int, char * []) {
    //Vector
    Eigen::VectorXd x(Eigen::VectorXd::LinSpaced(10,2.5,5));
    ConstEigenObject<Eigen::VectorXd> cox(x);
    if (cox.get() != x) return 1;
    Scalar a(1.0);
    if (cox.get_is_function_of(a) == true) return 2;
    if (cox.get_derivative(a) != Eigen::VectorXd::Zero(10)) return 3;
    //Matrix
    Eigen::MatrixXd Y(Eigen::MatrixXd::Random(10,6));
    ConstEigenObject<Eigen::MatrixXd> coY(Y);
    if (coY.get() != Y) return 4;
    if (coY.get_is_function_of(a) == true) return 5;
    if (coY.get_derivative(a) != Eigen::MatrixXd::Zero(10,6)) return 6;
    return 0;
}

