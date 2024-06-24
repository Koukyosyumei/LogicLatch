#!/bin/bash

# Check if the input argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <source_file>"
  exit 1
fi

INPUT_FILE=$1
mkdir "${INPUT_FILE%.*}"

clang++ -S -emit-llvm $INPUT_FILE -o "${INPUT_FILE%.*}/vanila.ll"
clang++ -o "${INPUT_FILE%.*}/vanila.o" "${INPUT_FILE%.*}/vanila.ll"

for p in 0.003 0.01 0.03; do
  for s in 1 2 3 4 5; do
    opt -enable-new-pm=0 -f -load ./build/libInsertSleepPass.so -insert-sleep -insert-sleep-seed $s -insert-sleep-probability $p -S -o "${INPUT_FILE%.*}/sleep_${p}_${s}.ll" "${INPUT_FILE%.*}/vanila.ll"
    clang++ -o "${INPUT_FILE%.*}/sleep_${p}_${s}.o" "${INPUT_FILE%.*}/sleep_${p}_${s}.ll"
  done
done
