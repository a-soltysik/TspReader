set(${PROJECT_NAME}_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE ${PROJECT_NAME}_SRC_LIST
    ${${PROJECT_NAME}_SRC_DIR}/*.h
    ${${PROJECT_NAME}_SRC_DIR}/*.cpp
    )

file(GLOB_RECURSE ${PROJECT_NAME}_INC_LIST
    ${${PROJECT_NAME}_INC_DIR}/*.h
    )

add_library(${PROJECT_NAME}
    STATIC
    ${${PROJECT_NAME}_SRC_LIST}
    )

target_sources(${PROJECT_NAME}
    PRIVATE
    ${${PROJECT_NAME}_SRC_LIST}
    )

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${${PROJECT_NAME}_SRC_DIR}
    PUBLIC
    ${${PROJECT_NAME}_INC_DIR}
    )