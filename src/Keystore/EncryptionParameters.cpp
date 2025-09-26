// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "EncryptionParameters.h"

#include "../Hash.h"
#include "../sodium_utils.h"

#include <cassert>
#include <TrustWalletCore/TWAESPaddingMode.h>
#include "TrustWalletCore/Generated/TWCrypto.h"

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

static Data rustScrypt(const Data& password, const ScryptParameters& params) {
    Rust::TWDataWrapper passwordData = password;
    Rust::TWDataWrapper saltData = params.salt;

    Rust::TWDataWrapper res = Rust::crypto_scrypt(
        passwordData.get(),
        saltData.get(),
        params.n,
        params.r,
        params.p,
        params.desiredKeyLength
    );
    auto data = res.toDataOrDefault();
    if (data.empty()) {
        throw std::runtime_error("Invalid scrypt parameters");
    }
    return data;
}

static Data rustPbkdf2(const Data& password, const PBKDF2Parameters& params) {
    Rust::TWDataWrapper passwordData = password;
    Rust::TWDataWrapper saltData = params.salt;

    Rust::TWDataWrapper res = Rust::tw_pbkdf2_hmac_sha256(
        passwordData.get(),
        saltData.get(),
        params.iterations,
        params.desiredKeyLength
    );
    auto data = res.toDataOrDefault();
    if (data.empty()) {
        throw std::runtime_error("Invalid pbkdf2 parameters");
    }
    return data;
}
template <typename CryptoFunc>
static Data rustAesOperation(const Data& data, const Data& iv, const Data& key, CryptoFunc cryptoFunc, const char* errorMsg) {
    Rust::TWDataWrapper dataWrapper = data;
    Rust::TWDataWrapper ivWrapper = iv;
    Rust::TWDataWrapper keyWrapper = key;

    Rust::TWDataWrapper res = cryptoFunc(
        keyWrapper.get(),
        dataWrapper.get(),
        ivWrapper.get()
    );
    auto resData = res.toDataOrDefault();
    if (resData.empty()) {
        throw std::runtime_error(errorMsg);
    }
    return resData;
}

static Data rustAesCtrEncrypt128(const Data& data, const Data& iv, const Data& key) {
    return rustAesOperation(data, iv, key, Rust::tw_aes_encrypt_ctr_128, "Invalid aes ctr encrypt 128");
}

static Data rustAesCtrDecrypt128(const Data& data, const Data& iv, const Data& key) {
    return rustAesOperation(data, iv, key, Rust::tw_aes_decrypt_ctr_128, "Invalid aes ctr decrypt 128");
}

static Data rustAesCtrEncrypt192(const Data& data, const Data& iv, const Data& key) {
    return rustAesOperation(data, iv, key, Rust::tw_aes_encrypt_ctr_192, "Invalid aes ctr encrypt 192");
}

static Data rustAesCtrDecrypt192(const Data& data, const Data& iv, const Data& key) {
    return rustAesOperation(data, iv, key, Rust::tw_aes_decrypt_ctr_192, "Invalid aes ctr decrypt 192");
}

static Data rustAesCtrEncrypt256(const Data& data, const Data& iv, const Data& key) {
    return rustAesOperation(data, iv, key, Rust::tw_aes_encrypt_ctr_256, "Invalid aes ctr encrypt 256");
}

static Data rustAesCtrDecrypt256(const Data& data, const Data& iv, const Data& key) {
    return rustAesOperation(data, iv, key, Rust::tw_aes_decrypt_ctr_256, "Invalid aes ctr decrypt 256");
}

static Data rustAesCbcEncrypt128(const Data& data, const Data& iv, const Data& key) {
    return rustAesOperation(data, iv, key, 
        [](auto d, auto i, auto k) { return Rust::tw_aes_encrypt_cbc_128(d, i, k, TWAESPaddingModePKCS7); }, 
        "Invalid aes cbc encrypt 128");
}

static Data rustAesCbcDecrypt128(const Data& data, const Data& iv, const Data& key) {
    return rustAesOperation(data, iv, key, 
        [](auto d, auto i, auto k) { return Rust::tw_aes_decrypt_cbc_128(d, i, k, TWAESPaddingModePKCS7); }, 
        "Invalid aes cbc decrypt 128");
}

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
    auto derivedKey = rustScrypt(password, scryptParams);

    switch(this->params.cipherParams.mCipherEncryption) {
    case TWStoredKeyEncryptionAes128Ctr:
        encrypted = rustAesCtrEncrypt128(data, this->params.cipherParams.iv, derivedKey);
        break;
    case TWStoredKeyEncryptionAes192Ctr:
        encrypted = rustAesCtrEncrypt192(data, this->params.cipherParams.iv, derivedKey);
        break;
    case TWStoredKeyEncryptionAes256Ctr:
        encrypted = rustAesCtrEncrypt256(data, this->params.cipherParams.iv, derivedKey);
        break;
    case TWStoredKeyEncryptionAes128Cbc:
        encrypted = rustAesCbcEncrypt128(data, this->params.cipherParams.iv, derivedKey);
        break;
    }
    _mac = computeMAC(derivedKey.end() - params.getKeyBytesSize(), derivedKey.end(), encrypted);
}

EncryptedPayload::~EncryptedPayload() {
    std::fill(encrypted.begin(), encrypted.end(), 0);
    std::fill(_mac.begin(), _mac.end(), 0);
}

Data EncryptedPayload::decrypt(const Data& password) const {
    auto derivedKey = Data();
    auto mac = Data();

    if (auto* scryptParams = std::get_if<ScryptParameters>(&params.kdfParams); scryptParams) {
        derivedKey = rustScrypt(password, *scryptParams);
        mac = computeMAC(derivedKey.end() - params.getKeyBytesSize(), derivedKey.end(), encrypted);
    } else if (auto* pbkdf2Params = std::get_if<PBKDF2Parameters>(&params.kdfParams); pbkdf2Params) {
        derivedKey = rustPbkdf2(password, *pbkdf2Params);
        mac = computeMAC(derivedKey.end() - params.getKeyBytesSize(), derivedKey.end(), encrypted);
    } else {
        throw DecryptionError::unsupportedKDF;
    }

    if (sodium_memcmp(mac.data(), _mac.data(), mac.size()) != 0) {
        throw DecryptionError::invalidPassword;
    }

    Data decrypted(encrypted.size());
    Data iv = params.cipherParams.iv;
    switch (params.cipherParams.mCipherEncryption) {
    case TWStoredKeyEncryptionAes128Ctr:
        decrypted = rustAesCtrDecrypt128(encrypted, iv, derivedKey);
        break;
    case TWStoredKeyEncryptionAes192Ctr:
        decrypted = rustAesCtrDecrypt192(encrypted, iv, derivedKey);
        break;
    case TWStoredKeyEncryptionAes256Ctr:
        decrypted = rustAesCtrDecrypt256(encrypted, iv, derivedKey);
        break;
    case TWStoredKeyEncryptionAes128Cbc:
        decrypted = rustAesCbcDecrypt128(encrypted, iv, derivedKey);
        break;
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
