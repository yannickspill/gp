#include "LDLT.h"

#include <Eigen/Dense>
#include <math.h>

class ConstantCov {
   public:
    Eigen::MatrixXd get() const {
        Eigen::MatrixXd m(2, 2);
        m << 1, .5, .5, 1;
        return m;
    }
};

int main(int, char * []) {
    ConstantCov cc;
    LDLT<ConstantCov> ldlt(cc);
    if (ldlt.get_log_determinant() != std::log(0.75)) return 1;
    return 0;
}

