// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EdPrivateKey.h"

#include <TrezorCrypto/ed25519.h>

using namespace TW;
using namespace TW::Nimiq;

EdPrivateKey::~EdPrivateKey() {
    std::fill(bytes.begin(), bytes.end(), 0);
}

std::array<uint8_t, 32> EdPrivateKey::getPublicKey() const {
    std::array<uint8_t, 32> result;
    ed25519_publickey(bytes.data(), result.data());
    return result;
}

std::array<uint8_t, 64> EdPrivateKey::sign(const std::vector<uint8_t>& digest) const {
    std::array<uint8_t, 64> result;
    ed25519_sign(digest.data(), digest.size(), bytes.data(), getPublicKey().data(), result.data());
    return result;
}
