// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

namespace TW::Bitshares {
// Encrypts message using AES-256 in CBC mode and PKCS#5 padding
// The key must be 256 bits long and the IV 128 bits.
TW::Data aesEncrypt(const uint8_t *message, size_t messageLength, const uint8_t *key, const uint8_t *initializationVector);

// A templated wrapper for the above funciton
template <typename T1, typename T2, typename T3>
TW::Data aesEncrypt(const T1& message, const T2& key, const T3& initializationVector) {
    return aesEncrypt(reinterpret_cast<const uint8_t*>(message.data()), message.size(), reinterpret_cast<const uint8_t*>(key.data()), reinterpret_cast<const uint8_t*>(initializationVector.data()));
}
} // namespace TW::Bitshares