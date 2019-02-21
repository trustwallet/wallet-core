#!/usr/bin/env bash

# Fail if any commands fails
set -e

echo "#### Updating submodules... ####"
git submodule update --init

echo "#### Generating files... ####"
tools/generate-files

echo "#### Building... ####"
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DGIT_SUBMODULE=OFF
make -C build tests

echo "#### Testing... ####"
build/tests/tests
