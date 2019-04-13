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
    TWP2PKHPrefixBitcoin = 0x00,
    TWP2PKHPrefixDash = 0x4C,
    TWP2PKHPrefixDecred = 0x3f,
    TWP2PKHPrefixDogecoin = 0x1e,
    TWP2PKHPrefixGroestlcoin = 0x24,
    TWP2PKHPrefixLitecoin = 0x30,
    TWP2PKHPrefixQtum = 0x3a,
    TWP2PKHPrefixViacoin = 0x47,
    TWP2PKHPrefixZcashT = 0xB8,
    TWP2PKHPrefixZcoin = 0x52,
};

TW_EXTERN_C_END
