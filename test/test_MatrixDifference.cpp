#include "MatrixDifference.h"
#include "macros.h"
#include "ConstEigenObject.h"

#include <Eigen/Dense>

int main(int, char*[]) {
    Eigen::MatrixXd A(Eigen::MatrixXd::Random(10,3));
    Eigen::MatrixXd B(Eigen::MatrixXd::Random(10,3));
    typedef ConstEigenObject<Eigen::MatrixXd> ConstMat;
    ConstMat cA(A), cB(B);
    typedef MatrixDifference<ConstMat, ConstMat> MD;
    MD diff(cA, cB);
    diff.get();
}
