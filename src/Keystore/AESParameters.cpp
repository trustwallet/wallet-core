// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AESParameters.h"

#include "../HexCoding.h"

#include <TrezorCrypto/rand.h>

using namespace TW;

namespace {

constexpr std::size_t gBlockSize{16};

Data generateIv(std::size_t blockSize = gBlockSize) {
    // https://www.reddit.com/r/crypto/comments/30x5xg/what_length_should_the_iv_be_for_aes256ctr/
    // First off, AES 128 uses a 128 bit key.
    // So if you're using AES 256, you're using a 256 bit key.
    // Let's not confuse the block length with key length here.
    // For AES, your block length is always going to be 128 bits/16 bytes regardless of the key length used
    auto iv = Data(blockSize, 0);
    random_buffer(iv.data(), blockSize);
    return iv;
}

static TWStoredKeyEncryption getCipher(const std::string& cipher) {
    if (cipher == "aes-128-ctr") {
        return TWStoredKeyEncryption::TWStoredKeyEncryptionAes128Ctr;
    } else if (cipher == "aes-192-ctr") {
        return TWStoredKeyEncryption::TWStoredKeyEncryptionAes192Ctr;
    } else if (cipher == "aes-256-ctr") {
        return TWStoredKeyEncryption::TWStoredKeyEncryptionAes256Ctr;
    }
    return TWStoredKeyEncryptionAes128Ctr;
}

const std::unordered_map<TWStoredKeyEncryption, Keystore::AESParameters> gEncryptionRegistry{
    {TWStoredKeyEncryptionAes128Ctr, Keystore::AESParameters{.mKeyLength = Keystore::A128, .mCipher = "aes-128-ctr", .mCipherEncryption = TWStoredKeyEncryptionAes128Ctr}},
    {TWStoredKeyEncryptionAes128Cbc, Keystore::AESParameters{.mKeyLength = Keystore::A128, .mCipher = "aes-128-cbc", .mCipherEncryption = TWStoredKeyEncryptionAes128Cbc}},
    {TWStoredKeyEncryptionAes192Ctr, Keystore::AESParameters{.mKeyLength = Keystore::A192, .mCipher = "aes-192-ctr", .mCipherEncryption = TWStoredKeyEncryptionAes192Ctr}},
    {TWStoredKeyEncryptionAes256Ctr, Keystore::AESParameters{.mKeyLength = Keystore::A256, .mCipher = "aes-256-ctr", .mCipherEncryption = TWStoredKeyEncryptionAes256Ctr}}
};
} // namespace

namespace TW::Keystore {

namespace CodingKeys {
static const auto iv = "iv";
} // namespace CodingKeys

/// Initializes `AESParameters` with a JSON object.
AESParameters AESParameters::AESParametersFromJson(const nlohmann::json& json, const std::string& cipher) {
    auto parameters = AESParameters::AESParametersFromEncryption(getCipher(cipher));
    parameters.iv = parse_hex(json[CodingKeys::iv].get<std::string>());
    return parameters;
}

/// Saves `this` as a JSON object.
nlohmann::json AESParameters::json() const {
    nlohmann::json j;
    j[CodingKeys::iv] = hex(iv);
    return j;
}

AESParameters AESParameters::AESParametersFromEncryption(TWStoredKeyEncryption encryption) {
    auto parameters = gEncryptionRegistry.at(encryption);
    // be sure to regenerate an iv.
    parameters.iv = generateIv();
    return parameters;
}

} // namespace TW::Keystore
