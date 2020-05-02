// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>

namespace TW::Bitcoin {

// Defines the number of bits of the hash type which is used to identify which
// outputs are signed.
static const uint8_t SigHashMask = 0x1f;

inline bool hashTypeIsSingle(enum TWBitcoinSigHashType type) { return (type & SigHashMask) == TWBitcoinSigHashTypeSingle; }

inline bool hashTypeIsNone(enum TWBitcoinSigHashType type) { return (type & SigHashMask) == TWBitcoinSigHashTypeNone; }

} // namespace TW::Bitcoin
