// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Public key types
TW_EXPORT_ENUM(uint32_t)
enum TWPublicKeyType {
    TWPublicKeyTypeSECP256k1 = 0, // 'Compressed' Secp256k1 key, with 1-byte Y coordinate sign (33 bytes)
    TWPublicKeyTypeSECP256k1Extended = 1, // Extended Secp256k1 key (65 bytes)
    TWPublicKeyTypeNIST256p1 = 2,
    TWPublicKeyTypeNIST256p1Extended = 3,
    TWPublicKeyTypeED25519 = 4,
    TWPublicKeyTypeED25519Blake2b = 5,
    TWPublicKeyTypeCURVE25519 = 6,
    TWPublicKeyTypeED25519Extended = 7,
    TWPublicKeyTypeSECP256k1Compact = 8, // Secp256pk1 with implicit Y coordinate, for Taproot (32 bytes)
};

TW_EXTERN_C_END
