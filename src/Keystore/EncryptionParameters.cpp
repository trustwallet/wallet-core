// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "EncryptionParameters.h"

#include "memory/memzero_wrapper.h"
#include "../Hash.h"

#include <TrezorCrypto/aes.h>
#include <TrezorCrypto/pbkdf2.h>
#include <TrezorCrypto/scrypt.h>
#include <cassert>

using namespace TW;

namespace TW::Keystore {

template <typename Iter>
static Data computeMAC(Iter begin, Iter end, const Data& key) {
    auto data = Data();
    data.reserve((end - begin) + key.size());
    data.insert(data.end(), begin, end);
    append(data, key);
    return Hash::keccak256(data);
}

// -----------------
// Encoding/Decoding
// -----------------

namespace CodingKeys {
static const auto encrypted = "ciphertext";
static const auto cipher = "cipher";
static const auto cipherParams = "cipherparams";
static const auto kdf = "kdf";
static const auto kdfParams = "kdfparams";
static const auto mac = "mac";
} // namespace CodingKeys

EncryptionParameters::EncryptionParameters(const nlohmann::json& json) {
    if (json.count(CodingKeys::cipher) == 0 || !json[CodingKeys::cipher].is_string()) {
        throw std::invalid_argument("Missing cipher");
    }
    if (json.count(CodingKeys::kdf) == 0 || !json[CodingKeys::kdf].is_string()) {
        throw std::invalid_argument("Missing kdf");
    }
    if (json.count(CodingKeys::cipherParams) == 0 || !json[CodingKeys::cipherParams].is_object()) {
        throw std::invalid_argument("Missing cipher params");
    }
    if (json.count(CodingKeys::kdfParams) == 0 || !json[CodingKeys::kdfParams].is_object()) {
        throw std::invalid_argument("Missing kdf params");
    }

    auto cipher = json[CodingKeys::cipher].get<std::string>();
    cipherParams = AESParameters::AESParametersFromJson(json[CodingKeys::cipherParams], cipher);
    if (const auto error = cipherParams.validate(); error.has_value()) {
        std::stringstream ss;
        ss << "Invalid cipher params: " << toString(*error);
        throw std::invalid_argument(ss.str());
    }

    auto kdf = json[CodingKeys::kdf].get<std::string>();
    if (kdf == "scrypt") {
        kdfParams = ScryptParameters(json[CodingKeys::kdfParams]);
    } else if (kdf == "pbkdf2") {
        kdfParams = PBKDF2Parameters(json[CodingKeys::kdfParams]);
    } else {
        throw std::invalid_argument("Unsupported kdf: " + kdf);
    }
}

nlohmann::json EncryptionParameters::json() const {
    nlohmann::json j;
    j[CodingKeys::cipher] = cipher();
    j[CodingKeys::cipherParams] = cipherParams.json();

    if (auto* scryptParams = std::get_if<ScryptParameters>(&kdfParams); scryptParams) {
        j[CodingKeys::kdf] = "scrypt";
        j[CodingKeys::kdfParams] = scryptParams->json();
    } else if (auto* pbkdf2Params = std::get_if<PBKDF2Parameters>(&kdfParams); pbkdf2Params) {
        j[CodingKeys::kdf] = "pbkdf2";
        j[CodingKeys::kdfParams] = pbkdf2Params->json();
    }

    return j;
}

bool EncryptionParameters::shouldFix() const {
    if (std::holds_alternative<ScryptParameters>(kdfParams)) {
        return std::get<ScryptParameters>(kdfParams).shouldFix();
    }

    // Note: re-encryption is currently supported for Scrypt only.
    return false;
}

EncryptedPayload::EncryptedPayload(const Data& password, const Data& data, const AESParameters& cipherParams, const ScryptParameters& scryptParams) {
    if (const auto error = cipherParams.validate(); error.has_value()) {
        std::stringstream ss;
        ss << "Invalid cipher params: " << toString(*error);
        throw std::invalid_argument(ss.str());
    }

    auto derivedKey = Data(scryptParams.desiredKeyLength);
    scrypt(reinterpret_cast<const byte*>(password.data()), password.size(), scryptParams.salt.data(),
           scryptParams.salt.size(), scryptParams.n, scryptParams.r, scryptParams.p, derivedKey.data(),
           scryptParams.desiredKeyLength);

    aes_encrypt_ctx ctx;
    auto result = 0;
    switch(cipherParams.mCipherEncryption) {
    case TWStoredKeyEncryptionAes128Ctr:
        result = aes_encrypt_key128(derivedKey.data(), &ctx);
        break;
    case TWStoredKeyEncryptionAes192Ctr:
        result = aes_encrypt_key192(derivedKey.data(), &ctx);
        break;
    case TWStoredKeyEncryptionAes256Ctr:
        result = aes_encrypt_key256(derivedKey.data(), &ctx);
        break;
    }
    assert(result == EXIT_SUCCESS);
    if (result == EXIT_SUCCESS) {
        Data iv = cipherParams.iv;
        // iv size should have been validated in `AESParameters::isValid()`.
        assert(iv.size() == gBlockSize);

        params = { cipherParams, scryptParams };
        encrypted = Data(data.size());
        aes_ctr_encrypt(data.data(), encrypted.data(), static_cast<int>(data.size()), iv.data(), aes_ctr_cbuf_inc, &ctx);
        _mac = computeMAC(derivedKey.end() - params.getKeyBytesSize(), derivedKey.end(), encrypted);
    }

    memzero(&ctx, sizeof(ctx));
    memzero(derivedKey.data(), derivedKey.size());
}

EncryptedPayload& EncryptedPayload::operator=(EncryptedPayload&& other) noexcept {
    if (this != &other) {
        memzero(encrypted.data(), encrypted.size());
        memzero(_mac.data(), _mac.size());

        params = std::move(other.params);
        encrypted = std::move(other.encrypted);
        _mac = std::move(other._mac);
    }
    return *this;
}

EncryptedPayload::~EncryptedPayload() {
    memzero(encrypted.data(), encrypted.size());
    memzero(_mac.data(), _mac.size());
}

Data EncryptedPayload::decrypt(const Data& password) const {
    auto derivedKey = Data();
    auto mac = Data();

    if (auto* scryptParams = std::get_if<ScryptParameters>(&params.kdfParams); scryptParams) {
        derivedKey.resize(scryptParams->defaultDesiredKeyLength);
        scrypt(password.data(), password.size(), scryptParams->salt.data(),
               scryptParams->salt.size(), scryptParams->n, scryptParams->r, scryptParams->p, derivedKey.data(),
               scryptParams->defaultDesiredKeyLength);
        mac = computeMAC(derivedKey.end() - params.getKeyBytesSize(), derivedKey.end(), encrypted);
    } else if (auto* pbkdf2Params = std::get_if<PBKDF2Parameters>(&params.kdfParams); pbkdf2Params) {
        derivedKey.resize(pbkdf2Params->defaultDesiredKeyLength);
        pbkdf2_hmac_sha256(password.data(), static_cast<int>(password.size()), pbkdf2Params->salt.data(),
                           static_cast<int>(pbkdf2Params->salt.size()), pbkdf2Params->iterations, derivedKey.data(),
                           pbkdf2Params->defaultDesiredKeyLength);
        mac = computeMAC(derivedKey.end() - params.getKeyBytesSize(), derivedKey.end(), encrypted);
    } else {
        throw DecryptionError::unsupportedKDF;
    }

    if (!isEqualConstantTime(mac, _mac)) {
        memzero(derivedKey.data(), derivedKey.size());
        throw DecryptionError::invalidPassword;
    }

    // Even though the cipher params should have been validated in `EncryptedPayload` constructor,
    // double check them here.
    if (params.cipherParams.validate().has_value()) {
        throw DecryptionError::invalidCipher;
    }
    assert(params.cipherParams.iv.size() == gBlockSize);

    Data decrypted(encrypted.size());
    Data iv = params.cipherParams.iv;
    const auto encryption = params.cipherParams.mCipherEncryption;
    if (encryption == TWStoredKeyEncryptionAes128Ctr
        || encryption == TWStoredKeyEncryptionAes192Ctr
        || encryption == TWStoredKeyEncryptionAes256Ctr) {
        aes_encrypt_ctx ctx;
        [[maybe_unused]] auto result = aes_encrypt_key(derivedKey.data(), params.getKeyBytesSize(), &ctx);
        assert(result != EXIT_FAILURE);

        aes_ctr_decrypt(encrypted.data(), decrypted.data(), static_cast<int>(encrypted.size()), iv.data(),
                        aes_ctr_cbuf_inc, &ctx);
        memzero(&ctx, sizeof(ctx));
        memzero(derivedKey.data(), derivedKey.size());
    } else {
        memzero(derivedKey.data(), derivedKey.size());
        throw DecryptionError::unsupportedCipher;
    }

    return decrypted;
}

EncryptedPayload EncryptedPayload::regenerateWithRecommendedParams(const Data& password) const {
    // IMPORTANT: `EncryptedPayload` constructor supports Scrypt encryption ONLY.
    // Hence, we can't regenerate PBKDF2 and re-encrypt a payload. Do nothing in that case.
    if (!std::holds_alternative<ScryptParameters>(params.kdfParams)) {
        return *this;
    }

    const auto decryptedData = ZeroizingData(decrypt(password));

    // Regenerate only necessary Scrypt parameters, while leaving other settings as is.
    const auto fixedScryptParams = std::get<ScryptParameters>(params.kdfParams).regenerateWithRecommendedParams();
    const auto cipherParams = params.cipherParams.copyWithNewIv();

    auto reEncryptedPayload = EncryptedPayload(password, decryptedData.get(), cipherParams, fixedScryptParams);

    // Try to decrypt the new payload to verify the full backward compatibility, before returning it.
    {
        auto reDecryptedData = ZeroizingData(reEncryptedPayload.decrypt(password));
        if (!isEqualConstantTime(decryptedData.get(), reDecryptedData.get())) {
            throw DecryptionError::invalidKeyFile;
        }
    }

    return reEncryptedPayload;
}

EncryptedPayload::EncryptedPayload(const nlohmann::json& json) {
    if (json.count(CodingKeys::encrypted) == 0 || !json[CodingKeys::encrypted].is_string()) {
        throw std::invalid_argument("Missing encrypted data");
    }
    if (json.count(CodingKeys::mac) == 0 || !json[CodingKeys::mac].is_string()) {
        throw std::invalid_argument("Missing mac");
    }

    params = EncryptionParameters(json);
    encrypted = parse_hex(json[CodingKeys::encrypted].get<std::string>());
    _mac = parse_hex(json[CodingKeys::mac].get<std::string>());
}

nlohmann::json EncryptedPayload::json() const {
    nlohmann::json j = params.json();
    j[CodingKeys::encrypted] = hex(encrypted);
    j[CodingKeys::mac] = hex(_mac);
    return j;
}

} // namespace TW::Keystore
