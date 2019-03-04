// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Account.h"

#include "../HexCoding.h"

using namespace TW;
using namespace TW::Keystore;

namespace CodingKeys {
    static const auto address = "address";
    static const auto addressData = "addressData";
    static const auto derivationPath = "derivationPath";
    static const auto indices = "indices";
    static const auto value = "value";
    static const auto hardened = "hardened";
}

Account::Account(const nlohmann::json& json) {
    if (json[CodingKeys::derivationPath].is_object()) {
        const auto indices = json[CodingKeys::derivationPath][CodingKeys::indices];
        for (auto& indexJSON : indices) {
            derivationPath.indices.emplace_back(indexJSON[CodingKeys::value].get<uint32_t>(), indexJSON[CodingKeys::hardened].get<bool>());
        }
    } else if (json[CodingKeys::derivationPath].is_string()) {
        derivationPath = DerivationPath(json[CodingKeys::derivationPath].get<std::string>());
    }

    if (json.count(CodingKeys::address) != 0 && json[CodingKeys::address].is_string()) {
        address = json[CodingKeys::address].get<std::string>();
    } else if (json.count(CodingKeys::addressData) != 0 && json[CodingKeys::addressData].is_string()) {
        address = json[CodingKeys::addressData].get<std::string>();
        // TODO: get properly encoded address string
    }
}

nlohmann::json Account::json() const {
    nlohmann::json j;
    j[CodingKeys::address] = address;
    j[CodingKeys::derivationPath] = derivationPath.string();
    return j;
}
