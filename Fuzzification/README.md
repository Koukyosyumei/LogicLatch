# Fuzzification

```bash
cmake -S . -B build
cmake --build build
opt -enable-new-pm=0 -f -load ./build/libBranchExtractorPass.so -branchextractor -S -o example/simple_sleep.ll example/simple.ll
```