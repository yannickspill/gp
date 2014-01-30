#include <utility>
#include <iostream>

template<class> class trtest;
template<class> class Test;

template<class Derived> struct trtest<Test<Derived> > {
    typedef Derived ttype;
    enum{ tval=1 };
};

template<class Derived> struct Test{
    typedef typename trtest<Test>::ttype ttype;
    enum{tval=trtest<Test>::tval};
    void get() const {
        std::cout << tval << std::endl;
        std::cout << typeid(ttype).name() << std::endl;
    }
};

