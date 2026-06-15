cmake_minimum_required(VERSION 4.2)
project(EC8)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(expenses_app src/main.cpp)
target_include_directories(expenses_app PRIVATE include)

add_executable(expenses_tests tests/test_expenses.cpp)
target_include_directories(expenses_tests PRIVATE include)

enable_testing()
add_test(NAME expenses_tests COMMAND expenses_tests)
