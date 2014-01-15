#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H

namespace GP{
namespace internal{
// declare traits class
template <class Derived> struct traits;

// declare basic matrix classes
template <class Derived> class MatrixBase;
template <class EigenType> class Matrix;

//matrix operations
template <class Lhs, class Rhs> class MatrixSum;
template <class Lhs, class Rhs> class MatrixDifference;
template <class Lhs, class Rhs> class MatrixMatrixProduct;

//define tree types
struct Leaf {};
struct Branch {};

}
}
#endif /*FORWARD_DECLARATIONS_H*/
