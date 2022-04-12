// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include "Data.h"
#include "HexCoding.h"
#include <emscripten/bind.h>

using namespace emscripten;

namespace TW::Wasm {

class HexCoding {
  public:
    static auto parseHex(const std::string& string) -> Data {
        return TW::parse_hex(string, true);
    }

    static auto hexEncoded(const Data& data) -> std::string {
        return TW::hexEncoded(data);
    }

    static auto toBuffer(const Data& data) -> val {
        return val(typed_memory_view(data.size(), data.data()));
    }
};

EMSCRIPTEN_BINDINGS(WASM_HexCoding) {
    class_<HexCoding>("HexCoding")
        .class_function("decode", &HexCoding::parseHex)
        .class_function("encode", &HexCoding::hexEncoded)
        .class_function("toBuffer", &HexCoding::toBuffer);

    register_vector<TW::byte>("UInt8Vector");
}

} // namespace TW::Wasm
