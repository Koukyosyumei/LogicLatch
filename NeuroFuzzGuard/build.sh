cd build
cmake ..
make

clang++ -shared -fPIC -o libInsertSleepPass.so -I /usr/include/llvm -L /usr/include/llvm -lLLVM ../InsertSleepPass.cpp