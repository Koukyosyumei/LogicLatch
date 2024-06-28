cmake_minimum_required(VERSION 3.16.3)
project(NFG)

set(LLVM_INCLUDE_DIRS "/usr/include/llvm-14")
set(LLVM_C_INCLUDE_DIRS "/usr/include/llvm-c-14/")

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Set include directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories($(LLVM_C_INCLUDE_DIRS))
add_definitions(${LLVM_DEFINITIONS})

# Set LLVM libraries
llvm_map_components_to_libnames(llvm_libs core support irreader)

# Add executable
add_executable(extractor extractor.cpp)

# Link LLVM libraries
target_link_libraries(extractor ${llvm_libs})

