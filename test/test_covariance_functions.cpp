#include "covariance_functions.h"
#include "Matrix.h"
#include "Functor.h"

#include <Eigen/Dense>
#include <math.h>

int main(){
    //matrix constructor
    Eigen::MatrixXd A(Eigen::MatrixXd::Random(3,3));
    Eigen::MatrixXd M(A.transpose()*A);
    GP::MatrixXd gM(M);
    auto cf = GP::covariance::squared_exponential(gM);
    Eigen::VectorXd lhs(Eigen::VectorXd::Random(3));
    Eigen::VectorXd rhs(Eigen::VectorXd::Random(3));
    auto diff = lhs-rhs;
    if (cf(lhs,rhs) != std::exp(-0.5*(diff.transpose()*M*diff)(0,0))) return 1;
    //Scalar constructor, dims > 1
    GP::Scalar lambda(2.0);
    Eigen::MatrixXd M2(Eigen::VectorXd::Constant(3,1./4.).asDiagonal());
    auto cf2 = GP::covariance::squared_exponential(lambda, 3);
    if (cf2(lhs, rhs) != std::exp(-0.5 * (diff.transpose() * M2 * diff)(0, 0)))
      return 2;
    //Scalar constructor, dims = 1
    auto cf3 = GP::covariance::squared_exponential(lambda);
    if (cf3(1.0,2.0) != std::exp(-0.5*(1./4.))) return 3;
    return 0;
}
