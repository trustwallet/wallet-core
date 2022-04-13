// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include <TrustWalletCore/TWAnySigner.h>
#include <emscripten/bind.h>

#include "WASMData.h"
#include "Coin.h"

using namespace emscripten;

namespace TW::Wasm {

class AnySigner {
  public:
    static auto sign(const std::string& string, TWCoinType coin) {
        auto in = TW::data(string);
        Data out;
        TW::anyCoinSign(coin, in, out);
        return DataToVal(out);
    }

    static auto supportsJSON(TWCoinType coin) {
        return TW::supportsJSONSigning(coin);
    }
};

EMSCRIPTEN_BINDINGS(WASM_TWAnyAddress) {
    class_<AnySigner>("AnySigner")
        .class_function("sign", &AnySigner::sign)
        .class_function("supportsJSON", &AnySigner::supportsJSON);
}

} // namespace TW::Wasm
