// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Pay to script hash (P2SH) address prefix.
///
/// - SeeAlso: https://en.bitcoin.it/wiki/List_of_address_prefixes
TW_EXPORT_ENUM(uint8_t)
enum TWP2SHPrefix {
    TWP2SHPrefixBitcoin = 0x05, // 5
    TWP2SHPrefixDash = 0x10, // 16
    TWP2SHPrefixDecred = 0x1a, // 26
    TWP2SHPrefixDogecoin = 0x16, // 22
    TWP2SHPrefixLitecoin = 0x32, // 50
    TWP2SHPrefixViacoin = 0x21, // 33
    TWP2SHPrefixIocoin = 0x55, // 85
    TWP2SHPrefixZcashT = 0xBD, // 189
    TWP2SHPrefixZcoin = 0x07, // 7
    TWP2SHPrefixS = 0x3F, // Lux and DigiByte 63
    TWP2SHPrefixMonetaryUnit = 0x4C, // 76
    TWP2SHPrefixRavencoin = 0x7a, // 122
};

// Do not export TWP2SHPrefixGroestlcoin because it the same to
// TWP2SHPrefixBitcoin and causes problems in Java:
// public static P2SHPrefix createFromValue(byte value)
static const uint8_t TWP2SHPrefixGroestlcoin = 0x05;

TW_EXTERN_C_END
