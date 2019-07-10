#!/usr/bin/env bash

# Fail if any commands fails
set -e

echo "#### Initializing... ####"
tools/install-dependencies

echo "#### Generating files... ####"
tools/generate-files

echo "#### Building... ####"
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug
make -Cbuild tests TrezorCryptoTests

if [ -x "$(command -v clang-tidy)" ]; then
    echo "#### Linting... ####"
    tools/lint
fi

echo "#### Testing... ####"
export CK_TIMEOUT_MULTIPLIER=4
build/trezor-crypto/tests/TrezorCryptoTests

ROOT="`dirname \"$0\"`"
TESTS_ROOT="`(cd \"$ROOT/tests\" && pwd)`"
build/tests/tests "$TESTS_ROOT"
