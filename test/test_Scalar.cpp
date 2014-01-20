#include "Scalar.h"

#include <limits>

using namespace GP;

int main(int, char * []) {
    Scalar s(1.0);
    double inf = std::numeric_limits<double>::infinity();
    if (s.get() != 1.0) return 1;

    //a scalar is just a handle on its data
    Scalar t(s);
    t.set(2.5);
    if (s.get() != t.get()) return 17;

    //test operator==
    if (!(t == s)) return 20;
    if (t != s) return 21;
    Scalar u(1.0);
    if (s == u) return 22;

    return 0;
}
