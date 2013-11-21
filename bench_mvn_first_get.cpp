#include "MultivariateFNormalSufficient.h"
#include "MVN.h"
#include "ConstEigenObject.h"
#include "BasicMVN.h"

#include <Eigen/Dense>

namespace {
///
/// \func DoNotOptimizeAway
///
/// \author Andrei Alexandrescu
///
template <class T>
void DoNotOptimizeAway(T&& datum) {
    const void* p = &datum;
    putchar(*static_cast<const char*>(p));
}
}

int main(int, char*[]){
    
}
