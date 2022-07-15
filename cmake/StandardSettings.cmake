#
# Default settings
#
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version" FORCE)

#
# Static analyzers
#
# Currently supporting: Clang-Tidy.
option(TW_ENABLE_CLANG_TIDY "Enable static analysis with Clang-Tidy." OFF)

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
