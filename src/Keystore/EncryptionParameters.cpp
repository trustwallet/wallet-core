// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EncryptionParameters.h"

#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/aes.h>
#include <TrezorCrypto/scrypt.h>

#include <cassert>

using namespace TW;
using namespace TW::Keystore;

template <typename Iter>
static Data computeMAC(Iter begin, Iter end, Data key) {
    auto data = Data();
    data.reserve((end - begin) + key.size());
    data.insert(data.end(), begin, end);
    append(data, key);
    return Hash::keccak256(data);
}

EncryptionParameters::EncryptionParameters(const std::string& password, Data data) : mac() {
    auto derivedKey = Data(kdfParams.defaultDesiredKeyLength);
    scrypt(reinterpret_cast<const byte*>(password.data()), password.size(), kdfParams.salt.data(), kdfParams.salt.size(), kdfParams.n, kdfParams.r, kdfParams.p, derivedKey.data(), kdfParams.defaultDesiredKeyLength);

    aes_encrypt_ctx ctx;
    auto result = aes_encrypt_key(derivedKey.data(), 16, &ctx);
    assert(result != EXIT_FAILURE);

    encrypted = Data(data.size());
    aes_ctr_encrypt(data.data(), encrypted.data(), data.size(), cipherParams.iv.data(), aes_ctr_cbuf_inc, &ctx);
    
    mac = computeMAC(derivedKey.end() - 16, derivedKey.end(), encrypted);
}

EncryptionParameters::~EncryptionParameters() {
    std::fill(encrypted.begin(), encrypted.end(), 0);
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
}

EncryptionParameters::EncryptionParameters(const nlohmann::json& json) {
    encrypted = parse_hex(json[CodingKeys::encrypted].get<std::string>());
    cipher = json[CodingKeys::cipher].get<std::string>();
    cipherParams = AESParameters(json[CodingKeys::cipherParams]);
    kdf = json[CodingKeys::kdf].get<std::string>();
    kdfParams = ScryptParameters(json[CodingKeys::kdfParams]);
    mac = parse_hex(json[CodingKeys::mac].get<std::string>());
}

nlohmann::json EncryptionParameters::json() const {
    nlohmann::json j;
    j[CodingKeys::encrypted] = hex(encrypted);
    j[CodingKeys::cipher] = cipher;
    j[CodingKeys::cipherParams] = cipherParams.json();
    j[CodingKeys::kdf] = kdf;
    j[CodingKeys::kdfParams] = kdfParams.json();
    j[CodingKeys::mac] = hex(mac);
    return j;
}
