cmake_minimum_required(VERSION 3.18)

project(dydx_governance_example)

set(CMAKE_CXX_STANDARD 17)

# Find TrustWalletCore
find_library(TRUST_WALLET_CORE TrustWalletCore PATHS ${CMAKE_SOURCE_DIR}/../../build)
find_path(TRUST_WALLET_CORE_INCLUDE TrustWalletCore PATHS ${CMAKE_SOURCE_DIR}/../../include)

# Include directories
include_directories(${TRUST_WALLET_CORE_INCLUDE})
include_directories(${CMAKE_SOURCE_DIR}/../../src)

# Create executable
add_executable(dydx_governance dydx_governance.cpp)

# Link libraries
target_link_libraries(dydx_governance ${TRUST_WALLET_CORE})

# Copy to build directory for easy execution
set_target_properties(dydx_governance PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)