#!/usr/bin/env bash

# Fail if any commands fails
set -e

echo "#### Initializing... ####"
tools/install-dependencies
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug

echo "#### Generating files... ####"
tools/generate-files

echo "#### Building... ####"
make -Cbuild tests

echo "#### Testing... ####"
build/tests/tests tests
