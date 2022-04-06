# wasm

file(GLOB wasm_sources *.cpp)
file(GLOB wasm_sources_head *.h)
add_executable(wasm ${wasm_sources} ${wasm_sources_head})
target_link_libraries(wasm TrustWalletCore)
target_include_directories(wasm PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/trezor-crypto/include)
target_compile_options(wasm PRIVATE "-Wall")

set_target_properties(wasm
        PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
set_target_properties(wasm PROPERTIES COMPILE_FLAGS "-s USE_BOOST_HEADERS=1" LINK_FLAGS "-s USE_BOOST_HEADERS=1")
