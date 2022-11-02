cmake_minimum_required(VERSION 3.14)

project(TSP_READER VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_REL_CXX_FLAGS "/O2 /MD")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Wpessimizing-move -Wredundant-move -pedantic -Wconversion")
    set(COMPILER_REL_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMPILER_REL_CXX_FLAGS}")

set(TEST_NAME ${PROJECT_NAME}_TEST)
set(${PROJECT_NAME}_INC_DIR ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME ${TEST_NAME} COMMAND Test)