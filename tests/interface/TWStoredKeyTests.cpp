// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWAccount.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWStoredKey.h>

#include "../src/HexCoding.h"

#include <gtest/gtest.h>

extern std::string TESTS_ROOT;

using namespace TW;
using namespace std;


TEST(TWStoredKey, loadPBKDF2Key) {
    const auto filename = WRAPS(TWStringCreateWithUTF8Bytes((TESTS_ROOT + "/Keystore/Data/pbkdf2.json").c_str()));
    const auto key = TWStoredKeyLoad(filename.get());
    const auto keyId = WRAPS(TWStoredKeyIdentifier(key));
    EXPECT_EQ(string(TWStringUTF8Bytes(keyId.get())), "3198bc9c-6672-5ab3-d995-4942343ae5b6");
    TWStoredKeyDelete(key);
}

TEST(TWStoredKey, createWallet) {
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto password = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto key = TWStoredKeyCreate(name.get(), password.get());
    const auto name2 = WRAPS(TWStoredKeyName(key));
    EXPECT_EQ(string(TWStringUTF8Bytes(name2.get())), "name");
    const auto mnemonic = WRAPS(TWStoredKeyDecryptMnemonic(key, password.get()));
    // mnemonic is random, check that it's reasonably long
    EXPECT_TRUE(TWStringSize(mnemonic.get()) > 36);
    TWStoredKeyDelete(key);
}

TEST(TWStoredKey, importPrivateKey) {
    const auto privateKeyHex = "3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266";
    const auto privateKey = WRAPD(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes(privateKeyHex)));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto password = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto coin = TWCoinTypeBitcoin;
    const auto key = TWStoredKeyImportPrivateKey(privateKey.get(), name.get(), password.get(), coin);
    const auto privateKey2 = WRAPD(TWStoredKeyDecryptPrivateKey(key, password.get()));
    EXPECT_EQ(hex(data(TWDataBytes(privateKey2.get()), TWDataSize(privateKey2.get()))), privateKeyHex);
    
    const auto privateKey3 = TWStoredKeyPrivateKey(key, coin, password.get());
    const auto pkData3 = WRAPD(TWPrivateKeyData(privateKey3));
    EXPECT_EQ(hex(data(TWDataBytes(pkData3.get()), TWDataSize(pkData3.get()))), privateKeyHex);
    TWPrivateKeyDelete(privateKey3);

    TWStoredKeyDelete(key);
}

TEST(TWStoredKey, importHDWallet) {
    const auto mnemonic = WRAPS(TWStringCreateWithUTF8Bytes("team engine square letter hero song dizzy scrub tornado fabric divert saddle"));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto password = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto coin = TWCoinTypeBitcoin;
    const auto key = TWStoredKeyImportHDWallet(mnemonic.get(), name.get(), password.get(), coin);
    EXPECT_TRUE(TWStoredKeyIsMnemonic(key));

    const auto wallet = TWStoredKeyWallet(key, password.get());
    const auto accountCoin = TWStoredKeyAccountForCoin(key, coin, wallet);
    const auto accountAddress = WRAPS(TWAccountAddress(accountCoin));
    EXPECT_EQ(string(TWStringUTF8Bytes(accountAddress.get())), "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
    TWAccountDelete(accountCoin);

    EXPECT_EQ(TWStoredKeyAccountCount(key), 1);
    const auto accountIdx = TWStoredKeyAccount(key, 0);
    TWAccountDelete(accountIdx);

    TWStoredKeyRemoveAccountForCoin(key, coin);
    EXPECT_EQ(TWStoredKeyAccountCount(key), 0);

    TWStoredKeyDelete(key);
}

TEST(TWStoredKey, exportJSON) {
    const auto mnemonic = WRAPS(TWStringCreateWithUTF8Bytes("team engine square letter hero song dizzy scrub tornado fabric divert saddle"));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto password = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto coin = TWCoinTypeBitcoin;
    const auto key = TWStoredKeyImportHDWallet(mnemonic.get(), name.get(), password.get(), coin);
    const auto json = WRAPD(TWStoredKeyExportJSON(key));
    // check size and first character
    EXPECT_TRUE(TWDataSize(json.get()) > 100);
    EXPECT_EQ(TWDataGet(json.get(), 0), '{');
    TWStoredKeyDelete(key);
}

TEST(TWStoredKey, fixAddresses) {
    const auto mnemonic = WRAPS(TWStringCreateWithUTF8Bytes("team engine square letter hero song dizzy scrub tornado fabric divert saddle"));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto password = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto coin = TWCoinTypeBitcoin;
    const auto key = TWStoredKeyImportHDWallet(mnemonic.get(), name.get(), password.get(), coin);
    EXPECT_TRUE(TWStoredKeyFixAddresses(key, password.get()));
    TWStoredKeyDelete(key);
}

TEST(TWStoredKey, importInvalidKey) {
    auto bytes = TW::parse_hex("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
    auto data = WRAPD(TWDataCreateWithBytes(bytes.data(), bytes.size()));
    auto name = WRAPS(TWStringCreateWithUTF8Bytes("test"));

    auto eth = TWStoredKeyImportPrivateKey(data.get(), name.get(), name.get(), TWCoinTypeEthereum);
    auto ont = TWStoredKeyImportPrivateKey(data.get(), name.get(), name.get(), TWCoinTypeOntology);
    auto tezos = TWStoredKeyImportPrivateKey(data.get(), name.get(), name.get(), TWCoinTypeTezos);

    ASSERT_EQ(eth, nullptr);
    ASSERT_EQ(ont, nullptr);
    ASSERT_NE(tezos, nullptr);
}

TEST(TWStoredKey, removeAccountForCoin) {
    auto password = "password";
    auto key = TWStoredKeyCreate("Test KeyStore", password);
    auto wallet = TWStoredKeyWallet(key, password);
    
    ASSERT_NE(TWStoredKeyAccountForCoin(key, TWCoinTypeEthereum, wallet), nullptr);
    ASSERT_NE(TWStoredKeyAccountForCoin(key, TWCoinTypeBitcoin, wallet), nullptr);
    
    ASSERT_EQ(TWStoredKeyAccountCount(key), 2);
    
    TWStoredKeyRemoveAccountForCoin(key, TWCoinTypeBitcoin);
    
    ASSERT_EQ(TWStoredKeyAccountCount(key), 1);
    
    ASSERT_NE(TWStoredKeyAccountForCoin(key, TWCoinTypeEthereum, nullptr), nullptr);
    ASSERT_EQ(TWStoredKeyAccountForCoin(key, TWCoinTypeBitcoin, nullptr), nullptr);
}
