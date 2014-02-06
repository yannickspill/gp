#include "Scalar.h"

#include <limits>
#include <math.h>

using namespace GP;

int main(int, char * []) {
    Scalar s(1.0);
    double inf = std::numeric_limits<double>::infinity();
    if (s.get() != 1.0) return 1;

    //a scalar is just a handle on its data
    Scalar t(s);
    t.set(2.5);
    if (s.get() != t.get()) return 2;

    //test operator==
    if (!(t == s)) return 3;
    if (t != s) return 4;
    Scalar u(1.0);
    if (s == u) return 5;

    // test exponential
    if (s.exp().get() != std::exp(s.get())) return 6;

    return 0;
}
