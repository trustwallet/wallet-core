# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

file(GLOB wasm_sources src/*.cpp src/generated/*.cpp)
file(GLOB wasm_headers src/*.h src/generated/*.h)
set(TARGET_NAME wallet-core)
set(CMAKE_EXECUTABLE_SUFFIX ".js")
add_executable(${TARGET_NAME} ${wasm_sources} ${wasm_headers})

target_link_libraries(${TARGET_NAME} TrustWalletCore)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/trezor-crypto/include)
target_compile_options(${TARGET_NAME} PRIVATE "-Wall")

set_target_properties(${TARGET_NAME}
        PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
set_target_properties(${TARGET_NAME} 
        PROPERTIES 
        COMPILE_FLAGS "-O3 -s USE_BOOST_HEADERS=1" 
        LINK_FLAGS "--bind --no-entry --closure=1 -O3 -s ALLOW_MEMORY_GROWTH=1 -s ERROR_ON_UNDEFINED_SYMBOLS=1"
)
