#
# Static analyzers
#
# Currently supporting: Clang-Tidy.

option(TW_ENABLE_CLANG_TIDY "Enable static analysis with Clang-Tidy." OFF)

#
# Specific platforms support
#
# Currently supporting: Wasm.
option(TW_COMPILE_WASM "Target Wasm" OFF)
