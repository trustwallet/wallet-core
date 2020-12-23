// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWStoreWallet.h>
#include <TrustWalletCore/TWStoredKey.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "../src/Data.h"
#include "../src/HexCoding.h"

#include <gtest/gtest.h>

#include <memory>

using namespace TW;
using namespace std;

const char* password = "password";
const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
const auto passwordData = WRAPD(TWDataCreateWithBytes((const unsigned char*)password, strlen(password)));
const auto mnemonic = WRAPS(TWStringCreateWithUTF8Bytes("team engine square letter hero song dizzy scrub tornado fabric divert saddle"));
const auto walletPath = "testwallet";

std::shared_ptr<TWStoreWallet> createWallet(const char* path = walletPath) {
    auto keyStore = WRAP(TWStoredKey, TWStoredKeyCreateWithMnemonic(WRAPS(TWStringCreateWithUTF8Bytes("name")).get(), passwordData.get(), mnemonic.get()));
    auto fullPath = WRAPS(TWStringCreateWithUTF8Bytes((string("./") + path).c_str()));
    return std::shared_ptr<TWStoreWallet>(TWStoreWalletCreate(fullPath.get(), keyStore.get()), TWStoreWalletDelete);
}

TEST(TWStoreWallet, create) {
    auto wallet = createWallet();
    EXPECT_EQ(std::string(TWStringUTF8Bytes(TWStoreWalletIndentifier(wallet.get()))), walletPath);
}

TEST(TWStoreWallet, getAccount) {
    auto wallet = createWallet();
    TWCoinType coin = TWCoinTypeBitcoin;
    auto account = WRAP(TWAccount, TWStoreWalletGetAccount(wallet.get(), passwordString.get(), coin));
    EXPECT_EQ(TWAccountCoin(account.get()), coin);
    EXPECT_EQ(*reinterpret_cast<const std::string*>(WRAPS(TWAccountAddress(account.get())).get()), "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
}

TEST(TWStoreWallet, privateKey) {
    auto wallet = createWallet();
    TWCoinType coin = TWCoinTypeBitcoin;
    auto privateKey = WRAP(TWPrivateKey, TWStoreWalletPrivateKey(wallet.get(), passwordString.get(), coin));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    EXPECT_EQ(hex(TW::data(TWDataBytes(privateKeyData.get()), TWDataSize(privateKeyData.get()))), 
        "d2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f");
}

TEST(TWStoreWallet, equal) {
    auto wallet1 = createWallet();
    auto wallet2 = createWallet();
    auto wallet3 = createWallet("testWallet3Other");
    EXPECT_TRUE(TWStoreWalletEqual(wallet1.get(), wallet2.get()));
    EXPECT_FALSE(TWStoreWalletEqual(wallet1.get(), wallet3.get()));
}
