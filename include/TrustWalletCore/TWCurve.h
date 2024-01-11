// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Elliptic cruves
TW_EXPORT_ENUM()
enum TWCurve {
    TWCurveSECP256k1              /* "secp256k1" */,
    TWCurveED25519                /* "ed25519" */,
    TWCurveED25519Blake2bNano     /* "ed25519-blake2b-nano" */,
    TWCurveCurve25519             /* "curve25519" */,
    TWCurveNIST256p1              /* "nist256p1" */,
    TWCurveED25519ExtendedCardano /* "ed25519-cardano-seed" */,
    TWCurveStarkex                /* "starkex" */,
    TWCurveNone
};

TW_EXTERN_C_END
