#include "mean_functions.h"
#include "Matrix.h"

#include <Eigen/Dense>

int main(){
    auto zf = GP::mean::zero();
    //scalar input
    GP::Scalar scal(3.0);
    if (zf(2.0) != 0.) return 1;
    return 0;
}
