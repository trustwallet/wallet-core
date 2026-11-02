// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "ScryptParameters.h"
#include "JsonParsing.h"

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

namespace {

const auto* const kErrorPrefix = "Invalid scrypt parameters: ";

} // namespace

std::string toString(const ScryptValidationError error) {
    switch (error) {
    case ScryptValidationError::invalidDesiredKeyLength:
            return "Desired key length must be 32";
    case ScryptValidationError::invalidSaltLength:
        return "Salt length is invalid";
    case ScryptValidationError::zeroBlockSizeOrParallelization:
            return "Block size r and parallelization p must be greater than 0";
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

ScryptParameters ScryptParameters::getPreset(TWStoredKeyEncryptionLevel preset) {
    switch (preset) {
    case TWStoredKeyEncryptionLevelMinimal:
        return minimal();
    case TWStoredKeyEncryptionLevelStandard:
        return standard();
    case TWStoredKeyEncryptionLevelWeak:
    case TWStoredKeyEncryptionLevelDefault:
    default:
        return weak();
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

std::optional<ScryptValidationError> ScryptParameters::validate() const {
    // wallet-core derives exactly `defaultDesiredKeyLength` bytes on both the encrypt and decrypt
    // paths. Any other value would size the derived-key buffer incorrectly for the AES key schedule
    // and the MAC, which read fixed offsets of up to 32 bytes.
    if (desiredKeyLength != defaultDesiredKeyLength) {
        return ScryptValidationError::invalidDesiredKeyLength;
    }
    // For backward compatibility with existing keys, we allow empty and less than 16 bytes salt.
    if (salt.size() > maxSaltLength) {
        return ScryptValidationError::invalidSaltLength;
    }
    // Must precede the overflow check below, which divides by `p` and by `r`. With either at zero
    // that division is undefined behaviour (SIGFPE on x86-64). scrypt itself rejects r == 0 || p == 0.
    if (r == 0 || p == 0) {
        return ScryptValidationError::zeroBlockSizeOrParallelization;
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

ScryptParameters ScryptParameters::regenerateWithRecommendedParams() const {
    ScryptParameters fixedParams = *this;
    fixedParams.salt = internal::randomSalt();
    return fixedParams;
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
    if (json.count(CodingKeys::SP::n) == 0
        || json.count(CodingKeys::SP::p) == 0
        || json.count(CodingKeys::SP::r) == 0
        || json.count(CodingKeys::SP::desiredKeyLength) == 0) {
        throw std::invalid_argument("Missing required scrypt parameters n, p, r, or dklen");
    }

    // For backward compatibility with existing keys, we allow missing salt, and fallback to empty salt in that case.
    if (json.count(CodingKeys::SP::salt) == 0) {
        salt = Data();
    } else {
        const auto res = parse_hex_checked(json[CodingKeys::SP::salt].get<std::string>());
        if (res.isFailure()) {
            throw std::invalid_argument("Invalid scrypt parameters: salt must be a valid hex");
        }
        salt = res.payload();
    }

    // Each field must be a JSON integer: floats (`32.0`, `16384.0`), negatives and non-numeric values
    // are rejected, and nothing is ever cast from a float, which would be UB when out of range.
    // `dklen` is only read here; `validate()` below is the single place that requires it to be 32.
    desiredKeyLength = static_cast<std::size_t>(internal::parseUnsigned(
        json[CodingKeys::SP::desiredKeyLength], kErrorPrefix, CodingKeys::SP::desiredKeyLength,
        std::numeric_limits<std::size_t>::max()));
    n = internal::parseU32(json[CodingKeys::SP::n], kErrorPrefix, CodingKeys::SP::n);
    p = internal::parseU32(json[CodingKeys::SP::p], kErrorPrefix, CodingKeys::SP::p);
    r = internal::parseU32(json[CodingKeys::SP::r], kErrorPrefix, CodingKeys::SP::r);

    if (const auto error = validate()) {
        throw std::invalid_argument(kErrorPrefix + toString(*error));
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
