#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <stdlib.h>
#include <boost/scoped_ptr.hpp>

//logging macro
#ifdef LOGGING
#define LOG(msg) { std::cout << msg; }
#else
#define LOG(msg) {}
#endif

//runtime check
#ifdef CHECKS
#define CHECK(expr, message)                   \
    {                                          \
        if (!(expr)) {                         \
            std::cout << message << std::endl; \
            abort();                           \
        }                                      \
    }
#else
#define CHECK(expr, message) {}
#endif

//arithmetic operations
#define SQUARE(expr) ((expr) * (expr))
#define CUBE(expr) ((expr) * (expr) * (expr))

//constants
#define PI 3.1415926535897931


#endif /* MACROS_H */
