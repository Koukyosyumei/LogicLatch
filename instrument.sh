#!/bin/bash

# Check if the input argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <source_file>"
  exit 1
fi

INPUT_FILE=$1
mkdir "${INPUT_FILE%.*}"

clang++ -S -emit-llvm $INPUT_FILE -o "${INPUT_FILE%.*}/vanila.ll"
clang -emit-llvm -c -o "${INPUT_FILE%.*}/vanila.bc" "${INPUT_FILE%.*}/vanila.ll"
AFL_USE_ASAN=1 afl-clang-fast -c -o "${INPUT_FILE%.*}/vanila.o" "${INPUT_FILE%.*}/vanila.bc"
afl-clang-fast++ -o "${INPUT_FILE%.*}/vanila.out" "${INPUT_FILE%.*}/vanila.o" -fsanitize=address
# clang++ -o "${INPUT_FILE%.*}/vanila.o" "${INPUT_FILE%.*}/vanila.ll"

for p in 0.003 0.01 0.03; do
  for s in 1 2 3 4 5; do
    opt -enable-new-pm=0 -f -load ./build/libInsertSleepPass.so -insert-sleep -insert-sleep-seed $s -insert-sleep-probability $p -insert-sleep-time 3000 -insert-sleep-fname "${INPUT_FILE%.*}/sleep_${p}_${s}.block" -S -o "${INPUT_FILE%.*}/sleep_${p}_${s}.ll" "${INPUT_FILE%.*}/vanila.ll"
    clang -emit-llvm -c -o "${INPUT_FILE%.*}/sleep_${p}_${s}.bc" "${INPUT_FILE%.*}/sleep_${p}_${s}.ll"
    AFL_USE_ASAN=1 afl-clang-fast -c -o "${INPUT_FILE%.*}/sleep_${p}_${s}.o" "${INPUT_FILE%.*}/sleep_${p}_${s}.bc"
    afl-clang-fast++ -o "${INPUT_FILE%.*}/sleep_${p}_${s}.out" "${INPUT_FILE%.*}/sleep_${p}_${s}.o" -fsanitize=address
    # clang++ -o "${INPUT_FILE%.*}/sleep_${p}_${s}.o" "${INPUT_FILE%.*}/sleep_${p}_${s}.ll"
  done
done
