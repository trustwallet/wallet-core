// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <Ethereum/EIP2645.h>
#include <Hash.h>
#include <HexCoding.h>

#include <optional>
#include <sstream>

namespace TW::Ethereum::internal {

static std::string getIntFromBits(const std::string& hexString, std::size_t from, std::optional<std::size_t> length = std::nullopt) {
    const auto data = hex_str_to_bin_str(hexString);
    const auto sub = data.substr(data.size() - from, length.value_or(std::string::npos));
    return std::to_string(std::stoll(sub, nullptr, 2));
}

} // namespace TW::Ethereum::internal

namespace TW::Ethereum {

// https://docs.starkware.co/starkex/key-derivation.html
std::string accountPathFromAddress(const std::string& ethAddress, const std::string& layer, const std::string& application, const std::string& index) noexcept {
    using namespace internal;
    std::stringstream out;
    const auto layerHash = getIntFromBits(hex(Hash::sha256(data(layer))), 31);
    const auto applicationHash = getIntFromBits(hex(Hash::sha256(data(application))), 31);
    const auto ethAddress1 = getIntFromBits(ethAddress.substr(2), 31);
    const auto ethAddress2 = getIntFromBits(ethAddress.substr(2), 62, 31);
    out << "m/2645'/" << layerHash << "'/" << applicationHash << "'/" << ethAddress1 << "'/" << ethAddress2 << "'/" << index;
    return out.str();
}

} // namespace TW::Ethereum
