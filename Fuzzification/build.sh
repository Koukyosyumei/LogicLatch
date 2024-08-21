mkdir build
cd build
cmake ..
make

ls

clang++ -shared -fPIC -o libCRCPass.so -I /usr/include/llvm -L /usr/include/llvm -lLLVM ../CRCPass.cpp
clang++ -shared -fPIC -o libBranchExtractorPass.so -I /usr/include/llvm -L /usr/include/llvm -lLLVM ../BranchExtractorPass.cpp