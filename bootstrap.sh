#!/usr/bin/env bash

# Fail if any commands fails
set -e

echo "#### Initializing... ####"
tools/install-dependencies

echo "#### Generating files... ####"
tools/generate-files

echo "#### Building... ####"
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug
make -Cbuild tests

echo "#### Testing... ####"
build/tests/tests tests
