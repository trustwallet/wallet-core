// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PrivateKey.h"

namespace TW::Arweave {

/// Generate a private key deterministically, using pseudo-random generation with the given input seed.
/// Typically seed should be 64 bits long, and key is 4096-bit RSA key.
PrivateKey keyGenerateFromPrngSeed(const Data& rngSeed);

} // namespace
