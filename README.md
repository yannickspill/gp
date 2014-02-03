C++ Gaussian Process library
===

[![Build
Status](https://travis-ci.org/yannickspill/gp.png?branch=develop)](https://travis-ci.org/yannickspill/gp)

Aim
---

- Be fast
- Scale well
- Depend only on open-source libraries
  - Eigen (matrix manipulation)
  - GSL (optimization)
- Be easily interfaceable
  - R module
  - IMP module

Build
---

In what follows, the repository is expected to be in ./src and the library will
be built in ./build

To build without comments:
```
cd build
cmake ../src -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja
```

To build with logging
```
cd build
cmake ../src -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja
```

To run test for MVN, in build folder
```
./TestMVN
```

Trivial benchmark for MVN
```
./MVNBench ../data/toy.dat ../data/fitting_protocol2.dat
```

Simple benchmark for MVN
```
./MVNBench ../data/simple.dat ../data/fitting_protocol.dat
```


