Welcome to the GP library documentation!
GP is a Gaussian Process library that was developed with several aims
- Be fast
- Scale well
- Depend only on open-source libraries
  - Eigen (matrix manipulation)
  - GSL (optimization)
- Be easily interfaceable
  - R module
  - IMP module
It is written in `C++11`.


# General Concepts
##Scalar and Matrix
Throughout the whole GP library, there are two classes that represent two
fundamental types: `GP::Scalar` and `GP::Matrix`. Both are handles to either
`double` or an Eigen type. In the case of `GP::Matrix`, convenience typedefs are
provided that hold their corresponding Eigen type, e.g. `GP::VectorXd` holds a
`Eigen::VectorXd` etc.

The idea behind this library is to extend the concept of [expression
templates](http://en.wikipedia.org/wiki/Expression_templates), that Eigen uses,
to input that can be changed at runtime.

`GP::Scalar` and `GP::Matrix` objects can be combined in expressions. A number
of operations are supported, such as sum, product, division, transpose, trace,
LDLT decomposition etc. whenever appropriate. For example

    // initialize a GP::Scalar with a value of 1.0
    GP::Scalar s(1.0);
    // initialize a GP::Matrix with a random 5x3 matrix
    GP::MatrixXd mat(Eigen::MatrixXd::Random(5,3));
    // we are interested in the following expression
    auto expr = s + (mat.transpose()*mat).trace();

Until now, `expr` is unevaluated. Its value can be computed by calling
`expr.get()`. Suppose, now that we do

    s.set(3.0)
    expr.get()

then the result of the expression will be computed with the new value of `s`.
Since the calculations behind `get()` rely on Eigen's efficient expression
templates, the expression is both speed-efficient and can be evaluated for
different values of `s` and/or `mat`.

##Functors
Changing the inputs of an expression means knowing which inputs to change.
Besides, the inputs and the resulting expression are different objects, of which
it is necessary to keep track. Functors can be used to ease their management.
The command 

    auto f = GP::make_functor(expr, s);

generates a function that takes the input of `s.set()` as argument, and outputs
the result of `expr.get()`. For example

    f(3.0)

is equivalent to

    s.set(3.0)
    expr.get()

with the added advantage of only having to carry around `f`. `make_functor` can
take any number of inputs as arguments.

##Matrix from Functor
Sometimes, a matrix is not the proper input to an expression, but one would want
to build a matrix by applying a function to another matrix's rows. This is where
`Matrix<>::Apply` comes in handy. For example

    GP::VectorXd input(Eigen::VectorXd::Random(5));
    auto ret = GP::VectorXd::Apply(f, input);

`ret` is a `GP::Vector` built by applying the previously constructed function
`f` to every row of `input`.

Similar methods exist. Suppose you have a bivariate function `g` and two inputs
`input1` and `input2` with `N` and `M` rows respectively. Then, calling

    GP::Matrix<>::Apply(g, input1, input2)

will return a `NxM` matrix constructed by calling `g` on the rows of `input1`
and `input2`. Finally, 

    GP::Matrix<>::SymmetricApply(h, input3)

is equivalent to the previous one but optimized to compute half of the
coefficients only.

All returned objects behave like a normal expression, that is if you change
`input` after `ret` has been built, `ret.get()` will return an updated result.







