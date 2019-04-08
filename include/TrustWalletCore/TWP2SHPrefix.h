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
    TWP2SHPrefixBitcoin = 0x05,
    TWP2SHPrefixLitecoin = 0x32,
    TWP2SHPrefixDash = 0x10,
    TWP2SHPrefixDecred = 0x1a,
    TWP2SHPrefixZcoin = 0x07,
    TWP2SHPrefixZcashT = 0xBD,
};

// Do not export TWP2SHPrefixGroestlcoin because it the same to
// TWP2SHPrefixBitcoin and causes problems in Java:
// public static P2SHPrefix createFromValue(byte value)
static const uint8_t TWP2SHPrefixGroestlcoin = 0x05;

TW_EXTERN_C_END
