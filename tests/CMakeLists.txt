cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED PROGRAM_GENERATOR_BUILD_TESTS)
    set(PROGRAM_GENERATOR_BUILD_TESTS ${BUILD_TESTING})
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(create_test)

include(FetchContent)

if(NOT TARGET GTest::gtest_main)
    set(INSTALL_GTEST OFF CACHE BOOL "Disable gtest install step" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "Use shared CRT on Windows" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()
endif()

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
