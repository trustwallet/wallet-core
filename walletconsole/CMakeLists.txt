# walletconsole executable
file(GLOB walletconsole_sources *.cpp)
add_executable(walletconsole ${walletconsole_sources})
#target_link_libraries(tests gtest_main TrezorCrypto TrustWalletCore protobuf Boost::boost)
target_link_libraries(walletconsole walletconsolelib TrezorCrypto TrustWalletCore ${Protobuf_LIBRARIES} Boost::boost)
target_include_directories(walletconsole PRIVATE ${CMAKE_SOURCE_DIR}/walletconsole/lib ${CMAKE_SOURCE_DIR}/src)
if(NOT (WIN32 AND ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")))
    target_compile_options(walletconsole PRIVATE "-Wall")
endif()

set_target_properties(walletconsole
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

INSTALL(TARGETS walletconsole DESTINATION ${CMAKE_INSTALL_BINDIR})