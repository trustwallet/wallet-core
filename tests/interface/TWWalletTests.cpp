// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWWallet.h>
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

std::shared_ptr<TWWallet> createWallet() {
    auto keyStore = WRAP(TWStoredKey, TWStoredKeyCreateWithMnemonic(WRAPS(TWStringCreateWithUTF8Bytes("name")).get(), passwordData.get(), mnemonic.get()));
    auto path = WRAPS(TWStringCreateWithUTF8Bytes((string("./") + walletPath).c_str()));
    return std::shared_ptr<TWWallet>(TWWalletCreate(path.get(), keyStore.get()), TWWalletDelete);
}

TEST(TWWallet, create) {
    auto wallet = createWallet();
    EXPECT_EQ(std::string(TWStringUTF8Bytes(TWWalletIndentifier(wallet.get()))), walletPath);
}

TEST(TWWallet, getAccount) {
    auto wallet = createWallet();
    TWCoinType coin = TWCoinTypeBitcoin;
    auto account = WRAP(TWAnyAddress, TWWalletGetAccount(wallet.get(), passwordString.get(), coin));
    EXPECT_EQ(TWAnyAddressCoin(account.get()), coin);
    EXPECT_EQ(*reinterpret_cast<const std::string*>(WRAPS(TWAnyAddressDescription(account.get())).get()), "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
}

TEST(TWWallet, privateKey) {
    auto wallet = createWallet();
    TWCoinType coin = TWCoinTypeBitcoin;
    auto privateKey = WRAP(TWPrivateKey, TWWalletPrivateKey(wallet.get(), passwordString.get(), coin));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    EXPECT_EQ(hex(TW::data(TWDataBytes(privateKeyData.get()), TWDataSize(privateKeyData.get()))), 
        "d2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f");
}
