// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Key.h"
#include "Random.h"

namespace TW::Arweave {

static const auto TargetLength = 4096 / 8;

PrivateKey keyGenerateFromPrngSeed(const Data& rngSeed) {
    // TODO dummy placeholder implementation, using directly output from the PRNG
    RandomGenerator rng = RandomGenerator(rngSeed);
    const auto keyData = rng.nextBytes(TargetLength);
    return PrivateKey(keyData);
}

} // namespace
