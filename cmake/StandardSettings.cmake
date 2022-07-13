#
# Static analyzers
#
# Currently supporting: Clang-Tidy.

option(${PROJECT_NAME}_ENABLE_CLANG_TIDY "Enable static analysis with Clang-Tidy." OFF)

#
# Specific platforms support
#
# Currently supporting: Wasm.
option(${PROJECT_NAME}_USE_WASM "Target Wasm" OFF)
