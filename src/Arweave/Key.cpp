// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Key.h"

namespace TW::Arweave {

static const auto TargetLength = 4096 / 8;

Data keyGenerateFromPrngSeed(const Data& rngSeed) {
    // TODO dummy placeholder implementation
    Data dummy;
    while (dummy.size() < TargetLength) {
        append(dummy, rngSeed);
    }
    const auto keyData = subData(dummy, 0, TargetLength);
    // TODO: return PrivateKey
    return keyData;
}

} // namespace
