// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Elliptic cruves
TW_EXPORT_ENUM()
enum TWCurve {
    TWCurveSECP256k1 = 0,             // "secp256k1"
    TWCurveED25519 = 1,               // "ed25519"
    TWCurveED25519Blake2bNano = 2,    // "ed25519-blake2b-nano"
    TWCurveCurve25519 = 3,            // "curve25519"
    TWCurveNIST256p1 = 4,             // "nist256p1"
    TWCurveED25519Extended = 5,       // "ed25519 cardano seed" with extended keys, used by e.g. Cardano
};

TW_EXTERN_C_END
