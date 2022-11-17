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

static std::string getIntFromBits(std::string hexString, std::size_t from, std::optional<std::size_t> length = std::nullopt) {
    auto data = hex_str_to_bin_str(hexString);
    auto sub = data.substr(data.size() - from, length.value_or(std::string::npos));
    return std::to_string(std::stoll(sub, nullptr, 2));
}

// https://docs.starkware.co/starkex/key-derivation.html
[[nodiscard("Use it to get derivation path")]] static std::string accountPathFromAddress(const std::string& ethAddress) noexcept {
    std::stringstream out;
    const auto layerHash = Hash::sha256(data(internal::gLayer));
    const auto applicationHash = Hash::sha256(data(internal::gApplication));
    const auto ethAddress1 = getIntFromBits(ethAddress.substr(2), 31);
    const auto ethAddress2 = getIntFromBits(ethAddress.substr(2), 62, 31);
    out << "m/2645'/" << getIntFromBits(hex(layerHash), 31) << "'/" << getIntFromBits(hex(applicationHash), 31) << "'/" << ethAddress1 << "'/" << ethAddress2 << "'/" << internal::gIndex;
    return out.str();
}

} // namespace TW::ImmutableX
