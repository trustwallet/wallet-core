#!/usr/bin/env bash
#
# Initializes the workspace with dependencies, then performs full build

# Fail if any commands fails
set -e

echo "#### Initializing workspace with dependencies ... ####"
tools/install-dependencies
tools/install-rust-dependencies

echo "#### Building and running tests ... ####"
tools/build-and-test
