cmake_minimum_required(VERSION 3.14)

project(TSP_READER VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(TEST_NAME ${PROJECT_NAME}_TEST)
set(${PROJECT_NAME}_INC_DIR ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME ${TEST_NAME} COMMAND Test)