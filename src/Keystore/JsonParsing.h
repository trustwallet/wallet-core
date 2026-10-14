// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

namespace TW::Keystore::internal {

/// Reads a keystore parameter that must be a JSON integer in `[0, max]`.
///
/// nlohmann stores a non-negative integer parsed from text as `number_unsigned`, but a value assigned
/// in memory from a signed literal (`j["n"] = 16384`) as `number_integer`. Both storage types are
/// accepted; a negative value is rejected. Floats (`16384.0`), strings, booleans and null are rejected
/// outright, and nothing is ever narrowed from a floating type, so no out-of-range conversion occurs.
///
/// @throws std::invalid_argument prefixed with `prefix`, naming `name`.
inline std::uint64_t parseUnsigned(const nlohmann::json& j, const char* prefix, const char* name, std::uint64_t max) {
    std::uint64_t value = 0;
    if (j.is_number_unsigned()) {
        value = j.get<std::uint64_t>();
    } else if (j.is_number_integer()) {
        const auto signedValue = j.get<std::int64_t>();
        if (signedValue < 0) {
            throw std::invalid_argument(std::string(prefix) + name + " must be a non-negative integer");
        }
        value = static_cast<std::uint64_t>(signedValue);
    } else {
        throw std::invalid_argument(std::string(prefix) + name + " must be a non-negative integer");
    }
    if (value > max) {
        throw std::invalid_argument(std::string(prefix) + name + " is out of range");
    }
    return value;
}

/// Reads a keystore parameter that must be a JSON integer fitting in `uint32_t`. See `parseUnsigned`.
inline std::uint32_t parseU32(const nlohmann::json& j, const char* prefix, const char* name) {
    return static_cast<std::uint32_t>(parseUnsigned(j, prefix, name, std::numeric_limits<std::uint32_t>::max()));
}

} // namespace TW::Keystore::internal
