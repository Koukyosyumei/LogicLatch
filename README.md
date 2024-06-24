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
opt -enable-new-pm=0 -f -load ./build/libInsertSleepPass.so -insert-sleep -insert-sleep-seed 42 -insert-sleep-probability 0.5 -S -o example/simpl
e_sleep.ll example/simple.ll
```

## Datasets

- [codeforces-code-dataset](https://www.kaggle.com/datasets/yeoyunsianggeremie/codeforces-code-dataset)
