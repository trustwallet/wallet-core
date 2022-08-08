#!/usr/bin/env bash
#
# Initializes the workspace with dependencies, then performs full build

# Fail if any commands fails
set -e

echo "#### Initializing workspace ... ####"
tools/install-dependencies

echo "#### Building and running ... ####"
tools/build-and-test
