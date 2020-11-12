// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Wallet.h"

using namespace TW::Keystore;
using namespace TW;

Wallet::Wallet(std::string path, const StoredKey& key): path(path), key(key) {
    identifier = std::filesystem::path(path).filename();
}

const Account Wallet::getAccount(const std::string& password, TWCoinType coin) {
    auto wallet = key.wallet(data(password));
    auto account = key.account(coin, &wallet);
    if (!account) {
        throw std::invalid_argument("Invalid password");
    }
    return account.value();
}

const std::vector<Account> Wallet::getAccounts(const std::string& password, const std::vector<TWCoinType>& coins) {
    auto wallet = key.wallet(data(password));
    std::vector<Account> accounts;
    for(auto coin: coins) {
        auto account = key.account(coin, &wallet);
        if (!account) {
            throw std::invalid_argument("Invalid password");
        }
        accounts.push_back(account.value());
    }
    return accounts;
}

PrivateKey Wallet::privateKey(const std::string& password, TWCoinType coin) {
    auto privateKey = key.privateKey(coin, data(password));
    return privateKey;
}

bool Wallet::operator==(const Wallet& w2) {
    return identifier == w2.identifier;
}
