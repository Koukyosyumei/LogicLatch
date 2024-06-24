#!/bin/bash

# Check if the input argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <source_file>"
  exit 1
fi

INPUT_FILE=$1

clang++ -S -emit-llvm $INPUT_FILE -o "${INPUT_FILE%.*}.ll"
opt -enable-new-pm=0 -f -load ./build/libInsertSleepPass.so -insert-sleep -insert-sleep-seed 42 -insert-sleep-probability 1.0 -S -o "${INPUT_FILE%.*}_sleep.ll" "${INPUT_FILE%.*}.ll"
clang++ -o "${INPUT_FILE%.*}" "${INPUT_FILE%.*}.ll"
clang++ -o "${INPUT_FILE%.*}_sleep" "${INPUT_FILE%.*}_sleep.ll"