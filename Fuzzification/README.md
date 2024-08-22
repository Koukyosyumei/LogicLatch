# Fuzzification

```bash
cmake -S . -B build
cmake --build build
opt -enable-new-pm=0 -f -load ./build/libBranchExtractorPass.so -branchextractor -S -o example/simple_sleep.ll example/simple.ll
```

## Preparation

- pcre2test

```
IF(PCRE2_BUILD_TESTS)
  ENABLE_TESTING()

  SET(PCRE2TEST_SOURCES src/pcre2test.c)

  set(CMAKE_C_COMPILER "clang")
  *set(CMAKE_C_FLAGS "-flto")
  *set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-flto")

  IF(MSVC)
    # This is needed to avoid a stack overflow error in the standard tests. The
    # flag should be indicated with a forward-slash instead of a hyphen, but
    # then CMake treats it as a file path.
    SET(PCRE2TEST_LINKER_FLAGS -STACK:2500000)
  ENDIF(MSVC)

  ADD_EXECUTABLE(pcre2test ${PCRE2TEST_SOURCES})
  *target_compile_options(pcre2test PUBLIC ${CMAKE_C_FLAGS} -flto)
  set(targets ${targets} pcre2test)
```