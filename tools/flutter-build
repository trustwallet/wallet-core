#!/bin/bash

set -e

mkdir -p build

pushd build
cmake .. -DFLUTTER=ON
make -j12
popd

if [ "$(uname -s)" == "Darwin" ]; then
    cp build/libTrustWalletCore.dylib flutter/lib/
elif [ "$(uname -s)" == "Linux" ]; then
    cp build/libTrustWalletCore.so flutter/lib/
elif [ "$(uname -s)" == "Windows" ]; then
    cp build/libTrustWalletCore.dll flutter/lib/
fi

pushd flutter

echo "Generating bindings..."
dart run ffigen --config config.yaml


echo "Verifying the build..."
dart run

echo "Done"

popd
