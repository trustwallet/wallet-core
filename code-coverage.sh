#!/usr/bin/env bash

# Fail if any commands fails
set -e

echo "#### Code coverage... ####"
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCODE_COVERAGE=ON
make -Cbuild -j12 tests
find . -name "*.gcda" -exec rm {} \;
./build/tests/tests tests --gtest_filter=*
rm -rf coverage.info coverage/
tools/coverage html
