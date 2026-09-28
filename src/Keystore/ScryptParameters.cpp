// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "ScryptParameters.h"

#include <TrezorCrypto/rand.h>
#include <bit>
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
    case ScryptValidationError::invalidSaltLength:
        return "Salt length is invalid";
    case ScryptValidationError::blockSizeTooLarge:
            return "Block size (r * p) is too large";
    case ScryptValidationError::invalidCostFactor:
            return "Cost factor n must be a power of 2 greater than 1";
    case ScryptValidationError::overflow:
            return "Parameters are too large and may cause overflow";
    case ScryptValidationError::invalidCostFactorForR:
            return "Cost factor n is too large for block size r (log2(n) must be less than 19 * r)";
    case ScryptValidationError::scryptMemoryTooLarge:
            return "Parameters would require too much scrypt memory (V + B + XY exceeds the limit)";
    case ScryptValidationError::scryptWorkTooLarge:
            return "Parameters would require too much CPU time (n * r * p exceeds the limit)";
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

#pragma GCC diagnostic ignored "-Wtautological-constant-out-of-range-compare"

std::optional<ScryptValidationError> ScryptParameters::validate() const {
    if (desiredKeyLength > ((1ULL << 32) - 1) * 32) { // depending on size_t size on platform, may be always false
        return ScryptValidationError::desiredKeyLengthTooLarge;
    }
    // For backward compatibility with existing keys, we allow empty and less than 16 bytes salt.
    if (salt.size() > maxSaltLength) {
        return ScryptValidationError::invalidSaltLength;
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

    // RFC 7914 requires N < 2^(128 * r / 8), i.e. log2(n) < 16r, for ROMix to index V uniformly.
    // Legacy geth-style wallets (N=262144, r=1) violate the strict bound, so it is relaxed to 19r,
    // matching tw_crypto's Rust scrypt (#4463). `n` is a power of two here, so countr_zero == log2.
    // Placed after the overflow check, which bounds r, so `r * 19` cannot overflow.
    const auto logN = static_cast<uint64_t>(std::countr_zero(n));
    if (logN >= static_cast<uint64_t>(r) * 19) {
        return ScryptValidationError::invalidCostFactorForR;
    }

    // scrypt allocates V = 128*r*N, B = 128*r*p and XY = 256*r + 64 (trezor-crypto scrypt.c).
    // Bound the total, not V alone: n=2, r=1, p=33554431 passes every check above with a 256-byte V
    // while B is 4 GiB. The overflow check already bounds 128*r*n and 128*r*p by UINT32_MAX each,
    // so this uint64_t sum cannot overflow. 512 MiB is twice the Standard preset (N=262144, r=8, p=1).
    const uint64_t r64 = r, n64 = n, p64 = p;
    const uint64_t scryptMemory = 128 * r64 * n64 + 128 * r64 * p64 + 256 * r64 + 64;
    if (scryptMemory > maxScryptMemory) {
        return ScryptValidationError::scryptMemoryTooLarge;
    }

    // The memory cap does not bound CPU: scrypt's work is proportional to n * r * p (smix runs p
    // times, each 2n block-mixes of r blocks), while p adds only 128*r bytes each. n=262144, r=1,
    // p=3900000 passes every check above at ~508 MiB yet costs ~500,000x the Standard preset.
    // r*p < 2^30 and n < 2^32 here, so the product fits in uint64_t.
    if (n64 * r64 * p64 > maxScryptWork) {
        return ScryptValidationError::scryptWorkTooLarge;
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

    desiredKeyLength = json[CodingKeys::SP::desiredKeyLength];
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
