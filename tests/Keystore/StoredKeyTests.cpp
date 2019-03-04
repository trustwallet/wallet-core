// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Keystore/StoredKey.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

extern std::string TESTS_ROOT;

namespace TW {
namespace Keystore {

TEST(StoredKey, LoadLegacyPrivateKey) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/legacy-private-key.json", "");
    ASSERT_EQ(key.id, "3051ca7d-3d36-4a4a-acc2-09e9083732b0");
}

TEST(StoredKey, LoadLegacyMnemonic) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/legacy-mnemonic.json", "");
    ASSERT_EQ(key.id, "629aad29-0b22-488e-a0e7-b4219d4f311c");
}

TEST(StoredKey, ReadWallet) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key.json", "");

    ASSERT_EQ(key.id, "e13b209c-3b2f-4327-bab0-3bef2e51630d");

    const auto header = key.payload;

    ASSERT_EQ(header.cipher, "aes-128-ctr");
    ASSERT_EQ(hex(header.encrypted), "d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c");
    ASSERT_EQ(header.kdf, "scrypt");
    ASSERT_EQ(hex(header.mac), "2103ac29920d71da29f15d75b4a16dbe95cfd7ff8faea1056c33131d846e3097");
    ASSERT_EQ(hex(header.cipherParams.iv), "83dbcc02d8ccb40e466191a123791e0e");
    ASSERT_EQ(header.kdfParams.desiredKeyLength, 32);
    ASSERT_EQ(header.kdfParams.n, 262144);
    ASSERT_EQ(header.kdfParams.p, 8);
    ASSERT_EQ(header.kdfParams.r, 1);
    ASSERT_EQ(hex(header.kdfParams.salt), "ab0c7876052600dd703518d6fc3fe8984592145b591fc8fb5c6d43190334ba19");
}

TEST(StoredKey, ReadMyEtherWallet) {
    ASSERT_NO_THROW(StoredKey::load(TESTS_ROOT + "/Keystore/Data/myetherwallet.uu", ""));
}

TEST(StoredKey, InvalidPassword) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key.json", "");

    ASSERT_THROW(key.payload.decrypt("password"), DecryptionError);
}

TEST(StoredKey, Decrypt) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key.json", "");
    const auto privateKey = key.payload.decrypt("testpassword");

    ASSERT_EQ(hex(privateKey), "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d");
}

TEST(StoredKey, CreateWallet) {
    const auto privateKey = parse_hex("3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266");
    const auto key = StoredKey(StoredKeyType::privateKey, "password", privateKey);
    const auto decrypted = key.payload.decrypt("password");

    ASSERT_EQ(hex(decrypted), hex(privateKey));
}

TEST(StoredKey, DecodingEthereumAddress) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key.json", "");

    ASSERT_EQ(key.accounts[0].address, "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b");
}

TEST(StoredKey, DecodingBitcoinAddress) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key_bitcoin.json", "");

    ASSERT_EQ(key.accounts[0].address, "3PWazDi9n1Hfyq9gXFxDxzADNL8RNYyK2y");
}

}} // namespace
