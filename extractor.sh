#!/bin/bash

# Check if the input argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <source_file>"
  exit 1
fi

INPUT_FILE=$1

clang++ -S -emit-llvm $INPUT_FILE -o "${INPUT_FILE%.*}.ll"
llvm-as "${INPUT_FILE%.*}.ll" -o "${INPUT_FILE%.*}.bc"

./build/extractor "${INPUT_FILE%.*}.ll"