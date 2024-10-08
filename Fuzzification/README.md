# Fuzzification

- count

```bash
./script/build.sh

opt -enable-new-pm=0 -f -load ./build/libBranchExtractorPass.so -branchextractor -S -o example/simple_sleep.ll example/simple.ll
```

- crc

```bash
./script/build.sh
clang++ -c crc.cpp -o crc.o

clang++ -S -emit-llvm example/simple.cpp -o example/simple.ll

opt -enable-new-pm=0 -load ./build/libCRCPass.so -crc-pass -S -o example/simple_crc.ll example/simple.ll
clang++ -o example/simple_crc example/simple_crc.ll crc.o
```

## Preparation

### pcre2

- repo: https://github.com/Koukyosyumei/pcre2/tree/fuzzification

```bash
mkdir build
cd build
cmake ..
make

cd ..
file ./build/CMakeFiles/pcre2test.dir/src/pcre2test.c.o

opt -enable-new-pm=0 -f -load ../../LogicLatch/Fuzzification/build/libCRCPass.so -crc-pass -S -o CMakeFiles/pcre2test.dir/src/pcre2test_crc.c.ll CMakeFiles/pcre2test.dir/src/pcre2test.c.o

clang -DSUPPORT_PCRE2_8 -o pcre2test_crc CMakeFiles/pcre2test.dir/src/pcre2test_crc.c.ll ../../LogicLatch/Fuzzification/crc.o ../../old/pcre2/build/CMakeFiles/pcre2-8-static.dir/src/pcre2_context.c.o ../../old/pcre2/build/CMakeFiles/pcre2-8-static.dir/src/pcre2_match_data.c.o ../../old/pcre2/build/CMakeFiles/pcre2-8-static.dir/pcre2_chartables.c.o -lpcre2-8 -lpcre2-posix -lreadline
```

### libpng

```bash
cmake .
make

clang -S -emit-llvm contrib/libtests/readpng.c -o contrib/libtests/readpng.ll
opt -enable-new-pm=0 -f -load ../LogicLatch/Fuzzification/build/libCRCPass.so -crc-pass -S -o contrib/libtests/readpng_crc.ll contrib/libtests/readpng.ll

clang -o contrib/libtests/readpng_crc contrib/libtests/readpng_crc.ll ../LogicLatch/Fuzzification/crc.o -I/usr/include/libpng16 -lpng
```

### libtiff

```bash
cmake .
make

# clang tools/tiffinfo.c -o tools/tiffinfo -I port/ -I libtiff -L./libtiff -ltiff

clang -S -emit-llvm tools/tiffinfo.c -o tools/tiffinfo.ll -I port/ -I libtiff
opt -enable-new-pm=0 -f -load ../LogicLatch/Fuzzification/build/libCRCPass.so -crc-pass -S -o tools/tiffinfo_crc.ll tools/tiffinfo.ll

clang -o tools/tiffinfo_crc tools/tiffinfo_crc.ll ../LogicLatch/Fuzzification/crc.o -L./libtiff -ltiff

export LD_LIBRARY_PATH=./libtiff:$LD_LIBRARY_PATH
```



