# NeuroFuzzGuard

## Install

```bash
sh ./build.sh
```

## Usage

- feature extrament

```bash
sh extractor.sh example/simple.cpp
```

- instrument

```bash
clang++ -S -emit-llvm example/simple.cpp -o example/simple.ll
opt -enable-new-pm=0 -f -load ./build/libInsertSleepPass.so -insert-sleep -insert-sleep-seed 42 -insert-sleep-probability 0.5 -S -o example/simple_sleep.ll example/simple.ll
clang++ -o example/simple example/simple.ll
clang++ -o example/simple_sleep example/simple_sleep.ll
```

- afl

```bash
afl-fuzz -i ../afl-tutorial/afl-2.52b/testcases/others/text/ -Q -o out/ example/simple_sleep
```

## Datasets

- [codeforces-code-dataset](https://www.kaggle.com/datasets/yeoyunsianggeremie/codeforces-code-dataset)


```
clang -emit-llvm -c -o program.bc program.ll
AFL_USE_ASAN=1 afl-clang-fast -c -o program.o program.bc
afl-clang-fast++ -o program program.o -fsanitize=address
```
