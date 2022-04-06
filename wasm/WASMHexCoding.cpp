// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Data.h"
#include "HexCoding.h"
#include <emscripten/bind.h>
#include <tuple>

using namespace emscripten;

TW::Data parse_hex(const std::string& string) {
    return TW::parse_hex(string);
}

std::string HexEncoded(TW::Data& data) {
    return TW::hexEncoded(data);
}

EMSCRIPTEN_BINDINGS(wallet_hexcoding_module) {
    function("parseHex", &parse_hex);
    function("hexEncoded", &HexEncoded);
}
