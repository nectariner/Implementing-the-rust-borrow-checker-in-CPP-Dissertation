# A Partial Runtime Implementation of The "Rust Borrow Checker" in C++ - Fred Cook (8463955)
This document contains all code associated with my dissertation project.

# Instructions for running (on linux)
## C++
### Requirements
- Clang
- Cmake
- Google test (Gtest) library

### Steps
- `cd ./cpp`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
- `./internal_tests && ./tests`

### Notes
- The C++ files follow the `Microsoft` style guides

## Rust
### Requirements
- Cargo (https://www.rust-lang.org/tools/install)

### Steps
- `cd ./rust/borrow_checker`
- `cargo t` or `cargo test` (these two are equivalent)
- Cargo will then pull all dependencies, handle the compilation process and run the test(s) located in `./rust/borrow_checker/src/lib.rs`

### Notes
- Rust files format the standard Rust guidelines defined by `rustfmt`

## Steps to make use of the C++ implementation in your own project
- Simply `#include` it to your own project
- e.g. `#include "../lib/borrow_checker.hpp"`
- then compile as normal
