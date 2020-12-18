// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StoreWallet.h"

using namespace TW::Keystore;
using namespace TW;

std::string StoreWallet::fileFromPath(const std::string& path) {
    std::size_t lastSeparator = path.find_last_of("/");
    if (lastSeparator == std::string::npos) {
        return path;
    }
    return path.substr(lastSeparator + 1, path.length());
    /*
        Alternatives:
        return std::filesystem::path(path).filename();  <-- std::filesystem has issues on iOS < 13.0
        return boost::filesystem::path(path).filename().string();  <-- uses Boost::filesystem
    */
}

StoreWallet::StoreWallet(std::string path, const StoredKey& key): path(path), key(key) {
    identifier = fileFromPath(path);
}

const Account StoreWallet::getAccount(const std::string& password, TWCoinType coin) {
    auto wallet = key.wallet(data(password));
    auto account = key.account(coin, &wallet);
    if (!account) {
        throw std::invalid_argument("Invalid password");
    }
    return *account; // account.value();
}

const std::vector<Account> StoreWallet::getAccounts(const std::string& password, const std::vector<TWCoinType>& coins) {
    auto wallet = key.wallet(data(password));
    std::vector<Account> accounts;
    for(auto coin: coins) {
        auto account = key.account(coin, &wallet);
        if (!account) {
            throw std::invalid_argument("Invalid password");
        }
        accounts.push_back(*account); // account.value()
    }
    return accounts;
}

PrivateKey StoreWallet::privateKey(const std::string& password, TWCoinType coin) {
    auto privateKey = key.privateKey(coin, data(password));
    return privateKey;
}

bool StoreWallet::operator==(const StoreWallet& w2) {
    return identifier == w2.identifier;
}
