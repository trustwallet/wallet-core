// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

#include <nlohmann/json.hpp>
#include <TrustWalletCore/TWStoredKeyEncryption.h>

namespace TW::Keystore {

enum AESKeySize : std::int32_t {
    Uninitialized = 0,
    A128 = 16,
    A192 = 24,
    A256 = 32,
};

inline constexpr std::size_t gBlockSize{16};
inline constexpr const char* gAes128Ctr{"aes-128-ctr"};
inline constexpr const char* gAes128Cbc{"aes-128-cbc"};
inline constexpr const char* gAes192Ctr{"aes-192-ctr"};
inline constexpr const char* gAes256Ctr{"aes-256-ctr"};

// AES128/192/256 parameters.
struct AESParameters {
    // For AES, your block length is always going to be 128 bits/16 bytes
    std::int32_t mBlockSize{gBlockSize};
    std::int32_t mKeyLength{A128};
    std::string  mCipher{gAes128Ctr};
    TWStoredKeyEncryption mCipherEncryption{TWStoredKeyEncryptionAes128Ctr};
    Data iv;

    /// Initializes `AESParameters` with a encryption cipher.
    static AESParameters AESParametersFromEncryption(TWStoredKeyEncryption encryption);;

    /// Initializes `AESParameters` with a JSON object.
    static AESParameters AESParametersFromJson(const nlohmann::json& json, const std::string& cipher);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

} // namespace TW::Keystore
