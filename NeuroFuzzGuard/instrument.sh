#!/bin/bash

# Check if the input argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <source_file>"
  exit 1
fi

INPUT_FILE=$1
OUTPUT_ROOT_DIR=$2

_bname=$(basename "$INPUT_FILE")
FILE_BASENAME="${_bname%.*}"
OUTPUT_DIR="${OUTPUT_ROOT_DIR}/${FILE_BASENAME}"

mkdir $OUTPUT_DIR

clang++ -S -emit-llvm $INPUT_FILE -o "${OUTPUT_DIR}/vanila.ll"
clang -emit-llvm -c -o "${OUTPUT_DIR}/vanila.bc" "${OUTPUT_DIR}/vanila.ll"
AFL_USE_ASAN=1 afl-clang-fast -c -o "${OUTPUT_DIR}/vanila.o" "${OUTPUT_DIR}/vanila.bc"
afl-clang-fast++ -o "${OUTPUT_DIR}/vanila.out" "${OUTPUT_DIR}/vanila.o" -fsanitize=address
# clang++ -o "${OUTPUT_DIR}/vanila.o" "${OUTPUT_DIR}/vanila.ll"

for p in 0.003 0.01 0.03; do
  for s in 1 2 3 4 5; do
    opt -enable-new-pm=0 -f -load ./build/libInsertSleepPass.so -insert-sleep -insert-sleep-seed $s -insert-sleep-probability $p -insert-sleep-time 3000 -insert-sleep-fname "${OUTPUT_DIR}/sleep_${p}_${s}.block" -S -o "${OUTPUT_DIR}/sleep_${p}_${s}.ll" "${OUTPUT_DIR}/vanila.ll"
    clang -emit-llvm -c -o "${OUTPUT_DIR}/sleep_${p}_${s}.bc" "${OUTPUT_DIR}/sleep_${p}_${s}.ll"
    AFL_USE_ASAN=1 afl-clang-fast -c -o "${OUTPUT_DIR}/sleep_${p}_${s}.o" "${OUTPUT_DIR}/sleep_${p}_${s}.bc"
    afl-clang-fast++ -o "${OUTPUT_DIR}/sleep_${p}_${s}.out" "${OUTPUT_DIR}/sleep_${p}_${s}.o" -fsanitize=address
    # clang++ -o "${OUTPUT_DIR}/sleep_${p}_${s}.o" "${OUTPUT_DIR}/sleep_${p}_${s}.ll"
  done
done
