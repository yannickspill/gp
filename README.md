C++ Gaussian Process library
===

Aim
---

- Be fast
- Scale well
- Depend only on open-source libraries
  - Eigen (matrix manipulation)
  - GSL (optimization, TODO)
  - Celero (benchmarking, TODO)
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

