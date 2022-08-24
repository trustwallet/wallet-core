#
# Default settings
#
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version" FORCE)

#
# IDE Settings
#
option(TW_IDE_CLION "Enable if your IDE is CLion" OFF)
option(TW_IDE_VSCODE "Enable if your IDE is VSCode" OFF)

#
# Build Settings
#
option(TW_UNITY_BUILD "Enable Unity build for TrustWalletCore and unit tests." OFF)

#
# Static analyzers
#
# Currently supporting: Clang-Tidy, PVS-Studio.
option(TW_ENABLE_CLANG_TIDY "Enable static analysis with Clang-Tidy." OFF)
option(TW_ENABLE_PVS_STUDIO "Enable static analysis with PVS-Studio." OFF)

#
# Runtime analyzers
#
# Currently supporting: Clang ASAN.
option(TW_CLANG_ASAN "Enable ASAN dynamic address sanitizer" OFF)

#
# Specific platforms support
#
# Currently supporting: Wasm.
option(TW_COMPILE_WASM "Target Wasm" OFF)

#
# Coverage
#
option(TW_CODE_COVERAGE "Enable coverage reporting" OFF)

#
# Compiler warnings options
#
option(TW_WARNINGS_AS_ERRORS "Compiler Options as Error" OFF)

#
# Compilation Speed options
#
option(TW_ENABLE_CCACHE "Enable the usage of Ccache, in order to speed up rebuild times." ON)

if (TW_ENABLE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if (CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
        message(STATUS "ccache activated")
    endif ()
endif ()

#
# Tests/Examples options
#
option(TW_UNIT_TESTS "Enable the unit tests of the project" ON)
option(TW_BUILD_EXAMPLES "Enable the examples builds of the project" ON)

if (ANDROID OR IOS_PLATFORM OR TW_COMPILE_WASM)
    set(TW_UNIT_TESTS OFF)
    set(TW_BUILD_EXAMPLES OFF)
endif()

if (TW_UNIT_TESTS)
    message(STATUS "Native unit tests activated")
else()
    message(STATUS "Native unit tests skipped")
endif()

if (TW_BUILD_EXAMPLES)
    message(STATUS "Native examples activated")
else()
    message(STATUS "Native examples skipped")
endif()


