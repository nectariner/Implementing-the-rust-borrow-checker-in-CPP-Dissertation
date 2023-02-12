cmake_minimum_required(VERSION 3.12)
project(RustBorrowChecker CXX)
set(DEPENDENCIES -lgtest )
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall --std=c++20 -Wpedantic")
set(CMAKE_CXX_COMPILER "clang++")

add_library(borrow_checker borrow_checker.hpp)
set_target_properties(borrow_checker PROPERTIES LINKER_LANGUAGE CXX)

#add_executable(internal_tests borrow_checker_tests.cpp)
#target_link_libraries(borrow_checker ${DEPENDENCIES})

include(GoogleTest)
add_executable(tests test.cpp ${FLAGS})
target_link_libraries(tests borrow_checker ${DEPENDENCIES})
gtest_add_tests(TARGET tests)

add_executable(internal_tests borrow_checker_tests.cpp ${FLAGS})
target_link_libraries(internal_tests borrow_checker ${DEPENDENCIES})
gtest_add_tests(TARGET internal_tests)
