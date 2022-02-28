// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Account.h"

#include "../Base64.h"
#include "../Coin.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Keystore;

namespace CodingKeys {
    static const auto address = "address";
    static const auto derivationPath = "derivationPath";
    static const auto extendedPublicKey = "extendedPublicKey";
    static const auto indices = "indices";
    static const auto value = "value";
    static const auto hardened = "hardened";
    static const auto coin = "coin";
} // namespace CodingKeys

Account::Account(const nlohmann::json& json) {
    if (json[CodingKeys::derivationPath].is_object()) {
        const auto indices = json[CodingKeys::derivationPath][CodingKeys::indices];
        for (auto& indexJSON : indices) {
            derivationPath.indices.emplace_back(indexJSON[CodingKeys::value].get<uint32_t>(),
                                                indexJSON[CodingKeys::hardened].get<bool>());
        }
    } else if (json[CodingKeys::derivationPath].is_string()) {
        derivationPath = DerivationPath(json[CodingKeys::derivationPath].get<std::string>());
    }

    if (json.find(CodingKeys::coin) == json.end()) {
        // legacy format, get coin from derivation path
        coin = TWCoinType(uint32_t(derivationPath.indices[1].value));
    } else {
        coin = TWCoinType(json[CodingKeys::coin].get<uint32_t>());
    }

    if (json.count(CodingKeys::address) != 0 && json[CodingKeys::address].is_string()) {
        address = json[CodingKeys::address].get<std::string>();
    } else {
        address = "";
    }

    if (json.count(CodingKeys::extendedPublicKey) > 0 &&
        json[CodingKeys::extendedPublicKey].is_string()) {
        extendedPublicKey = json[CodingKeys::extendedPublicKey].get<std::string>();
    }
}

nlohmann::json Account::json() const {
    nlohmann::json j;
    j[CodingKeys::address] = address;
    j[CodingKeys::derivationPath] = derivationPath.string();
    j[CodingKeys::coin] = coin;
    if (!extendedPublicKey.empty()) {
        j[CodingKeys::extendedPublicKey] = extendedPublicKey;
    }
    return j;
}
