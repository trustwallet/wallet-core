// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include <TrezorCrypto/hmac_drbg.h>

namespace TW::Arweave {

/// Interface for a PRNG
class RandomGeneratorInterface {
public:
    /// Generate next n bytes
    virtual Data nextBytes(size_t size) = 0;
};

/// Hmac-DRBG-based random number generator
class RandomGenerator : RandomGeneratorInterface {
public:
    RandomGenerator(const Data& seed) {
        hmac_drbg_init(&ctx, seed.data(), seed.size(), nullptr, 0);
    }
    virtual Data nextBytes(size_t size) {
        Data buf(size);
        hmac_drbg_generate(&ctx, buf.data(), buf.size());
        return buf;
    }
private:
    HMAC_DRBG_CTX ctx;
};

} // namespace
