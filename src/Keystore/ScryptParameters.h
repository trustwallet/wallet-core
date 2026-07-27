// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "TrustWalletCore/TWStoredKeyEncryptionLevel.h"
#include "../HexCoding.h"

#include <nlohmann/json.hpp>
#include <optional>

namespace TW::Keystore {

enum class ScryptValidationError {
    desiredKeyLengthTooLarge,
    invalidSaltLength,
    blockSizeTooLarge,
    invalidCostFactor,
    overflow,
};

std::string toString(ScryptValidationError error);

/// Scrypt function parameters.
struct ScryptParameters {
    /// The N and P parameters of Scrypt encryption algorithm, using 256MB memory and
    /// taking approximately 1s CPU time on a modern processor.
    static const uint32_t standardN = 1 << 18;
    static const uint32_t standardP = 1;

    static const uint32_t weakN = 1 << 14;
    static const uint32_t weakP = 4;

    /// The N and P parameters of Scrypt encryption algorithm, using 4MB memory and
    /// taking approximately 100ms CPU time on a modern processor.
    static const uint32_t minimalN = 1 << 12;
    static const uint32_t minimalP = 6;

    /// Default `R` parameter of Scrypt encryption algorithm.
    static const uint32_t defaultR = 8;

    /// Default desired key length of Scrypt encryption algorithm.
    static const std::size_t defaultDesiredKeyLength = 32;

    /// Minimum and maximum salt length for Scrypt encryption algorithm.
    /// https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-132.pdf
    static const std::size_t minSaltLength = 16;
    static const std::size_t maxSaltLength = 1024;

    /// Random salt.
    Data salt;

    /// Desired key length in bytes.
    std::size_t desiredKeyLength = defaultDesiredKeyLength;

    /// CPU/Memory cost factor.
    uint32_t n = minimalN;

    /// Parallelization factor (1..232-1 * hLen/MFlen).
    uint32_t p = minimalP;

    /// Block size factor.
    uint32_t r = defaultR;

    /// Returns a preset of Scrypt encryption parameters for the given encryption level.
    static ScryptParameters getPreset(TWStoredKeyEncryptionLevel preset);

    /// Generates Scrypt encryption parameters with the minimal sufficient level (4096), and with a random salt.
    static ScryptParameters minimal();
    /// Generates Scrypt encryption parameters with the weak sufficient level (16k), and with a random salt.
    static ScryptParameters weak();
    /// Generates Scrypt encryption parameters with the standard sufficient level (262k), and with a random salt.
    static ScryptParameters standard();

    /// Initializes with default scrypt parameters and a random salt.
    ScryptParameters();

    /// Initializes `ScryptParameters` with all values.
    ///
    /// @throws ScryptValidationError if the parameters are invalid.
    ScryptParameters(Data salt, uint32_t n, uint32_t r, uint32_t p, std::size_t desiredKeyLength)
        : salt(std::move(salt)), desiredKeyLength(desiredKeyLength), n(n), p(p), r(r) {
        auto error = validate();
        if (error) {
            throw *error;
        }
    }

    /// Validates the parameters.
    ///
    /// - Returns: a `ValidationError` or `nil` if the parameters are valid.
    std::optional<ScryptValidationError> validate() const;

    /// Regenerates the parameters with recommended Scrypt parameters.
    /// Note: this method only regenerates the salt with a random value for now,
    /// but it can be extended in the future to also update the N, r, and p parameters.
    ScryptParameters regenerateWithRecommendedParams() const;

    /// Checks if the parameters should be fixed, i.e. if they are "valid" but do not meet the recommended security requirements,
    /// for example if the salt is empty or too short.
    bool shouldFix() const {
        return salt.size() < minSaltLength;
    }

    /// Initializes `ScryptParameters` with a JSON object.
    explicit ScryptParameters(const nlohmann::json& json);

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

} // namespace TW::Keystore
