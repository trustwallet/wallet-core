// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StoredKey.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using namespace TW;
using namespace TW::Keystore;

StoredKey StoredKey::load(const std::string& path, const std::string& password) {
    std::ifstream stream(path);
    nlohmann::json j;
    stream >> j;

    StoredKey key(j);
    return key;
}

void StoredKey::store(const std::string& path, const std::string& password) {
    auto stream = std::ofstream(path);
    stream << json();
}

// -----------------
// Encoding/Decoding
// -----------------

namespace CodingKeys {
    static const auto address = "address";
    static const auto type = "type";
    static const auto id = "id";
    static const auto crypto = "crypto";
    static const auto activeAccounts = "activeAccounts";
    static const auto version = "version";
    static const auto coin = "coin";
}

namespace UppercaseCodingKeys {
    static const auto crypto = "Crypto";
}

namespace TypeString {
    static const auto privateKey = "private-key";
    static const auto mnemonic = "mnemonic";
    static const auto watch = "watch";
}

StoredKey::StoredKey(const nlohmann::json& json) {
    if (json.count(CodingKeys::type) != 0 && json[CodingKeys::type].get<std::string>() == TypeString::mnemonic) {
        type = StoredKeyType::mnemonicPhrase;
    } else if (json.count(CodingKeys::type) != 0 && json[CodingKeys::type].get<std::string>() == TypeString::watch) {
        type = StoredKeyType::watchOnly;
    }  else {
        type = StoredKeyType::privateKey;
    }

    if (json.count(CodingKeys::id) != 0) {
        id = json[CodingKeys::id].get<std::string>();
    }

    if (json.count(CodingKeys::crypto) != 0) {
        payload = EncryptionParameters(json[CodingKeys::crypto]);
    } else if (json.count(UppercaseCodingKeys::crypto) != 0) {
        // Workaround for myEtherWallet files
        payload = EncryptionParameters(json[UppercaseCodingKeys::crypto]);
    } else if (type == StoredKeyType::watchOnly) {
        payload = EncryptionParameters();
    } else {
        throw DecryptionError::invalidKeyFile;
    }

    if (json.count(CodingKeys::activeAccounts) != 0 && json[CodingKeys::activeAccounts].is_array()) {
        for (auto& accountJSON : json[CodingKeys::activeAccounts]) {
            accounts.emplace_back(accountJSON);
        }
    }

    if (accounts.empty() && json[CodingKeys::address].is_string()) {
        TWCoinType coin = TWCoinTypeEthereum;
        if (json.count(CodingKeys::coin) != 0) {
            coin = json[CodingKeys::coin].get<TWCoinType>();
        }
        auto address = json[CodingKeys::address].get<std::string>();
        accounts.emplace_back(address, DerivationPath(TWPurposeBIP44, coin, 0, 0, 0));
    }
}

nlohmann::json StoredKey::json() const {
    nlohmann::json j;
    j[CodingKeys::version] = 3;

    switch (type) {
    case StoredKeyType::privateKey:
        j[CodingKeys::type] = TypeString::privateKey;
        break;
    case StoredKeyType::mnemonicPhrase:
        j[CodingKeys::type] = TypeString::mnemonic;
        break;
    case StoredKeyType::watchOnly:
        j[CodingKeys::type] = TypeString::watch;
        break;
    }

    if (id) {
        j[CodingKeys::id] = *id;
    }

    j[CodingKeys::crypto] = payload.json();

    nlohmann::json accountsJSON = nlohmann::json::array();
    for (const auto& account : accounts) {
        accountsJSON.push_back(account.json());
    }
    j[CodingKeys::activeAccounts] = accountsJSON;

    return j;
}
