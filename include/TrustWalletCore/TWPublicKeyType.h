// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Public key types
TW_EXPORT_ENUM(uint32_t)
enum TWPublicKeyType {
    TWPublicKeyTypeSECP256k1 = 0,
    TWPublicKeyTypeSECP256k1Extended = 1,
    TWPublicKeyTypeNIST256p1 = 2,
    TWPublicKeyTypeNIST256p1Extended = 3,
    TWPublicKeyTypeED25519 = 4,
    TWPublicKeyTypeED25519Blake2b = 5,
    TWPublicKeyTypeCURVE25519 = 6,
    TWPublicKeyTypeED25519Cardano = 7,
    TWPublicKeyTypeStarkex = 8,
};

TW_EXTERN_C_END
