// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Coin type for Level 2 of BIP44.
///
/// - SeeAlso: https://github.com/satoshilabs/slips/blob/master/slip-0044.md
TW_EXPORT_ENUM(uint32_t)
enum TWCoinType {
    TWCoinTypeBinance = 714,
    TWCoinTypeBitcoin = 0,
    TWCoinTypeBitcoinCash = 145,
    TWCoinTypeCallisto = 820,
    TWCoinTypeDash = 5,
    TWCoinTypeEOS = 194,
    TWCoinTypeEthereum = 60,
    TWCoinTypeEthereumClassic = 61,
    TWCoinTypeGo = 6060,
    TWCoinTypeICON = 74,
    TWCoinTypeLitecoin = 2,
    TWCoinTypePoa = 178,
    TWCoinTypeThunderToken = 1001,
    TWCoinTypeTomoChain = 889,
    TWCoinTypeTron = 195,
    TWCoinTypeVeChain = 818,
    TWCoinTypeWanChain = 5718350,
    TWCoinTypeZcoin = 136,
    TWCoinTypeZcash = 133,
    TWCoinTypeRipple = 144,
};

TW_EXTERN_C_END
