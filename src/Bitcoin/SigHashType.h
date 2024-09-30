// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

namespace TW::Bitcoin {

// Defines the number of bits of the hash type which is used to identify which
// outputs are signed.
static const uint32_t SigHashMask = 0x1f;

// Return the default HashType for the given coin, such as TWBitcoinSigHashTypeAll.
inline enum TWBitcoinSigHashType hashTypeForCoin(enum TWCoinType coinType) {
    // set fork hash type for BCH and XEC
    switch (coinType) {
        case TWCoinTypeBitcoinCash:
        case TWCoinTypeECash:
            return (TWBitcoinSigHashType)((uint32_t)TWBitcoinSigHashTypeAll | (uint32_t)TWBitcoinSigHashTypeFork);
        case TWCoinTypeBitcoinGold:
            return (TWBitcoinSigHashType)((uint32_t)TWBitcoinSigHashTypeAll | (uint32_t)TWBitcoinSigHashTypeForkBTG);
        default:
            return TWBitcoinSigHashTypeAll;
    }
}

inline bool hashTypeIsSingle(enum TWBitcoinSigHashType type) { return ((uint32_t)type & SigHashMask) == (uint32_t)TWBitcoinSigHashTypeSingle; }

inline bool hashTypeIsNone(enum TWBitcoinSigHashType type) { return ((uint32_t)type & SigHashMask) == (uint32_t)TWBitcoinSigHashTypeNone; }

} // namespace TW::Bitcoin
