Gaussian Process implementation without IMP dependency
==

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


