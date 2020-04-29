// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>

namespace TW::Bitcoin {

inline bool hashTypeIsSingle(enum TWBitcoinSigHashType type) { return (type & 0x1f) == TWBitcoinSigHashTypeSingle; }

inline bool hashTypeIsNone(enum TWBitcoinSigHashType type) { return (type & 0x1f) == TWBitcoinSigHashTypeNone; }

} // namespace TW::Bitcoin
