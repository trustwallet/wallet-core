// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "PBKDF2Parameters.h"

#include <TrezorCrypto/rand.h>

using namespace TW;

namespace TW::Keystore {

PBKDF2Parameters::PBKDF2Parameters()
    : salt(32) {
    random_buffer(salt.data(), salt.size());
}

// -----------------
// Encoding/Decoding
// -----------------

namespace CodingKeys {
static const auto salt = "salt";
static const auto desiredKeyLength = "dklen";
static const auto iterations = "c";
} // namespace CodingKeys

PBKDF2Parameters::PBKDF2Parameters(const nlohmann::json& json) {
    salt = parse_hex(json[CodingKeys::salt].get<std::string>());
    desiredKeyLength = json[CodingKeys::desiredKeyLength];
    if (json.count(CodingKeys::iterations) != 0)
        iterations = json[CodingKeys::iterations];
}

/// Saves `this` as a JSON object.
nlohmann::json PBKDF2Parameters::json() const {
    nlohmann::json j;
    j[CodingKeys::salt] = hex(salt);
    j[CodingKeys::desiredKeyLength] = desiredKeyLength;
    j[CodingKeys::iterations] = iterations;
    return j;
}

} // namespace TW::Keystore
