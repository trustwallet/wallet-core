// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "AESParameters.h"

#include "../HexCoding.h"

#include <sstream>
#include <TrezorCrypto/rand.h>

using namespace TW;

namespace {

Data generateIv(std::size_t blockSize = TW::Keystore::gBlockSize) {
    auto iv = Data(blockSize, 0);
    random_buffer(iv.data(), blockSize);
    return iv;
}

static TWStoredKeyEncryption getCipher(const std::string& cipher) {
    if (cipher == Keystore::gAes128Ctr) {
        return TWStoredKeyEncryption::TWStoredKeyEncryptionAes128Ctr;
    }
    if (cipher == Keystore::gAes192Ctr) {
        return TWStoredKeyEncryption::TWStoredKeyEncryptionAes192Ctr;
    }
    if (cipher == Keystore::gAes256Ctr) {
        return TWStoredKeyEncryption::TWStoredKeyEncryptionAes256Ctr;
    }

    std::stringstream ss;
    ss << "Unsupported cipher: " << cipher;
    throw std::invalid_argument(ss.str());
}

const std::unordered_map<TWStoredKeyEncryption, Keystore::AESParameters> gEncryptionRegistry{
    {TWStoredKeyEncryptionAes128Ctr, Keystore::AESParameters{.mKeyLength = Keystore::A128, .mCipher = Keystore::gAes128Ctr, .mCipherEncryption = TWStoredKeyEncryptionAes128Ctr, .iv{}}},
    {TWStoredKeyEncryptionAes192Ctr, Keystore::AESParameters{.mKeyLength = Keystore::A192, .mCipher = Keystore::gAes192Ctr, .mCipherEncryption = TWStoredKeyEncryptionAes192Ctr, .iv{}}},
    {TWStoredKeyEncryptionAes256Ctr, Keystore::AESParameters{.mKeyLength = Keystore::A256, .mCipher = Keystore::gAes256Ctr, .mCipherEncryption = TWStoredKeyEncryptionAes256Ctr, .iv{}}}
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
