#include "fwd.h"
#include "base.h"
#include "vec.h"

#include <Eigen/Dense>
#include <iostream>

int main(){
    Vec<Eigen::RowVectorXd> v(Eigen::RowVectorXd::Random(5));
    std::cout << v.get() << std::endl;
    return 0;
}
