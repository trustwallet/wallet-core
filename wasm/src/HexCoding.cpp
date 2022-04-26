// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
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
