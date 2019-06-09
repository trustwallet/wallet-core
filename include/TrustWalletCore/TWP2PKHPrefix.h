// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Public key hash address prefix.
///
/// - SeeAlso: https://dash-docs.github.io/en/developer-reference#address-conversion
TW_EXPORT_ENUM(uint8_t)
enum TWP2PKHPrefix {
    TWP2PKHPrefixBitcoin = 0x00, // 0
    TWP2PKHPrefixDash = 0x4C, // 76
    TWP2PKHPrefixDecred = 0x3f, // 63
    TWP2PKHPrefixD = 0x1e, // Dogecoin and DigiByte 30
    TWP2PKHPrefixGroestlcoin = 0x24, // 36
    TWP2PKHPrefixLitecoin = 0x30, // 48
    TWP2PKHPrefixQtum = 0x3a, // 58
    TWP2PKHPrefixViacoin = 0x47, // 71
    TWP2PKHPrefixIocoin = 0x67, // 103
    TWP2PKHPrefixZcashT = 0xB8, // 184
    TWP2PKHPrefixZcoin = 0x52, // 82
    TWP2PKHPrefixMonetaryUnit = 0x10, // 16
    TWP2PKHPrefixRavencoin = 0x3c, // 60
};

TW_EXTERN_C_END
