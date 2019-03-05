// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PrivateKeySecp256k1.h"

#include "PublicKeySecp256k1.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/rand.h>
#include <TrezorCrypto/secp256k1.h>

using namespace TW;
using namespace TW::secp256k1;

PrivateKey::~PrivateKey() {
    std::fill(bytes.begin(), bytes.end(), 0);
}

std::vector<uint8_t> PrivateKey::getPublicKey(bool compressed) const {
    std::vector<uint8_t> result;
    if (compressed)  {
        result.resize(PublicKey::compressedSize);
        ecdsa_get_public_key33(&curve_secp256k1, bytes.data(), result.data());
     } else {
        result.resize(PublicKey::uncompressedSize);
        ecdsa_get_public_key65(&curve_secp256k1, bytes.data(), result.data());
     }

    return result;
}

std::array<uint8_t, 65> PrivateKey::sign(const std::vector<uint8_t>& digest) const {
    std::array<uint8_t, 65> result;
    bool success = ecdsa_sign_digest(&curve_secp256k1, bytes.data(), digest.data(), result.data(), result.data() + 64, NULL) == 0;
    if (!success) {
        return {};
    }
    return result;
}

std::vector<uint8_t> PrivateKey::signAsDER(const std::vector<uint8_t>&  digest) const {
    std::array<uint8_t, 64> sig;
    bool success = ecdsa_sign_digest(&curve_secp256k1, bytes.data(), digest.data(), sig.data(), NULL, NULL) == 0;
    if (!success) {
        return {};
    }

    uint8_t resultBytes[72];
    size_t size = ecdsa_sig_to_der(sig.data(), resultBytes);

    auto result = std::vector<uint8_t>{};
    std::copy(resultBytes, resultBytes + size, std::back_inserter(result));
    return result;
}
