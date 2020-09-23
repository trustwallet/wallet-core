project(WalletCoreSwift Swift)

file(GLOB_RECURSE SWIFT_SOURCES Sources/*)
file(GLOB_RECURSE SWIFT_TEST_SOURCES Tests/*)

add_library(framework ${SWIFT_SOURCES} TrustWalletCore)
add_executable(framework_test ${SWIFT_TEST_SOURCES} framework TrustWalletCore)
