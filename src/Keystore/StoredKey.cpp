// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StoredKey.h"

#include "Coin.h"
#include "HDWallet.h"
#include "PrivateKey.h"

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
#include <cassert>

using namespace TW;
using namespace TW::Keystore;

StoredKey StoredKey::createWithMnemonic(const std::string& name, const Data& password, const std::string& mnemonic) {
    if (!HDWallet::isValid(mnemonic)) {
        throw std::invalid_argument("Invalid mnemonic");
    }
    
    Data mnemonicData = TW::Data(mnemonic.begin(), mnemonic.end());
    StoredKey key = StoredKey(StoredKeyType::mnemonicPhrase, name, password, mnemonicData);
    return key;
}

StoredKey StoredKey::createWithMnemonicRandom(const std::string& name, const Data& password) {
    const auto wallet = TW::HDWallet(128, "");
    const auto& mnemonic = wallet.mnemonic;
    assert(HDWallet::isValid(mnemonic));
    Data mnemonicData = TW::Data(mnemonic.begin(), mnemonic.end());
    StoredKey key = StoredKey(StoredKeyType::mnemonicPhrase, name, password, mnemonicData);
    return key;
}

StoredKey StoredKey::createWithMnemonicAddDefaultAddress(const std::string& name, const Data& password, const std::string& mnemonic, TWCoinType coin) {
    StoredKey key = createWithMnemonic(name, password, mnemonic);

    const auto wallet = HDWallet(mnemonic, "");
    const auto derivationPath = TW::derivationPath(coin);
    const auto address = TW::deriveAddress(coin, wallet.getKey(coin, derivationPath));
    const auto extendedKey = wallet.getExtendedPublicKey(TW::purpose(coin), coin, TW::xpubVersion(coin));
    key.accounts.emplace_back(address, coin, derivationPath, extendedKey);

    return key;
}

StoredKey StoredKey::createWithPrivateKey(const std::string& name, const Data& password, const Data& privateKeyData) {
    StoredKey key = StoredKey(StoredKeyType::privateKey, name, password, privateKeyData);
    return key;
}

StoredKey StoredKey::createWithPrivateKeyAddDefaultAddress(const std::string& name, const Data& password, TWCoinType coin, const Data& privateKeyData) {
    const auto curve = TW::curve(coin);
    if (!PrivateKey::isValid(privateKeyData, curve)) {
        throw std::invalid_argument("Invalid private key data");
    }

    StoredKey key = createWithPrivateKey(name, password, privateKeyData);

    const auto derivationPath = TW::derivationPath(coin);
    const auto address = TW::deriveAddress(coin, PrivateKey(privateKeyData));
    key.accounts.emplace_back(address, coin, derivationPath);

    return key;
}

StoredKey::StoredKey(StoredKeyType type, std::string name, const Data& password, const Data& data)
    : type(type), id(), name(std::move(name)), payload(password, data), accounts() {
    boost::uuids::random_generator gen;
    id = boost::lexical_cast<std::string>(gen());
}

const HDWallet StoredKey::wallet(const Data& password) const {
    if (type != StoredKeyType::mnemonicPhrase) {
        throw std::invalid_argument("Invalid account requested.");
    }
    const auto data = payload.decrypt(password);
    const auto mnemonic = std::string(reinterpret_cast<const char*>(data.data()), data.size());
    return HDWallet(mnemonic, "");
}

std::optional<const Account> StoredKey::account(TWCoinType coin) const {
    for (auto& account : accounts) {
        if (account.coin == coin) {
            return account;
        }
    }
    return std::nullopt;
}

std::optional<const Account> StoredKey::account(TWCoinType coin, const HDWallet* wallet) {
    if (wallet == nullptr) {
        return account(coin);
    }
    assert(wallet != nullptr);

    for (auto& account : accounts) {
        if (account.coin == coin) {
            if (account.address.empty()) {
                account.address = wallet->deriveAddress(coin);
            }
            return account;
        }
    }

    const auto derivationPath = TW::derivationPath(coin);
    const auto address = wallet->deriveAddress(coin);
    
    const auto version = TW::xpubVersion(coin);
    const auto extendedPublicKey = wallet->getExtendedPublicKey(derivationPath.purpose(), coin, version);

    accounts.emplace_back(address, coin, derivationPath, extendedPublicKey);
    return accounts.back();
}

void StoredKey::addAccount(const std::string& address, TWCoinType coin, const DerivationPath& derivationPath, const std::string& extetndedPublicKey) {
    accounts.emplace_back(address, coin, derivationPath, extetndedPublicKey);
}

