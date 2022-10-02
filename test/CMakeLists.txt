include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(${PROJECT_NAME}_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(${TEST_NAME}_DIR ${PROJECT_SOURCE_DIR}/test)
set(${TEST_NAME}_SRC_DIR ${${TEST_NAME}_DIR}/src)

set(${TEST_NAME}_SRC_LIST
    ${${PROJECT_NAME}_SRC_DIR}/parsers/SubParsers.cpp
    ${${PROJECT_NAME}_SRC_DIR}/parsers/TspTags.cpp
    )

set(${TEST_NAME}_SRC_LIST
    ${${TEST_NAME}_SRC_LIST}
    ${${TEST_NAME}_SRC_DIR}/main.cpp
    ${${TEST_NAME}_SRC_DIR}/parsers/ParserTest.cpp
    ${${TEST_NAME}_SRC_DIR}/parsers/SubParsersTest.cpp
    )

add_executable(${TEST_NAME} ${${TEST_NAME}_SRC_LIST})
target_include_directories(${TEST_NAME}
    PRIVATE
    ${${TEST_NAME}_SRC_DIR}
    ${${PROJECT_NAME}_SRC_DIR}
    PUBLIC
    ${${PROJECT_NAME}_INC_DIR}
    )
target_link_libraries(${TEST_NAME} PRIVATE gtest gtest_main)

