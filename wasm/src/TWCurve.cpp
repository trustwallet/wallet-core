// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <TrustWalletCore/TWCurve.h>

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(WASM_TWCurve) {
    enum_<TWCurve>("Curve")
        .value("SECP256k1", TWCurve::TWCurveSECP256k1)
        .value("ED25519", TWCurve::TWCurveED25519)
        .value("ED25519Blake2bNano", TWCurve::TWCurveED25519Blake2bNano)
        .value("Curve25519", TWCurve::TWCurveCurve25519)
        .value("NIST256p1", TWCurve::TWCurveNIST256p1)
        .value("ED25519Extended", TWCurve::TWCurveED25519Extended)
        .value("None", TWCurve::TWCurveNone);
}
