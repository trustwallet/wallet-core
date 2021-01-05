// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "StoredKey.h"
#include "Account.h"
#include "../Data.h"
#include "../PrivateKey.h"
#include <TrustWalletCore/TWCoinType.h>

#include <string>
#include <vector>

namespace TW::Keystore {

/// Coin wallet.
class StoreWallet {
private:
    /// Unique wallet identifier.
    std::string identifier;

    /// URL for the key file on disk.
    std::string path;

    /// Encrypted wallet key
    StoredKey key;

public:
    /// Creates a StoreWallet from an encrypted key.
    StoreWallet(std::string path, const StoredKey& key);

    std::string getIdentifier() const { return identifier; }

    const StoredKey& storedKey() const { return key; }

    /// Returns the account for a specific coin.
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coin: coin type
    /// - Returns: the account
    /// - Throws: invalid_argument("Invalid password") if the password is incorrect.
    const Account getAccount(const std::string& password, TWCoinType coin);

    /// Returns the accounts for a specific coins.
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coins: coins to add accounts for
    /// - Returns: the added accounts
    /// - Throws: invalid_argument("Invalid password") if the password is incorrect.
    const std::vector<Account> getAccounts(const std::string& password, const std::vector<TWCoinType>& coins);

    /// Returns the private key for a specific coin.
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coin: coin type
    /// - Returns: the private key
    PrivateKey privateKey(const std::string& password, TWCoinType coin);

    bool operator==(const StoreWallet& w2);

    static std::string fileFromPath(const std::string& path);
};

} // namespace TW::Keystore

/// Wrapper for C interface.
struct TWStoreWallet {
    TW::Keystore::StoreWallet impl;
};
