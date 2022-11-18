// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Hash.h"
#include "HexCoding.h"
#include "uint256.h"
#include "ImmutableX/Constants.h"
#include "Ethereum/Signer.h"
#include "HDWallet.h"
#include <sstream>
#include <string>
#include <iostream>

namespace TW::ImmutableX {

static std::string getIntFromBits(const std::string& hexString, std::size_t from, std::optional<std::size_t> length = std::nullopt) {
    const auto data = hex_str_to_bin_str(hexString);
    const auto sub = data.substr(data.size() - from, length.value_or(std::string::npos));
    return std::to_string(std::stoll(sub, nullptr, 2));
}

static uint256_t hashKeyWithIndex(const std::string& seed, std::size_t index) {
    auto data = parse_hex(seed);
    data.push_back(static_cast<uint8_t>(index));
    auto out = Hash::sha256(data);
    return load(out);
}

static std::string grindKey(const std::string& seed) {
    std::size_t index{0};
    int256_t key = hashKeyWithIndex(seed, index);
    while (key >= internal::gStarkDeriveBias) {
            std::stringstream ss;
            ss << std::hex << key;
            key = hashKeyWithIndex(ss.str(), index);
            index += 1;
    }
    auto final = key % internal::gStarkCurveN;
    std::stringstream ss;
    ss << std::hex << final;
    return ss.str();
}
static std::string getPrivateKeyFromSeed(const std::string& seed, const std::string& path) {
    auto dataSeed = parse_hex(seed);
    auto key = HDWallet::bip32DeriveRawSeed(TWCoinTypeEthereum, dataSeed, DerivationPath(path));
    return grindKey(hex(key.bytes));
}

// https://docs.starkware.co/starkex/key-derivation.html
[[nodiscard("Use it to get derivation path")]] static std::string accountPathFromAddress(const std::string& ethAddress) noexcept {
    using namespace internal;
    std::stringstream out;
    const auto layerHash = getIntFromBits(hex(Hash::sha256(data(gLayer))), 31);
    const auto applicationHash = getIntFromBits(hex(Hash::sha256(data(gApplication))), 31);
    const auto ethAddress1 = getIntFromBits(ethAddress.substr(2), 31);
    const auto ethAddress2 = getIntFromBits(ethAddress.substr(2), 62, 31);
    out << "m/2645'/" << layerHash << "'/" << applicationHash << "'/" << ethAddress1 << "'/" << ethAddress2 << "'/" << gIndex;
    return out.str();
}

static std::string getPrivateKeyFromRawSignature(const std::string& signature, const std::string& ethAddress) {
    auto data = parse_hex(signature);
    auto ethSignature = Ethereum::Signer::signatureDataToStructSimple(data);
    auto seed = store(ethSignature.s);
    return getPrivateKeyFromSeed(hex(seed), accountPathFromAddress(ethAddress));
}

} // namespace TW::ImmutableX
