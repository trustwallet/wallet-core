// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PublicKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>

using namespace TW;

PublicKey PublicKey::compressed() const {
    if (isCompressed()) {
        return *this;
    }

    std::array<uint8_t, compressedSize> newBytes;
    newBytes[0] = 0x02 | (bytes[64] & 0x01);
    std::copy(bytes.begin() + 1, bytes.begin() + compressedSize - 1, newBytes.begin() + 1);
    return PublicKey(newBytes);
}

bool PublicKey::verify(const std::vector<uint8_t>& signature, const std::vector<uint8_t>& message) const {
    return ecdsa_verify_digest(&secp256k1, bytes.data(), signature.data(), message.data()) == 0;
}
