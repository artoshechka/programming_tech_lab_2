cmake_minimum_required(VERSION 3.16)
project(unit LANGUAGES CXX)

add_subdirectory(src/common)
add_subdirectory(src/cpp)
add_subdirectory(src/java)
add_subdirectory(src/csharp)

add_library(${PROJECT_NAME} STATIC
    src/language_factories.cpp
    src/unit.cpp
    ${UNIT_COMMON_SOURCES}
    ${UNIT_CPP_SOURCES}
    ${UNIT_JAVA_SOURCES}
    ${UNIT_CSHARP_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)