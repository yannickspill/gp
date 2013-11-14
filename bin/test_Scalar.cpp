#include "Scalar.h"

#include <boost/scoped_ptr.hpp>

int main(int, char*[]){
    Scalar *s = new Scalar(1.0);
    if (s->get() != 1.0) return 1;
    if (s->has_lower() == true) return 2;
    if (s->has_upper() == true) return 3;

    s->set_lower(2.0);
    if (s->get() != 2.0) return 4;
    if (s->has_lower() == false) return 5;
    if (s->has_upper() == true) return 6;

    s->clear_lower();
    if (s->get() != 2.0) return 7;
    if (s->has_lower() == true) return 8;
    if (s->has_upper() == true) return 9;

    s->set_upper(0.0);
    if (s->get() != 0.0) return 10;
    if (s->has_lower() == true) return 11;
    if (s->has_upper() == false) return 12;

    s->clear_upper();
    if (s->get() != 0.0) return 13;
    if (s->has_lower() == true) return 14;
    if (s->has_upper() == true) return 15;

    s->set_lower(3.0);
    s->set_upper(2.0);
    if (s->get_lower() != s->get_upper()) return 16;

    delete s;
    return 0;
}
