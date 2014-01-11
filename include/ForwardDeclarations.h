#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H

// declare traits class
template <class Derived> struct traits;

// declare basic matrix classes
template <class Derived> class GPMatrixBase;
template <class Derived> class GPMatrix;

//matrix operations
template <class Lhs, class Rhs> class GPMatrixSum;


#endif /*FORWARD_DECLARATIONS_H*/
