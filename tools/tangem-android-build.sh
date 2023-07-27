#!/usr/bin/env bash
#
# Initializes the workspace with dependencies, then performs full build

# Fail if any commands fails
set -e

echo "#### Initializing workspace with dependencies ... ####"
echo "Install system deps"
tools/install-sys-dependencies-mac
tools/install-rust-dependencies

echo "Install android deps"
tools/install-android-dependencies
tools/install-dependencies

echo "#### Generating android files ... ####"
tools/generate-files android

echo "#### Building android ... ####"
tools/android-build
