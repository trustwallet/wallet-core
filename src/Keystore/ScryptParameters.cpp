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

namespace {

/// Reads a scrypt parameter that must be a JSON unsigned integer fitting in `uint32_t`.
/// Floats (`16384.0`), negatives and non-numeric values are rejected outright, and nothing is ever
/// narrowed from a floating type, so no out-of-range conversion can occur.
uint32_t parseU32(const nlohmann::json& j, const char* name) {
    if (!j.is_number_unsigned()) {
        throw std::invalid_argument(std::string("Invalid scrypt parameters: ") + name + " must be an unsigned integer");
    }
    const auto value = j.get<std::uint64_t>();
    if (value > std::numeric_limits<uint32_t>::max()) {
        throw std::invalid_argument(std::string("Invalid scrypt parameters: ") + name + " is out of range");
    }
    return static_cast<uint32_t>(value);
}

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

    // `dklen` is checked for format only, since the derived length is fixed (see `validate()`).
    // It must be the unsigned integer 32. Floats (`32.0`, `32.7`), negatives and non-numeric values
    // are all rejected, and nothing is ever cast from a float, which would be UB when out of range.
    const auto& dkLen = json[CodingKeys::SP::desiredKeyLength];
    if (!dkLen.is_number_unsigned() || dkLen.get<std::uint64_t>() != static_cast<std::uint64_t>(defaultDesiredKeyLength)) {
        throw std::invalid_argument("Invalid scrypt parameters: dklen must be 32");
    }
    desiredKeyLength = defaultDesiredKeyLength;
    n = parseU32(json[CodingKeys::SP::n], "n");
    p = parseU32(json[CodingKeys::SP::p], "p");
    r = parseU32(json[CodingKeys::SP::r], "r");

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
