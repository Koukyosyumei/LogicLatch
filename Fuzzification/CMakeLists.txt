cmake_minimum_required(VERSION 3.10)
project(CRCPass)

set(CMAKE_CXX_STANDARD 14)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(CRCPass MODULE CRCPass.cpp)
add_library(BranchExtractorPass MODULE BranchExtractorPass.cpp)

target_link_libraries(CRCPass PRIVATE LLVMCore LLVMSupport LLVMIRReader LLVMInstCombine LLVMScalarOpts)
target_link_libraries(BranchExtractorPass PRIVATE LLVMCore LLVMSupport LLVMIRReader LLVMInstCombine LLVMScalarOpts)
