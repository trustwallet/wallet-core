// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#include <emscripten/bind.h>

#include "WasmData.h"
#include "HexCoding.h"

using namespace emscripten;

namespace TW::Wasm {

class HexCoding {
  public:
    static auto parseHex(const std::string& string) {
        return DataToVal(TW::parse_hex(string, true));
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
