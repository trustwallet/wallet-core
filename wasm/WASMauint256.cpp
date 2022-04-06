// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "uint256.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

uint256_t loadUint256FromData(const Data& data) {
    return load(data);
}

uint256_t loadUint256FromString(const std::string& data) {
    return load(data);
}

EMSCRIPTEN_BINDINGS(wallet_uint256_module) {
    function("loadUint256FromData", &loadUint256FromData);
    function("loadUint256FromString", &loadUint256FromString);
    function("loadUint256WithOffset", &loadWithOffset);
    function("storeUint256", &store);
    function("encode256BE", &encode256BE);
    function("uint256ToString", &toString);
}
