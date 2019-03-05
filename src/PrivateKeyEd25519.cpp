// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PrivateKeyEd25519.h"

#include "PublicKeyEd25519.h"

#include <TrezorCrypto/ed25519.h>

using namespace TW;
using namespace TW::ed25519;

PrivateKey::~PrivateKey() {
    std::fill(bytes.begin(), bytes.end(), 0);
}

std::array<uint8_t, 32> PrivateKey::getPublicKey() const {
    std::array<uint8_t, 32> result;
    ed25519_publickey(bytes.data(), result.data());
    return result;
}

std::array<uint8_t, 64> PrivateKey::sign(const std::vector<uint8_t>& digest) const {
    std::array<uint8_t, 64> result;
    auto pubkey = getPublicKey();
    ed25519_sign(digest.data(), digest.size(), bytes.data(), pubkey.data(), result.data());
    return result;
}
