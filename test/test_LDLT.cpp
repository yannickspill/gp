#include "ConstEigenObject.h"
#include "LDLT.h"

#include <Eigen/Dense>
#include <math.h>

typedef ConstEigenObject<Eigen::MatrixXd> ConstantCov;

int main(int, char * []) {
    Eigen::MatrixXd m(2, 2);
    m << 1, .5, .5, 1;
    ConstantCov cc(m);
    LDLT<ConstantCov> ldlt(cc);
    if (ldlt.get_log_determinant() != std::log(0.75)) return 1;
    return 0;
}

