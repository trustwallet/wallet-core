// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "AESParameters.h"
#include "ScryptParameters.h"
#include "../Data.h"

#include <nlohmann/json.hpp>
#include <string>

namespace TW {
namespace Keystore {

/// Errors thrown when decrypting a key.
enum class DecryptionError {
    unsupportedKDF,
    unsupportedCipher,
    unsupportedCoin,
    invalidKeyFile,
    invalidCipher,
    invalidPassword,
};

struct EncryptionParameters {
    /// Encrypted data.
    Data encrypted;

    /// Cipher algorithm.
    std::string cipher = "aes-128-ctr";

    /// Cipher parameters.
    AESParameters cipherParams = AESParameters();

    /// Key derivation function, must be scrypt.
    std::string kdf = "scrypt";

    /// Key derivation function parameters.
    ScryptParameters kdfParams = ScryptParameters();

    /// Message authentication code.
    Data mac;

    EncryptionParameters() = default;

    /// Initializes `EncryptionParameters` with standard values.
    EncryptionParameters(const Data& encrypted, const AESParameters& cipherParams, const ScryptParameters& kdfParams, const Data& mac)
        : encrypted(encrypted)
        , cipherParams(cipherParams)
        , kdfParams(kdfParams)
        , mac(mac)
    {}

    /// Initializes `EncryptionParameters` by encrypting data with a password using standard values.
    EncryptionParameters(const std::string& password, Data data);

    /// Initializes `EncryptionParameters` with a JSON object.
    EncryptionParameters(const nlohmann::json& json);

    /// Decrypts the payload with the given password.
    Data decrypt(const std::string& password) const;

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;

    EncryptionParameters(const EncryptionParameters& other) = default;
    EncryptionParameters(EncryptionParameters&& other) = default;
    EncryptionParameters& operator =(const EncryptionParameters& other) = default;
    EncryptionParameters& operator =(EncryptionParameters&& other) = default;

    virtual ~EncryptionParameters();
};

}} // namespace
