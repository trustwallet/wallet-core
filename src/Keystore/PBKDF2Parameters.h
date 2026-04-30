// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../HexCoding.h"

#include <nlohmann/json.hpp>
#include <optional>

namespace TW::Keystore {

/// PBKDF2 function parameters.
struct PBKDF2Parameters {
    /// Default number of iterations for the PBKDF2 encryption algorithm.
    static const uint32_t defaultIterations = 262144;

    /// Default desired key length of PBKDF2 encryption algorithm.
    static const std::size_t defaultDesiredKeyLength = 32;

    /// Minimum and maximum salt length for PBKDF2 encryption algorithm.
    /// https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-132.pdf
    static const std::size_t minSaltLength = 16;

    /// Random salt.
    Data salt;

    /// Desired key length in bytes.
    std::size_t desiredKeyLength = defaultDesiredKeyLength;

    /// CPU/Memory cost factor.
    uint32_t iterations = defaultIterations;

    /// Initializes with default scrypt parameters and a random salt.
    PBKDF2Parameters();

    /// Initializes `PBKDF2Parameters` with all values.
    PBKDF2Parameters(const Data& salt, uint32_t iterations, std::size_t desiredKeyLength)
        : salt(std::move(salt)), desiredKeyLength(desiredKeyLength), iterations(iterations) {}

    /// Initializes `PBKDF2Parameters` with a JSON object.
    PBKDF2Parameters(const nlohmann::json& json);

    /// Checks if the parameters should be fixed, i.e. if they are "valid" but do not meet the recommended security requirements,
    /// for example if the salt is empty or too short.
    bool shouldFix() const {
        return salt.size() < minSaltLength;
    }

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

} // namespace TW::Keystore
