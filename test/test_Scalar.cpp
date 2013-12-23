#include "Scalar.h"

#include <limits>

int main(int, char * []) {
    Scalar s(1.0);
    double inf = std::numeric_limits<double>::infinity();
    if (s.get() != 1.0) return 1;
    if (s.get_lower() != -inf) return 2;
    if (s.get_upper() != inf) return 3;

    s.set_lower(2.0);
    if (s.get() != 2.0) return 4;
    if (s.get_lower() != 2.0) return 5;
    if (s.get_upper() != inf) return 6;

    s.clear_lower();
    if (s.get() != 2.0) return 7;
    if (s.get_lower() != -inf) return 8;
    if (s.get_upper() != inf) return 9;

    s.set_upper(0.0);
    if (s.get() != 0.0) return 10;
    if (s.get_lower() != -inf) return 11;
    if (s.get_upper() != 0.0) return 12;

    s.clear_upper();
    if (s.get() != 0.0) return 13;
    if (s.get_lower() != -inf) return 14;
    if (s.get_upper() != inf) return 15;

    //one bound moves the other one
    s.set_lower(3.0);
    s.set_upper(2.0);
    if (s.get_lower() != s.get_upper()) return 16;

    //a scalar is just a handle on its data
    Scalar t(s);
    t.set(2.5);
    if (s.get() != t.get()) return 17;
    if (s.get_lower() != t.get_lower()) return 18;
    if (s.get_upper() != t.get_upper()) return 19;

    //test operator==
    if (!(t == s)) return 20;
    if (t != s) return 21;
    Scalar u(1.0);
    if (s == u) return 22;

    return 0;
}
