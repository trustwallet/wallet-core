# walletconsolelib library
file(GLOB_RECURSE walletconsolelib_sources *.cpp)
add_library(walletconsolelib ${walletconsolelib_sources})
#target_link_libraries(tests gtest_main TrezorCrypto TrustWalletCore protobuf Boost::boost)
#target_link_libraries(walletconsolelib TrezorCrypto TrustWalletCore protobuf Boost::boost)
target_include_directories(walletconsolelib PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_compile_options(walletconsolelib PRIVATE "-Wall")

set_target_properties(walletconsolelib
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