void StoredKey::removeAccount(TWCoinType coin) {
    accounts.erase(std::remove_if(accounts.begin(), accounts.end(), [coin](Account& account) -> bool {
        return account.coin == coin;
    }), accounts.end());
}

const PrivateKey StoredKey::privateKey(TWCoinType coin, const Data& password) {
    switch (type) {
    case StoredKeyType::mnemonicPhrase: {
        const auto wallet = this->wallet(password);
        const auto account = this->account(coin, &wallet);
        return wallet.getKey(coin, account->derivationPath);
    }
    case StoredKeyType::privateKey:
        return PrivateKey(payload.decrypt(password));
    }
}

void StoredKey::fixAddresses(const Data& password) {
    switch (type) {
        case StoredKeyType::mnemonicPhrase: {
                const auto wallet = this->wallet(password);
                for (auto& account : accounts) {
                    if (!account.address.empty() && TW::validateAddress(account.coin, account.address)) {
                        continue;
                    }
                    const auto& derivationPath = account.derivationPath;
                    const auto key = wallet.getKey(account.coin, derivationPath);
                    account.address = TW::deriveAddress(account.coin, key);
                }
            }
            break;

        case StoredKeyType::privateKey: {
                auto key = PrivateKey(payload.decrypt(password));
                for (auto& account : accounts) {
                    if (!account.address.empty() && TW::validateAddress(account.coin, account.address)) {
                        continue;
                    }
                    account.address = TW::deriveAddress(account.coin, key);
                }
            }
            break;
    }
}


// -----------------
// Encoding/Decoding
// -----------------

StoredKey StoredKey::createWithJson(const nlohmann::json& json) {
    StoredKey storedKey;
    storedKey.loadJson(json);
    return storedKey;
}

namespace CodingKeys {
    static const auto address = "address";
    static const auto type = "type";
    static const auto name = "name";
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
} // namespace TypeString

void StoredKey::loadJson(const nlohmann::json& json) {
    if (json.count(CodingKeys::type) != 0 &&
        json[CodingKeys::type].get<std::string>() == TypeString::mnemonic) {
        type = StoredKeyType::mnemonicPhrase;
    } else {
        type = StoredKeyType::privateKey;
    }

    if (json.count(CodingKeys::name) != 0) {
        name = json[CodingKeys::name].get<std::string>();
    }

    if (json.count(CodingKeys::id) != 0) {
        id = json[CodingKeys::id].get<std::string>();
    }

    if (json.count(CodingKeys::crypto) != 0) {
        payload = EncryptionParameters(json[CodingKeys::crypto]);
    } else if (json.count(UppercaseCodingKeys::crypto) != 0) {
        // Workaround for myEtherWallet files
        payload = EncryptionParameters(json[UppercaseCodingKeys::crypto]);
    } else {
        throw DecryptionError::invalidKeyFile;
    }

    if (json.count(CodingKeys::activeAccounts) != 0 &&
        json[CodingKeys::activeAccounts].is_array()) {
        for (auto& accountJSON : json[CodingKeys::activeAccounts]) {
            accounts.emplace_back(accountJSON);
        }
    }

    if (accounts.empty() && json.count(CodingKeys::address) != 0 && json[CodingKeys::address].is_string()) {
        TWCoinType coin = TWCoinTypeEthereum;
        if (json.count(CodingKeys::coin) != 0) {
            coin = json[CodingKeys::coin].get<TWCoinType>();
        }
        auto address = json[CodingKeys::address].get<std::string>();
        accounts.emplace_back(address, coin, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(coin), 0, 0, 0));
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
    }

    if (id) {
        j[CodingKeys::id] = *id;
    }

    j[CodingKeys::name] = name;
    j[CodingKeys::crypto] = payload.json();

    nlohmann::json accountsJSON = nlohmann::json::array();
    for (const auto& account : accounts) {
        accountsJSON.push_back(account.json());
    }
    j[CodingKeys::activeAccounts] = accountsJSON;

    return j;
}

// File operations

void StoredKey::store(const std::string& path) {
    auto stream = std::ofstream(path);
    stream << json();
}

StoredKey StoredKey::load(const std::string& path) {
    std::ifstream stream(path);
    if (!stream.is_open()) {
        throw std::invalid_argument("Can't open file");
    }
    nlohmann::json j;
    stream >> j;

    return createWithJson(j);
}
