// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "ScryptParameters.h"

#include <TrezorCrypto/rand.h>
#include <limits>
#include <sstream>

using namespace TW;

namespace TW::Keystore {

namespace internal {

Data randomSalt() {
    Data salt(32);
    random_buffer(salt.data(), salt.size());
    return salt;
}

} // namespace internal

std::string toString(const ScryptValidationError error) {
    switch (error) {
    case ScryptValidationError::desiredKeyLengthTooLarge:
            return "Desired key length is too large";
    case ScryptValidationError::blockSizeTooLarge:
            return "Block size (r * p) is too large";
    case ScryptValidationError::invalidCostFactor:
            return "Cost factor n must be a power of 2 greater than 1";
    case ScryptValidationError::overflow:
            return "Parameters are too large and may cause overflow";
    default:
            return "Unknown error";
    }
}

ScryptParameters ScryptParameters::minimal() {
    return { internal::randomSalt(), minimalN, defaultR, minimalP, defaultDesiredKeyLength };
}

ScryptParameters ScryptParameters::weak() {
    return { internal::randomSalt(), weakN, defaultR, weakP, defaultDesiredKeyLength };
}

ScryptParameters ScryptParameters::standard() {
    return { internal::randomSalt(), standardN, defaultR, standardP, defaultDesiredKeyLength };
}

ScryptParameters::ScryptParameters()
    : salt(internal::randomSalt()) {
}

#pragma GCC diagnostic ignored "-Wtautological-constant-out-of-range-compare"

std::optional<ScryptValidationError> ScryptParameters::validate() const {
    if (desiredKeyLength > ((1ULL << 32) - 1) * 32) { // depending on size_t size on platform, may be always false
        return ScryptValidationError::desiredKeyLengthTooLarge;
    }
    if (static_cast<uint64_t>(r) * static_cast<uint64_t>(p) >= (1 << 30)) {
        return ScryptValidationError::blockSizeTooLarge;
    }
    if ((n & (n - 1)) != 0 || n < 2) {
        return ScryptValidationError::invalidCostFactor;
    }
    if ((r > std::numeric_limits<uint32_t>::max() / 128 / p) ||
        (n > std::numeric_limits<uint32_t>::max() / 128 / r)) {
        return ScryptValidationError::overflow;
    }
    return {};
}

// -----------------
// Encoding/Decoding
// -----------------

namespace CodingKeys::SP {

static const auto salt = "salt";
static const auto desiredKeyLength = "dklen";
static const auto n = "n";
static const auto p = "p";
static const auto r = "r";

} // namespace CodingKeys::SP

ScryptParameters::ScryptParameters(const nlohmann::json& json) {
    salt = parse_hex(json[CodingKeys::SP::salt].get<std::string>());
    desiredKeyLength = json[CodingKeys::SP::desiredKeyLength];
    if (json.count(CodingKeys::SP::n) == 0
        || json.count(CodingKeys::SP::p) == 0
        || json.count(CodingKeys::SP::r) == 0) {
        throw std::invalid_argument("Missing required scrypt parameters n, p, or r");
    }
    n = json[CodingKeys::SP::n];
    p = json[CodingKeys::SP::p];
    r = json[CodingKeys::SP::r];

    if (const auto error = validate()) {
        std::stringstream ss;
        ss << "Invalid scrypt parameters: " << toString(*error);
        throw std::invalid_argument(ss.str());
    }
}

/// Saves `this` as a JSON object.
nlohmann::json ScryptParameters::json() const {
    nlohmann::json j;
    j[CodingKeys::SP::salt] = hex(salt);
    j[CodingKeys::SP::desiredKeyLength] = desiredKeyLength;
    j[CodingKeys::SP::n] = n;
    j[CodingKeys::SP::p] = p;
    j[CodingKeys::SP::r] = r;
    return j;
}

} // namespace TW::Keystore
