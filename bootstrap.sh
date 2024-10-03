#!/usr/bin/env bash
#
# Initializes the workspace with dependencies, then performs full build

# Fail if any commands fails
set -e

source tools/parse_args "$@"

if isHelp; then
  echo "usage: bootstrap.sh [-h | --help] [all | wasm | android | ios]"
  echo ""
  echo "Installs dependencies and prepares WalletCore for building"
  exit 0
fi

echo "#### Installing system dependencies ... ####"
if [[ $(uname) == "Darwin" ]]; then
  tools/install-sys-dependencies-macos
else
  tools/install-sys-dependencies-linux
fi

echo "#### Installing C++ libraries ... ####"
tools/install-dependencies

echo "#### Installing Rust toolchain and tools ... ####"
tools/install-rust-dependencies dev

# WASM
if isTargetSpecified "wasm"; then
  echo "#### Installing WASM environment ... ####"
  tools/install-wasm-dependencies
fi

# Android
if isTargetSpecified "android"; then
  echo "#### Installing Android dependencies ... ####"
  tools/install-android-dependencies
fi

echo "#### Generating files... ####"
tools/generate-files "$@"

echo ""
echo "WalletCore is ready for development!"
echo "Consider running native C++ tests via './tools/build-and-test'"
