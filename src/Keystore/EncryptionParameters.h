// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "AESParameters.h"
#include "Data.h"
#include "PBKDF2Parameters.h"
#include "ScryptParameters.h"
#include <TrustWalletCore/TWStoredKeyEncryption.h>
#include <TrustWalletCore/TWStoredKeyEncryptionLevel.h>

#include <nlohmann/json.hpp>
#include <string>
#include <variant>

namespace TW::Keystore {

/// Set of parameters used when encoding
struct EncryptionParameters {
    std::int32_t getKeyBytesSize() const noexcept {
        return cipherParams.mKeyLength;
    }

    std::string cipher() const noexcept {
        return cipherParams.mCipher;
    }

    /// Cipher parameters.
    AESParameters cipherParams = AESParameters();

    /// Key derivation function parameters.
    std::variant<ScryptParameters, PBKDF2Parameters> kdfParams = ScryptParameters();

    EncryptionParameters() = default;

    /// Initializes with standard values.
    EncryptionParameters(AESParameters cipherParams, std::variant<ScryptParameters, PBKDF2Parameters> kdfParams)
        : cipherParams(std::move(cipherParams)), kdfParams(std::move(kdfParams)) {
    }

    /// Initializes with a JSON object.
    explicit EncryptionParameters(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;

    EncryptionParameters(const EncryptionParameters& other) = default;
    EncryptionParameters(EncryptionParameters&& other) = default;
    EncryptionParameters& operator=(const EncryptionParameters& other) = default;
    EncryptionParameters& operator=(EncryptionParameters&& other) = default;

    /// Checks if the parameters should be fixed, i.e. if they are "valid" but do not meet the recommended security requirements,
    bool shouldFix() const;

    virtual ~EncryptionParameters() = default;
};

/// Errors thrown when decrypting a key.
enum class DecryptionError {
    unsupportedKDF,
    unsupportedCipher,
    unsupportedCoin,
    invalidKeyFile,
    invalidCipher,
    invalidPassword,
};

/// An encrypted payload data
struct EncryptedPayload {
public:
    EncryptionParameters params;

    /// Encrypted data.
    Data encrypted;

    /// Message authentication code.
    Data _mac;

    EncryptedPayload() = default;

    /// Initializes with standard values.
    EncryptedPayload(EncryptionParameters  params, Data encrypted, Data mac)
        : params(std::move(params))
        , encrypted(std::move(encrypted))
        , _mac(std::move(mac)) {}

    /// Initializes by encrypting data with a password using standard values.
    /// Note that we enforce to use Scrypt as KDF for new wallets encryption.
    EncryptedPayload(const Data& password, const Data& data, const AESParameters& cipherParams, const ScryptParameters& scryptParams);

    /// Initializes with a JSON object.
    explicit EncryptedPayload(const nlohmann::json& json);

    /// Decrypts the payload with the given password.
    Data decrypt(const Data& password) const;

    /// Regenerates the encrypted payload with new recommended encryption parameters.
    ///
    /// IMPORTANT: Due to a technical limitation, Scrypt parameters will be replaced with new recommended values,
    /// while PBKDF2 parameters will be left as is.
    /// See implementation for details.
    [[nodiscard]] EncryptedPayload regenerateWithRecommendedParams(const Data& password) const;

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;

    EncryptedPayload(const EncryptedPayload& other) = default;
    EncryptedPayload(EncryptedPayload&& other) = default;
    // Copy assignment is explicitly deleted.
    EncryptedPayload& operator=(const EncryptedPayload& other) = delete;
    EncryptedPayload& operator=(EncryptedPayload&& other) noexcept;

    virtual ~EncryptedPayload();
};

} // namespace TW::Keystore
