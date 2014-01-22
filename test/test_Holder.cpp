#include "Holder.h"
#include <typeinfo>
#include <iostream>
#include <Eigen/Dense>

int main(int, char*[]){
    Eigen::MatrixXd Id(Eigen::MatrixXd::Identity(5,5));
    Mat m(Id);
    Eigen::MatrixXd em((m*m*m).get());
    return 0;
}
