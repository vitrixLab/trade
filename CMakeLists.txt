cmake_minimum_required(VERSION 3.20)

project(TradingAnalysisEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MANIFEST_BUILD_TESTS "Build the GoogleTest test suite" ON)
option(MANIFEST_ENABLE_ASAN_UBSAN "Enable AddressSanitizer and UndefinedBehaviorSanitizer in Debug builds" ON)
option(MANIFEST_ENABLE_TSAN "Enable ThreadSanitizer (separate from ASan/UBSan)" OFF)

if(MANIFEST_ENABLE_TSAN AND MANIFEST_ENABLE_ASAN_UBSAN)
    message(FATAL_ERROR "ThreadSanitizer must use a separate build; disable MANIFEST_ENABLE_ASAN_UBSAN.")
endif()

if(MANIFEST_ENABLE_TSAN)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options("$<$<CONFIG:Debug>:-fsanitize=thread>" "$<$<CONFIG:Debug>:-fno-omit-frame-pointer>")
        add_link_options("$<$<CONFIG:Debug>:-fsanitize=thread>")
    else()
        message(WARNING "ThreadSanitizer is not configured for this compiler.")
    endif()
elseif(MANIFEST_ENABLE_ASAN_UBSAN)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options("$<$<CONFIG:Debug>:-fsanitize=address,undefined>" "$<$<CONFIG:Debug>:-fno-omit-frame-pointer>")
        add_link_options("$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
    else()
        message(STATUS "ASan/UBSan flags are not configured for this compiler.")
    endif()
endif()

find_package(OpenSSL REQUIRED)

include(cmake/Dependencies.cmake)

add_library(manifest_foundation
    src/Scanner.cpp
    src/Hasher.cpp
)

target_include_directories(manifest_foundation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(manifest_foundation
    PUBLIC
        OpenSSL::Crypto
)

target_compile_features(manifest_foundation PUBLIC cxx_std_20)

if(UNIX AND NOT APPLE)
    target_compile_definitions(manifest_foundation PRIVATE _GNU_SOURCE)
elseif(APPLE)
    target_compile_definitions(manifest_foundation PRIVATE _DARWIN_C_SOURCE)
endif()

if(MSVC)
    target_compile_options(manifest_foundation PRIVATE /W4 /permissive-)
else()
    target_compile_options(manifest_foundation PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(MANIFEST_BUILD_TESTS)
    include(CTest)
    enable_testing()

    add_executable(manifest_foundation_tests
        tests/ScannerTests.cpp
        tests/HasherTests.cpp
    )

    target_link_libraries(manifest_foundation_tests
        PRIVATE
            manifest_foundation
            GTest::gtest_main
    )

    if(MSVC)
        target_compile_options(manifest_foundation_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(manifest_foundation_tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    endif()

    include(GoogleTest)
    gtest_discover_tests(manifest_foundation_tests)
endif()
