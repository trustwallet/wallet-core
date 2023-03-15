// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

    /// Saves `this` as a JSON object.
    nlohmann::json json() const;
};

} // namespace TW::Keystore
