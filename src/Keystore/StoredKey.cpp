// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StoredKey.h"

#include "Coin.h"

#define BOOST_UUID_RANDOM_PROVIDER_FORCE_POSIX 1

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace TW;
using namespace TW::Keystore;

StoredKey::StoredKey(StoredKeyType type, EncryptionParameters payload)
    : type(type), payload(std::move(payload)), id(), accounts() {
    boost::uuids::random_generator gen;
    id = boost::lexical_cast<std::string>(gen());
}

StoredKey::StoredKey(StoredKeyType type, const std::string& password, Data data)
    : type(type), payload(password, data), id(), accounts() {
    boost::uuids::random_generator gen;
    id = boost::lexical_cast<std::string>(gen());
}

StoredKey StoredKey::load(const std::string& path) {
    std::ifstream stream(path);
    if (!stream.is_open()) {
        throw std::invalid_argument("Can't open file");
    }
    nlohmann::json j;
    stream >> j;

    StoredKey key(j);
    return key;
}

void StoredKey::store(const std::string& path) {
    auto stream = std::ofstream(path);
    stream << json();
}

HDWallet StoredKey::wallet(const std::string& password) {
    if (type != StoredKeyType::mnemonicPhrase) {
        throw std::invalid_argument("Invalid account requested.");
    }
    const auto data = payload.decrypt(password);
    const auto mnemonic = std::string(reinterpret_cast<const char*>(data.data()), data.size());
    return HDWallet(mnemonic, "");
}

const Account* StoredKey::account(TWCoinType coin, const HDWallet* wallet) {
    for (auto& account : accounts) {
        if (account.coin() == coin) {
            if (account.address.empty() && wallet != nullptr) {
                account.address = wallet->deriveAddress(coin);
            }
            return &account;
        }
    }

    if (wallet == nullptr) {
        return nullptr;
    }

    const auto derivationPath = TW::derivationPath(coin);
    const auto address = wallet->deriveAddress(coin);

    std::string extendedPublicKey;
    const auto version = TW::xpubVersion(coin);
    if (version != TWHDVersionNone) {
        extendedPublicKey = wallet->getExtendedPublicKey(derivationPath.purpose(), coin, version);
    }

    accounts.emplace_back(address, derivationPath, extendedPublicKey);
    return &accounts.back();
}

const Account* StoredKey::account(TWCoinType coin) const {
    for (auto& account : accounts) {
        if (account.coin() == coin) {
            return &account;
        }
    }
    return nullptr;
}

const PrivateKey StoredKey::privateKey(TWCoinType coin, const std::string& password) {
    switch (type) {
    case StoredKeyType::mnemonicPhrase: {
        const auto wallet = this->wallet(password);
        const auto account = *this->account(coin, &wallet);
        return wallet.getKey(account.derivationPath);
    }
    case StoredKeyType::privateKey:
        return PrivateKey(payload.decrypt(password));

    case StoredKeyType::watchOnly:
        throw std::invalid_argument("This is a watch-only key");
    }
}

void StoredKey::fixAddresses(const std::string& password) {
    switch (type) {
    case StoredKeyType::mnemonicPhrase: {
        const auto wallet = this->wallet(password);
        for (auto& account : accounts) {
            if (!account.address.empty() && TW::validateAddress(account.coin(), account.address)) {
                continue;
            }
            const auto& derivationPath = account.derivationPath;
            const auto key = wallet.getKey(derivationPath);
            account.address = TW::deriveAddress(derivationPath.coin(), key);
        }
    }
        break;
    case StoredKeyType::privateKey: {
        auto key = PrivateKey(payload.decrypt(password));
        for (auto& account : accounts) {
            if (!account.address.empty() && TW::validateAddress(account.coin(), account.address)) {
                continue;
            }
            account.address = TW::deriveAddress(account.coin(), key);
        }
    }
        break;
    case StoredKeyType::watchOnly:
        break;
    }
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
} // namespace CodingKeys

namespace UppercaseCodingKeys {
static const auto crypto = "Crypto";
} // namespace UppercaseCodingKeys

namespace TypeString {
static const auto privateKey = "private-key";
static const auto mnemonic = "mnemonic";
static const auto watch = "watch";
} // namespace TypeString

StoredKey::StoredKey(const nlohmann::json& json) {
    if (json.count(CodingKeys::type) != 0 &&
        json[CodingKeys::type].get<std::string>() == TypeString::mnemonic) {
        type = StoredKeyType::mnemonicPhrase;
    } else if (json.count(CodingKeys::type) != 0 &&
               json[CodingKeys::type].get<std::string>() == TypeString::watch) {
        type = StoredKeyType::watchOnly;
    } else {
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

    if (json.count(CodingKeys::activeAccounts) != 0 &&
        json[CodingKeys::activeAccounts].is_array()) {
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
