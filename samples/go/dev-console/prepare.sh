#!/bin/bash

mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DTW_UNIT_TESTS=OFF -DTW_BUILD_EXAMPLES=OFF -DTW_UNITY_BUILD=ON -GNinja "$PWD"/../../../../
ninja
cp libTrustWalletCore.a ../native/packaged/lib/
cp libprotobuf.a ../native/packaged/lib
cp ../../../../build/local/lib/libwallet_core_rs.a ../native/packaged/lib
cp trezor-crypto/libTrezorCrypto.a ../native/packaged/lib
cd -
cp -R ../../../include native/packaged/
