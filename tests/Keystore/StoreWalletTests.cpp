// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Keystore/StoreWallet.h"

#include "Coin.h"
#include "HexCoding.h"
#include "Data.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>
#include <iostream>

extern std::string TESTS_ROOT;

namespace TW::Keystore {

using namespace std;

const auto password = "password";
const auto passwordString = string(password);
const auto passwordData = TW::data(string(passwordString));
const auto mnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
const auto walletPath = "testwallet";

StoreWallet createWallet() {
    auto keyStore = StoredKey::createWithMnemonic("name", passwordData, mnemonic);
    StoreWallet wallet(string("./") + walletPath, keyStore);
    return wallet;
}

TEST(StoreWallet, fileFromPath) {
    EXPECT_EQ(StoreWallet::fileFromPath("/d1/d2/file.3"), "file.3");
    EXPECT_EQ(StoreWallet::fileFromPath("file4"), "file4");
    EXPECT_EQ(StoreWallet::fileFromPath(""), "");
    EXPECT_EQ(StoreWallet::fileFromPath("/"), "");
    EXPECT_EQ(StoreWallet::fileFromPath("d/"), "");
    EXPECT_EQ(StoreWallet::fileFromPath("/file5"), "file5");
    EXPECT_EQ(StoreWallet::fileFromPath("d1///d2//file3"), "file3");
}

TEST(StoreWallet, create) {
    StoreWallet wallet = createWallet();
    EXPECT_EQ(wallet.getIdentifier(), walletPath);
    EXPECT_EQ(wallet.storedKey().name, "name");
}

TEST(StoreWallet, getAccount) {
    StoreWallet wallet = createWallet();
    TWCoinType coin = TWCoinTypeBitcoin;
    Account account = wallet.getAccount(passwordString, coin);
    EXPECT_EQ(account.coin, coin);
    EXPECT_EQ(account.address, "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
}

TEST(StoreWallet, getAccounts) {
    StoreWallet wallet = createWallet();
    vector<TWCoinType> coins = {TWCoinTypeBitcoin, TWCoinTypeEthereum, TWCoinTypeBinance};
    vector<string> expectedAddresses = {
        "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny",
        "0x494f60cb6Ac2c8F5E1393aD9FdBdF4Ad589507F7",
        "bnb1vp6g8w35c56qrdkefpjwyndaaavetasgcdclrg"
    };
    vector<Account> accounts = wallet.getAccounts(passwordString, coins);
    ASSERT_EQ(accounts.size(), coins.size());
    for (auto i = 0; i < coins.size(); ++i) {
        EXPECT_EQ(accounts[i].coin, coins[i]);
        EXPECT_EQ(accounts[i].address, expectedAddresses[i]);
    }
}

TEST(StoreWallet, privateKey) {
    StoreWallet wallet = createWallet();
    TWCoinType coin = TWCoinTypeBitcoin;
    PrivateKey privateKey = wallet.privateKey(password, coin);
    EXPECT_EQ(hex(privateKey.bytes), "d2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f");
}

TEST(StoreWallet, equals) {
    auto keyStore1 = StoredKey::createWithMnemonic("name1", passwordData, mnemonic);
    StoreWallet wallet1(string("./") + walletPath + "1", keyStore1);
    auto keyStore2 = StoredKey::createWithMnemonic("name2", passwordData, mnemonic);
    StoreWallet wallet2(string("./") + walletPath + "1", keyStore2);
    auto keyStore3 = StoredKey::createWithMnemonic("name3", passwordData, mnemonic);
    StoreWallet wallet3(string("./") + walletPath + "3", keyStore3);
    
    EXPECT_TRUE(wallet1 == wallet1);
    EXPECT_TRUE(wallet1 == wallet2);
    EXPECT_FALSE(wallet1 == wallet3);
    EXPECT_TRUE(wallet2 == wallet1);
    EXPECT_TRUE(wallet2 == wallet2);
    EXPECT_FALSE(wallet2 == wallet3);
    EXPECT_FALSE(wallet3 == wallet1);
    EXPECT_FALSE(wallet3 == wallet2);
    EXPECT_TRUE(wallet3 == wallet3);
}

} // namespace TW::Keystore
