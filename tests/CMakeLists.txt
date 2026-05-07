cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED PROGRAM_GENERATOR_BUILD_TESTS)
    set(PROGRAM_GENERATOR_BUILD_TESTS ${BUILD_TESTING})
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(create_test)

create_test(codegen_tests
    code_factory_test.cpp
    LINK_LIBS
        unit
)

if(NOT TARGET build_tests)
    add_custom_target(build_tests)
endif()

if(TARGET program_generator_all_tests)
    add_dependencies(build_tests program_generator_all_tests)
endif()
