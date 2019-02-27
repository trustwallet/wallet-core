#!/usr/bin/env bash

# Fail if any commands fails
set -e

echo "#### Initializing... ####"
git submodule update --init
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DGIT_SUBMODULE=OFF

echo "#### Generating files... ####"
make -Cbuild protobuf_ext
tools/generate-files

echo "#### Building... ####"
make -Cbuild tests

echo "#### Testing... ####"
build/tests/tests
