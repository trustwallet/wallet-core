// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "../PublicKey.h"
#include "../PrivateKey.h"
#include "../Data.h"

namespace TW::Bitshares {

// Encrypts message using AES-256 in CBC mode and PKCS#5 padding
// The key must be 256 bits long and the IV 128 bits.
TW::Data aesEncrypt(const uint8_t *message, size_t messageLength, const uint8_t *key, const uint8_t *initializationVector);

template <typename T>
TW::Data aesEncrypt(const T& message, const uint8_t *key, const uint8_t *initializationVector) {
    return aesEncrypt(reinterpret_cast<const uint8_t*>(message.data()), message.size(), key, initializationVector);
}

class Memo {
public:
    PublicKey from, to;
    uint64_t nonce;
    Data encryptedMessage;

    // Nonce should not be set manually except for debugging
    Memo(const PrivateKey& senderKey, const PublicKey& recipientKey, const std::string& message, uint64_t nonce = 0);

    void serialize(Data& os) const noexcept;
    nlohmann::json serialize() const noexcept;

    static Data getSharedSecret(const PrivateKey& senderKey, const PublicKey& recipientKey);
};

} // namespace TW::Bitshares