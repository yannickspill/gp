#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <stdlib.h>

#ifdef LOGGING
#define LOG(msg) {std::cout << msg;}
#else
#define LOG(msg) {}
#endif

#define CHECK(expr, message) {if (!(expr)) {\
    std::cout << message << std::endl;\
    abort();\
}}

#define SQUARE(expr) ((expr)*(expr))

#define CUBE(expr) ((expr)*(expr)*(expr))

#define PI 3.1415926535897931

#endif /* MACROS_H */
