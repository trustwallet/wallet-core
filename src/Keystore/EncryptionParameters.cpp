// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EncryptionParameters.h"

#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/aes.h>
#include <TrezorCrypto/pbkdf2.h>
#include <TrezorCrypto/scrypt.h>

#include <boost/variant/get.hpp>
#include <cassert>

using namespace TW;
using namespace TW::Keystore;

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
    cipher = json[CodingKeys::cipher].get<std::string>();
    cipherParams = AESParameters(json[CodingKeys::cipherParams]);

    auto kdf = json[CodingKeys::kdf].get<std::string>();
    if (kdf == "scrypt") {
        kdfParams = ScryptParameters(json[CodingKeys::kdfParams]);
    } else if (kdf == "pbkdf2") {
        kdfParams = PBKDF2Parameters(json[CodingKeys::kdfParams]);
    }
}

nlohmann::json EncryptionParameters::json() const {
    nlohmann::json j;
    j[CodingKeys::cipher] = cipher;
    j[CodingKeys::cipherParams] = cipherParams.json();

    if (kdfParams.which() == 0) {
        auto scryptParams = boost::get<ScryptParameters>(kdfParams);
        j[CodingKeys::kdf] = "scrypt";
        j[CodingKeys::kdfParams] = scryptParams.json();
    } else if (kdfParams.which() == 1) {
        auto pbkdf2Params = boost::get<PBKDF2Parameters>(kdfParams);
        j[CodingKeys::kdf] = "pbkdf2";
        j[CodingKeys::kdfParams] = pbkdf2Params.json();
    }

    return j;
}

EncryptedPayload::EncryptedPayload(const Data& password, const Data& data, const EncryptionParameters& params) :
    params(std::move(params)), mac() {
    auto scryptParams = boost::get<ScryptParameters>(params.kdfParams);
    auto derivedKey = Data(scryptParams.desiredKeyLength);
    scrypt(reinterpret_cast<const byte*>(password.data()), password.size(), scryptParams.salt.data(),
           scryptParams.salt.size(), scryptParams.n, scryptParams.r, scryptParams.p, derivedKey.data(),
           scryptParams.desiredKeyLength);

    aes_encrypt_ctx ctx;
    auto result = aes_encrypt_key128(derivedKey.data(), &ctx);
    assert(result == EXIT_SUCCESS);
    if (result == EXIT_SUCCESS) {
        Data iv = params.cipherParams.iv;
        encrypted = Data(data.size());
        aes_ctr_encrypt(data.data(), encrypted.data(), static_cast<int>(data.size()), iv.data(), aes_ctr_cbuf_inc, &ctx);

        mac = computeMAC(derivedKey.end() - 16, derivedKey.end(), encrypted);
    }
}

EncryptedPayload::~EncryptedPayload() {
    std::fill(encrypted.begin(), encrypted.end(), 0);
    std::fill(mac.begin(), mac.end(), 0);
}

Data EncryptedPayload::decrypt(const Data& password) const {
    auto derivedKey = Data();
    auto mac = Data();

    if (params.kdfParams.which() == 0) {
        auto scryptParams = boost::get<ScryptParameters>(params.kdfParams);
        derivedKey.resize(scryptParams.defaultDesiredKeyLength);
        scrypt(password.data(), password.size(), scryptParams.salt.data(),
            scryptParams.salt.size(), scryptParams.n, scryptParams.r, scryptParams.p, derivedKey.data(),
            scryptParams.defaultDesiredKeyLength);
        mac = computeMAC(derivedKey.end() - 16, derivedKey.end(), encrypted);
    } else if (params.kdfParams.which() == 1) {
        auto pbkdf2Params = boost::get<PBKDF2Parameters>(params.kdfParams);
        derivedKey.resize(pbkdf2Params.defaultDesiredKeyLength);
        pbkdf2_hmac_sha256(password.data(), static_cast<int>(password.size()), pbkdf2Params.salt.data(),
            static_cast<int>(pbkdf2Params.salt.size()), pbkdf2Params.iterations, derivedKey.data(),
            pbkdf2Params.defaultDesiredKeyLength);
        mac = computeMAC(derivedKey.end() - 16, derivedKey.end(), encrypted);
    } else {
        throw DecryptionError::unsupportedKDF;
    }

    if (mac != this->mac) {
        throw DecryptionError::invalidPassword;
    }

    Data decrypted(encrypted.size());
    Data iv = params.cipherParams.iv;
    if (params.cipher == "aes-128-ctr") {
        aes_encrypt_ctx ctx;
        auto __attribute__((unused)) result = aes_encrypt_key(derivedKey.data(), 16, &ctx);
        assert(result != EXIT_FAILURE);

        aes_ctr_decrypt(encrypted.data(), decrypted.data(), static_cast<int>(encrypted.size()), iv.data(),
                        aes_ctr_cbuf_inc, &ctx);
    } else if (params.cipher == "aes-128-cbc") {
        aes_decrypt_ctx ctx;
        auto __attribute__((unused)) result = aes_decrypt_key(derivedKey.data(), 16, &ctx);
        assert(result != EXIT_FAILURE);

        for (auto i = 0; i < encrypted.size(); i += 16) {
            aes_cbc_decrypt(encrypted.data() + i, decrypted.data() + i, 16, iv.data(), &ctx);
        }
    } else {
        throw DecryptionError::unsupportedCipher;
    }

    return decrypted;
}

EncryptedPayload::EncryptedPayload(const nlohmann::json& json) {
    params = EncryptionParameters(json);
    encrypted = parse_hex(json[CodingKeys::encrypted].get<std::string>());
    mac = parse_hex(json[CodingKeys::mac].get<std::string>());
}

nlohmann::json EncryptedPayload::json() const {
    nlohmann::json j = params.json();
    j[CodingKeys::encrypted] = hex(encrypted);
    j[CodingKeys::mac] = hex(mac);
    return j;
}
