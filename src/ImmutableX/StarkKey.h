// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Hash.h"
#include "HexCoding.h"
#include <sstream>
#include <string>
#include <iostream>

namespace TW::ImmutableX {

namespace internal {
inline constexpr const char* gLayer = "starkex";
inline constexpr const char* gApplication = "immutablex";
inline constexpr const char* gIndex = "1";
} // namespace internal

static std::int64_t getIntFromBits(std::string hexString, std::size_t from, std::optional<std::size_t> length = std::nullopt) {
    auto data = parse_hex(hexString);
    auto sub = Data();
    if (length.has_value()) {
        sub = subData(data, from, length.value());
    } else {
        sub = subData(data, from);
    }
    return std::stoll(hex(sub), nullptr, 16);
}

// https://docs.starkware.co/starkex/key-derivation.html
[[nodiscard("Use it to get derivation path")]] static std::string accountPathFromAddress([[maybe_unused]] const std::string& ethAddress) noexcept {
    std::stringstream out;
    out << "m/2645'/";
    const auto layerHash = Hash::sha256(data(internal::gLayer));
    out << std::to_string(getIntFromBits(hex(layerHash), 28)) << "'/";
    const auto applicationHash = Hash::sha256(data(internal::gApplication));
    out << std::to_string(getIntFromBits(hex(applicationHash), 28)) << "'/";
    const auto ethAddress1 = std::to_string(getIntFromBits(ethAddress.substr(2), 16));
    const auto ethAddress2 = std::to_string(getIntFromBits(ethAddress.substr(2), 12, 4));
    out << ethAddress1 << "'/" << ethAddress2 << "'/" << internal::gIndex;
    return out.str();
}

} // namespace TW::ImmutableX
