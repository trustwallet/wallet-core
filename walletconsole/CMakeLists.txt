# walletconsole executable
file(GLOB walletconsole_sources *.cpp)

add_executable(walletconsole ${walletconsole_sources})
target_link_libraries(walletconsole walletconsolelib TrezorCrypto TrustWalletCore CONAN_PKG::protobuf CONAN_PKG::boost)
target_include_directories(walletconsole PRIVATE ${CMAKE_SOURCE_DIR}/walletconsole/lib ${CMAKE_SOURCE_DIR}/src)
target_compile_options(walletconsole PRIVATE "-Wall")

set_target_properties(walletconsole
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

INSTALL(TARGETS walletconsole DESTINATION ${CMAKE_INSTALL_BINDIR})