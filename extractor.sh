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

clang++ -S -emit-llvm $INPUT_FILE -o "${OUTPUT_DIR}/${FILE_BASENAME}.ll"
# llvm-as "${INPUT_FILE%.*}.ll" -o "${INPUT_FILE%.*}.bc"

./build/extractor "${OUTPUT_DIR}/${FILE_BASENAME}.ll"