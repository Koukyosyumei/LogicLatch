# NeuroFuzzGuard


## Install

### dependencies

- LLVM: LLVM-14 or above

### build

```bash
sh ./build.sh
```

## Usage

- feature extraction

```bash
sh extractor.sh ./example/simple.cpp ./example
```

- instrument

```bash
clang++ -S -emit-llvm example/simple.cpp -o example/simple.ll
opt -enable-new-pm=0 -f -load ./build/libInsertSleepPass.so -insert-sleep -insert-sleep-seed 42 -insert-sleep-probability 0.5 -S -o example/simple_sleep.ll example/simple.ll
clang++ -o example/simple example/simple.ll
clang++ -o example/simple_sleep example/simple_sleep.ll

# afl-fuzz -i ../afl-tutorial/afl-2.52b/testcases/others/text/ -Q -o out/ example/simple_sleep
```

- instrument with afl

```bash
clang++ -S -emit-llvm example/simple.cpp -o example/simple.ll
opt -enable-new-pm=0 -f -load ./build/libInsertSleepPass.so -insert-sleep -insert-sleep-seed 42 -insert-sleep-probability 0.5 -S -o example/simple_sleep.ll example/simple.ll
clang -emit-llvm -c -o example/simple_sleep.bc example/simple_sleep.ll
AFL_USE_ASAN=1 afl-clang-fast -c -o example/simple_sleep.o example/simple_sleep.bc
afl-clang-fast++ -o example/simple_sleep.out example/simple_sleep.o -fsanitize=address

afl-fuzz -i ../afl-tutorial/afl-2.52b/testcases/others/text/ -o out/ example/simple_sleep
```

## Datasets

- [codeforces-code-dataset](https://www.kaggle.com/datasets/yeoyunsianggeremie/codeforces-code-dataset)
