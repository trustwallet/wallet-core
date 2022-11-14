// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    static TWStoredKeyEncryption getCipher(const std::string& cipher) {
        if (cipher == "aes-128-ctr") {
            return TWStoredKeyEncryption::TWAes128Ctr;
        } else if (cipher == "aes-192-ctr") {
            return TWStoredKeyEncryption::TWAes192Ctr;
        } else if (cipher == "aes-256-ctr") {
            return TWStoredKeyEncryption::TWAes256Ctr;
        }
        return TWAes128Ctr;
    }

    static AESSize getAesKeylength(TWStoredKeyEncryption cipher) {
        switch (cipher) {
        case TWAes128Ctr:
            return AESSize::A128;
        case TWAes128Cbc:
            return AESSize::A128;
        case TWAes192Ctr:
            return AESSize::A192;
        case TWAes256Ctr:
            return AESSize::A256;
        default:
            return AESSize::A128;
        }
    }

    static EncryptionParameters getPreset(enum TWStoredKeyEncryptionLevel preset, enum TWStoredKeyEncryption encryption = TWAes128Ctr) {
        switch (preset) {
        case TWStoredKeyEncryptionLevelMinimal:
            return EncryptionParameters(AESParameters(getAesKeylength(encryption)), ScryptParameters::Minimal, encryption);
        case TWStoredKeyEncryptionLevelWeak:
        case TWStoredKeyEncryptionLevelDefault:
        default:
            return EncryptionParameters(AESParameters(getAesKeylength(encryption)), ScryptParameters::Weak, encryption);
        case TWStoredKeyEncryptionLevelStandard:
            return EncryptionParameters(AESParameters(getAesKeylength(encryption)), ScryptParameters::Standard, encryption);
        }
    }

    std::int32_t getKeyBytesSize() const noexcept {
        return cipherParams.mKeyLength;
    }

    std::string cipher() const noexcept {
        switch (mCipher) {
        case TWAes128Ctr:
            return "aes-128-ctr";
        case TWAes192Ctr:
            return "aes-192-ctr";
        case TWAes256Ctr:
            return "aes-256-ctr";
        default:
            return "aes-128-ctr";
        }
    }

    /// Cipher algorithm.
    TWStoredKeyEncryption mCipher{TWAes128Ctr};

    /// Cipher parameters.
    AESParameters cipherParams = AESParameters();

    /// Key derivation function parameters.
    std::variant<ScryptParameters, PBKDF2Parameters> kdfParams = ScryptParameters();

    EncryptionParameters() = default;

    /// Initializes with standard values.
    EncryptionParameters(AESParameters cipherParams, std::variant<ScryptParameters, PBKDF2Parameters> kdfParams, TWStoredKeyEncryption cipher = TWAes128Ctr)
        : mCipher(cipher), cipherParams(std::move(cipherParams)), kdfParams(std::move(kdfParams)) {
    }

    /// Initializes with a JSON object.
    EncryptionParameters(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;

    EncryptionParameters(const EncryptionParameters& other) = default;
    EncryptionParameters(EncryptionParameters&& other) = default;
    EncryptionParameters& operator=(const EncryptionParameters& other) = default;
    EncryptionParameters& operator=(EncryptionParameters&& other) = default;

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
    EncryptedPayload(const EncryptionParameters& params, const Data& encrypted, const Data& mac)
        : params(std::move(params))
        , encrypted(std::move(encrypted))
        , _mac(std::move(mac)) {}

    /// Initializes by encrypting data with a password
    /// using standard values.
    EncryptedPayload(const Data& password, const Data& data, const EncryptionParameters& params);

    /// Initializes with a JSON object.
    EncryptedPayload(const nlohmann::json& json);

    /// Decrypts the payload with the given password.
    Data decrypt(const Data& password) const;

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;

    EncryptedPayload(const EncryptedPayload& other) = default;
    EncryptedPayload(EncryptedPayload&& other) = default;
    EncryptedPayload& operator=(const EncryptedPayload& other) = default;
    EncryptedPayload& operator=(EncryptedPayload&& other) = default;

    virtual ~EncryptedPayload();
};

} // namespace TW::Keystore
