// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Stellar address version byte.
TW_EXPORT_ENUM(uint16_t)
enum TWStellarVersionByte {
    TWStellarVersionByteAccountID = 0x30,   // G
    TWStellarVersionByteSeed = 0xc0,        // S
    TWStellarVersionBytePreAuthTX = 0xc8,   // T
    TWStellarVersionByteSHA256Hash = 0x118, // X
};

TW_EXTERN_C_END
