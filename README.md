Standalone Gaussian Process implementation
==

Aim
=

- Depend only on open-source libraries
  - Eigen (matrix manipulation)
  - GSL (optimization)
- Be easily interfaceable
  - R module
  - IMP module

Build
=

To build without comments:
```
cd build
cmake ../ -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja
```

To build with logging
```
cd build
cmake ../ -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja
```

