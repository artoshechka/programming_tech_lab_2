cmake_minimum_required(VERSION 3.16)
project(unit LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC
    src/class_unit.cpp
    src/language_factories.cpp
    src/method_unit.cpp
    src/print_operator_unit.cpp
    src/unit.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)