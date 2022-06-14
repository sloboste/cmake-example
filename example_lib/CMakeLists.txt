cmake_minimum_required(VERSION 3.23)

include(CMakePrintHelpers)
include(FeatureSummary)
feature_summary(
    WHAT ALL
    VAR VERBOSE_FEATURE_SUMMARY
)
message(VERBOSE "${VERBOSE_FEATURE_SUMMARY}")

project(
    example_lib
    DESCRIPTION "Custom library code"
    LANGUAGES C
    VERSION 0.0.1
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Prefer shared libraries unless otherwise specified.
option(BUILD_SHARED_LIBS "Build using shared libraries" TRUE)

add_library(example_lib)

# Note: CMake doesn't always add -std=x to the compiler flags when it should so specify manually.
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
target_compile_options(
    example_lib
    PRIVATE -std=gnu11 -Wall -Werror -Wextra -Wpedantic
)
if((CMAKE_BUILD_TYPE STREQUAL "Debug") OR (CMAKE_BUILD_TYPE STREQUAL ""))
    target_compile_definitions(
        example_lib
        PRIVATE DEBUG
    )
    target_compile_options(
        example_lib
        PRIVATE -ggdb -O0
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(
        example_lib
        PRIVATE NDEBUG
    )
    target_compile_options(
        example_lib
        PRIVATE -ggdb -O3
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(
        example_lib
        PRIVATE NDEBUG
    )
    target_compile_options(
        example_lib
        PRIVATE -ggdb -O3
    )
else()
    message(FATAL_ERROR "Unknown build type ${CMAKE_BUILD_TYPE}")
endif()

set(
    EXAMPLE_LIB_HEADERS
    inc/example_lib/example.h
)
set(
    EXAMPLE_LIB_SOURCES
    src/example.c
)

target_include_directories(
    example_lib
    PUBLIC inc
)
target_sources(
    example_lib
    PUBLIC ${EXAMPLE_LIB_HEADERS}
    PRIVATE ${EXAMPLE_LIB_SOURCES}
)

# FIXME add tests

# FIXME move this to common helper file
add_custom_target(
    example_lib_clang_format_check
    COMMAND clang-format --style=file --dry-run ${EXAMPLE_LIB_HEADERS} ${EXAMPLE_LIB_SOURCES}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# FIXME move this to common helper file
add_custom_target(
    example_lib_clang_format_fix
    COMMAND clang-format --style=file -i ${EXAMPLE_LIB_HEADERS} ${EXAMPLE_LIB_SOURCES}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# FIXME move this to common helper file
# FIXME run in parallel
add_custom_target(
    example_lib_clang_tidy_check
    COMMAND clang-tidy
        --config-file="${PROJECT_SOURCE_DIR}/../.clang-tidy"
        -p ${CMAKE_CURRENT_BINARY_DIR}
        ${EXAMPLE_LIB_HEADERS} ${EXAMPLE_LIB_SOURCES}
    DEPENDS example_lib
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# FIXME move this to common helper file
# TODO Add example_lib_clang_tidy_fix

# FIXME move this to common helper file
add_custom_target(
    example_lib_lint
    COMMAND true # noop
    DEPENDS example_lib_clang_format_check example_lib_clang_tidy_check
)

# FIXME move this to common helper file
add_custom_target(
    example_lib_lint_fix
    COMMAND true # noop
    DEPENDS example_lib_clang_format_fix example_lib_clang_tidy
)

# Note: CMAKE_INSTALL_PREFIX defaults to /usr/local and DESTDIR becomes a prefix to the install
# directory. So, if you want to install in a non-standard location you need to set
# CMAKE_INSTALL_PREFIX at configure time.
install(
    FILES ${EXAMPLE_LIB_HEADERS}
    DESTINATION include/example_lib
)
install(
    TARGETS example_lib
    DESTINATION lib
)
