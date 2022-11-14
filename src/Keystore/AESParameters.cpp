// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AESParameters.h"

#include "../HexCoding.h"

#include <TrezorCrypto/rand.h>

using namespace TW;

namespace TW::Keystore {

AESParameters::AESParameters(AESSize blockSize) : mBlockSize(blockSize) {
    iv = Data(mBlockSize, 0);
    random_buffer(iv.data(), mBlockSize);
}

namespace CodingKeys {
static const auto iv = "iv";
} // namespace CodingKeys

/// Initializes `AESParameters` with a JSON object.
AESParameters::AESParameters(const nlohmann::json& json, const std::string& cipher) {
    iv = parse_hex(json[CodingKeys::iv].get<std::string>());
    if (cipher == "aes-128-ctr" || cipher == "aes-128-cbc") {
        mBlockSize = A128;
    } else if (cipher == "aes-192-ctr") {
        mBlockSize = A192;
    } else if (cipher == "aes-256-ctr") {
        mBlockSize = A256;
    }
}

/// Saves `this` as a JSON object.
nlohmann::json AESParameters::json() const {
    nlohmann::json j;
    j[CodingKeys::iv] = hex(iv);
    return j;
}

} // namespace TW::Keystore
