#include "GPMatrix.h"

#include <Eigen/Dense>


int main(int, char*[]) {
    //matrix creation
    Eigen::MatrixXd X(Eigen::MatrixXd::Random(10,3));
    GPMatrixXd gpX(X);
    if (gpX.eigen() != X) return 1;
    //matrix sum
    Eigen::MatrixXd Y(Eigen::MatrixXd::Random(10,3));
    GPMatrixXd gpY(Y);
    GPMatrixXd gpSum(X+Y);
    if (gpSum.eigen() != X+Y) return 2;
    //gpY*2.;
    //if ((gpX+gpY).eigen() != X+Y) return 3;
    return 0;
}
