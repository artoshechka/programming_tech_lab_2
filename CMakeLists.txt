cmake_minimum_required(VERSION 3.12)
project(program_generator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
option(PROGRAM_GENERATOR_BUILD_TESTS "Build unit tests for program_generator" ${BUILD_TESTING})

if(PROGRAM_GENERATOR_BUILD_TESTS)
    set(BUILD_TESTING ON CACHE BOOL "Enable CTest-based testing" FORCE)
endif()

if(WIN32)
    set(CMAKE_PREFIX_PATH "")
endif()

add_subdirectory(unit)

add_compile_options(-Wno-return-type)

if(PROGRAM_GENERATOR_BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_executable(${PROJECT_NAME}
  main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    unit
)