#!/usr/bin/env bash
#
# Initializes the workspace with dependencies, then performs full build

# Fail if any commands fails
set -e

echo "#### Initializing workspace with dependencies ... ####"
tools/install-dependencies

echo "#### Building and running tests ... ####"
tools/build-and-test

echo "#### Installing... ####"
sudo cp build/libTrustWalletCore.a build/libprotobuf.a build/trezor-crypto/libTrezorCrypto.a /usr/local/lib
sudo ln -s ${PWD}/include/TrustWalletCore /usr/local/include/TrustWalletCore