// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "EncryptionParameters.h"

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
    auto cipher = json[CodingKeys::cipher].get<std::string>();
    cipherParams = AESParameters::AESParametersFromJson(json[CodingKeys::cipherParams], cipher);

    auto kdf = json[CodingKeys::kdf].get<std::string>();
    if (kdf == "scrypt") {
        kdfParams = ScryptParameters(json[CodingKeys::kdfParams]);
    } else if (kdf == "pbkdf2") {
        kdfParams = PBKDF2Parameters(json[CodingKeys::kdfParams]);
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

EncryptedPayload::EncryptedPayload(const Data& password, const Data& data, const EncryptionParameters& params)
    : params(std::move(params)), _mac() {
    auto scryptParams = std::get<ScryptParameters>(this->params.kdfParams);
    auto derivedKey = Data(scryptParams.desiredKeyLength);
    scrypt(reinterpret_cast<const byte*>(password.data()), password.size(), scryptParams.salt.data(),
           scryptParams.salt.size(), scryptParams.n, scryptParams.r, scryptParams.p, derivedKey.data(),
           scryptParams.desiredKeyLength);

    aes_encrypt_ctx ctx;
    auto result = 0;
    switch(this->params.cipherParams.mCipherEncryption) {
    case TWStoredKeyEncryptionAes128Ctr:
    case TWStoredKeyEncryptionAes128Cbc:
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
        Data iv = this->params.cipherParams.iv;
        encrypted = Data(data.size());
        aes_ctr_encrypt(data.data(), encrypted.data(), static_cast<int>(data.size()), iv.data(), aes_ctr_cbuf_inc, &ctx);
        _mac = computeMAC(derivedKey.end() - params.getKeyBytesSize(), derivedKey.end(), encrypted);
    }
}

EncryptedPayload::~EncryptedPayload() {
    std::fill(encrypted.begin(), encrypted.end(), 0);
    std::fill(_mac.begin(), _mac.end(), 0);
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

    if (mac != _mac) {
        throw DecryptionError::invalidPassword;
    }

    Data decrypted(encrypted.size());
    Data iv = params.cipherParams.iv;
    const auto encryption = params.cipherParams.mCipherEncryption;
    if (encryption == TWStoredKeyEncryptionAes128Ctr || encryption == TWStoredKeyEncryptionAes256Ctr) {
        aes_encrypt_ctx ctx;
        [[maybe_unused]] auto result = aes_encrypt_key(derivedKey.data(), params.getKeyBytesSize(), &ctx);
        assert(result != EXIT_FAILURE);

        aes_ctr_decrypt(encrypted.data(), decrypted.data(), static_cast<int>(encrypted.size()), iv.data(),
                        aes_ctr_cbuf_inc, &ctx);
    } else if (encryption == TWStoredKeyEncryptionAes128Cbc) {
        aes_decrypt_ctx ctx;
        [[maybe_unused]] auto result = aes_decrypt_key(derivedKey.data(), params.getKeyBytesSize(), &ctx);
        assert(result != EXIT_FAILURE);

        for (auto i = 0ul; i < encrypted.size(); i += params.getKeyBytesSize()) {
            aes_cbc_decrypt(encrypted.data() + i, decrypted.data() + i, params.getKeyBytesSize(), iv.data(), &ctx);
        }
    } else {
        throw DecryptionError::unsupportedCipher;
    }

    return decrypted;
}

EncryptedPayload::EncryptedPayload(const nlohmann::json& json) {
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
