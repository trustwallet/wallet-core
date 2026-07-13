// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#include <emscripten/bind.h>
#include <TrezorCrypto/memzero.h>

#include "WasmData.h"
#include "HexCoding.h"

using namespace emscripten;

namespace TW::Wasm {

class HexCoding {
  public:
    static auto parseHex(const std::string& string) {
        auto data = TW::parse_hex(string, true);
        auto result = DataToVal(data);
        if (!data.empty()) { memzero(data.data(), data.size()); }
        return result;
    }

    static auto hexEncoded(const std::string& string) {
        auto data = TW::data(string);
        return TW::hexEncoded(data);
    }
};

EMSCRIPTEN_BINDINGS(Wasm_HexCoding) {
    class_<HexCoding>("HexCoding")
        .class_function("decode", &HexCoding::parseHex)
        .class_function("encode", &HexCoding::hexEncoded);
}

} // namespace TW::Wasm
