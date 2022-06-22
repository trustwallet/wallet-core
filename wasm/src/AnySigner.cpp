// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include <TrustWalletCore/TWAnySigner.h>
#include <emscripten/bind.h>

#include "WasmData.h"
#include "Coin.h"

using namespace emscripten;

namespace TW::Wasm {

class AnySigner {
  public:
    static auto sign(const std::string& string, TWCoinType coin) {
        Data out;
        TW::anyCoinSign(coin, TW::data(string), out);
        return DataToVal(out);
    }

    static auto supportsJSON(TWCoinType coin) {
        return TW::supportsJSONSigning(coin);
    }

    static auto plan(const std::string& string, TWCoinType coin) {
        Data out;
        TW::anyCoinPlan(coin, TW::data(string), out);
        return DataToVal(out);
    }
};

EMSCRIPTEN_BINDINGS(Wasm_TWAnySigner) {
    class_<AnySigner>("AnySigner")
        .class_function("sign", &AnySigner::sign)
        .class_function("plan", &AnySigner::plan)
        .class_function("supportsJSON", &AnySigner::supportsJSON);
}

} // namespace TW::Wasm
