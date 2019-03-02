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
}

StoredKey::StoredKey(const nlohmann::json& json) {
    auto version = json[CodingKeys::version].get<int>();
    if (version != 3) {
        throw DecryptionError::invalidKeyFile;
    }

    if (json[CodingKeys::type] && json[CodingKeys::type].get<std::string>() == TypeString::mnemonic) {
        type = StoredKeyType::mnemonicPhrase;
    }  else {
        type = StoredKeyType::privateKey;
    }

    if (json[CodingKeys::id]) {
        id = json[CodingKeys::id].get<std::string>();
    }

    if (json[CodingKeys::crypto]) {
        payload = EncryptionParameters(json[CodingKeys::crypto]);
    } else if (json[UppercaseCodingKeys::crypto]) {
        // Workaround for myEtherWallet files
        payload = EncryptionParameters(json[UppercaseCodingKeys::crypto]);
    } else {
        throw DecryptionError::invalidKeyFile;
    }

    if (json[CodingKeys::activeAccounts].is_array()) {
        for (auto& accountJSON : json[CodingKeys::activeAccounts]) {
            accounts.emplace_back(accountJSON);
        }
    }

    if (accounts.empty() && json[CodingKeys::address].is_string()) {
        TWCoinType coin = TWCoinTypeEthereum;
        if (json[CodingKeys::coin]) {
            coin = json[CodingKeys::coin].get<TWCoinType>();
        }
        auto address = json[CodingKeys::address].get<std::string>();
        accounts.emplace_back(address, DerivationPath(TWPurposeBIP44, coin));
    }
}

nlohmann::json StoredKey::json() const {
    var container = encoder.container(keyedBy: CodingKeys.self)
    switch type {
    case .encryptedKey:
        try container.encode(TypeString.privateKey, forKey: .type)
    case .hierarchicalDeterministicWallet:
        try container.encode(TypeString.mnemonic, forKey: .type)
    }
    try container.encode(id, forKey: .id)
    try container.encodeIfPresent(address?.description, forKey: .address)
    try container.encode(crypto, forKey: .crypto)
    try container.encode(version, forKey: .version)
    try container.encode(activeAccounts, forKey: .activeAccounts)
    try container.encodeIfPresent(coin, forKey: .coin)
}

public static func address(for coin: CoinType?, addressString: String) -> Address? {
    guard let coin = coin else {
        guard let data = Data(hexString: addressString) else {
            return nil
        }
        return EthereumAddress(keyHash: data)
    }
    return blockchain(coin: coin).address(string: addressString)
}
