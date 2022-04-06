// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Base58.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

std::string base58Encode(const Data& data) {
    return Base58::bitcoin.encode(data);
}

Data base58Decode(const std::string& string) {
    return Base58::bitcoin.decode(string);
}

std::string base58EncodeCheck(const Data& data) {
    return Base58::bitcoin.encodeCheck(data);
}

Data base58DecodeCheck(const std::string& string) {
    return Base58::bitcoin.decodeCheck(string);
}

EMSCRIPTEN_BINDINGS(wallet_base58_module) {
    function("base58Encode", &base58Encode);
    function("base58Decode", &base58Decode);
    function("base58EncodeCheck", &base58EncodeCheck);
    function("base58DecodeCheck", &base58DecodeCheck);
}
