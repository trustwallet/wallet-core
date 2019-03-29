// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Keystore/StoredKey.h"
#include "PrivateKey.h"

#include <stdexcept>
#include <gtest/gtest.h>

extern std::string TESTS_ROOT;

namespace TW {
namespace Keystore {

TEST(StoredKey, LoadNonexistent) {
    ASSERT_THROW(StoredKey::load(TESTS_ROOT + "/Keystore/Data/nonexistent.json"), std::invalid_argument);
}

TEST(StoredKey, LoadLegacyPrivateKey) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/legacy-private-key.json");
    EXPECT_EQ(key.id, "3051ca7d-3d36-4a4a-acc2-09e9083732b0");
    EXPECT_EQ(key.accounts[0].coin(), TWCoinTypeEthereum);
    EXPECT_EQ(hex(key.payload.decrypt("testpassword")), "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d");
}

TEST(StoredKey, LoadLegacyMnemonic) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/legacy-mnemonic.json");
    EXPECT_EQ(key.id, "629aad29-0b22-488e-a0e7-b4219d4f311c");

    const auto data = key.payload.decrypt("password");
    const auto mnemonic = std::string(reinterpret_cast<const char*>(data.data()));
    EXPECT_EQ(mnemonic, "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn back");

    EXPECT_EQ(key.accounts[0].coin(), TWCoinTypeEthereum);
    EXPECT_EQ(key.accounts[0].derivationPath.string(), "m/44'/60'/0'/0/0");
    EXPECT_EQ(key.accounts[0].address, "0xeDe8F58dADa22c3A49dB60D4f82BAD428ab65F89");
    EXPECT_EQ(key.accounts[1].coin(), TWCoinTypeBitcoin);
    EXPECT_EQ(key.accounts[1].derivationPath.string(), "m/84'/0'/0'/0/0");
    EXPECT_EQ(key.accounts[1].address, "bc1q2ddhp55sq2l4xnqhpdv0xazg02v9dr7uu8c2p2");
    EXPECT_EQ(key.accounts[1].extendedPublicKey, "zpub6r97AegwVxVbJeuDAWP5KQgX5y4Q6KyFUrsFQRn8yzSXrnmpwg1ZKHSWwECR1Kiqgr4h93WN5kdS48KC6hVFniuZHqVFXjULZZkCwurqyPn");
}

TEST(StoredKey, ReadWallet) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key.json");

    EXPECT_EQ(key.id, "e13b209c-3b2f-4327-bab0-3bef2e51630d");

    const auto header = key.payload;

    EXPECT_EQ(header.cipher, "aes-128-ctr");
    EXPECT_EQ(hex(header.encrypted), "d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c");
    EXPECT_EQ(header.kdf, "scrypt");
    EXPECT_EQ(hex(header.mac), "2103ac29920d71da29f15d75b4a16dbe95cfd7ff8faea1056c33131d846e3097");
    EXPECT_EQ(hex(header.cipherParams.iv), "83dbcc02d8ccb40e466191a123791e0e");
    EXPECT_EQ(header.kdfParams.desiredKeyLength, 32);
    EXPECT_EQ(header.kdfParams.n, 262144);
    EXPECT_EQ(header.kdfParams.p, 8);
    EXPECT_EQ(header.kdfParams.r, 1);
    EXPECT_EQ(hex(header.kdfParams.salt), "ab0c7876052600dd703518d6fc3fe8984592145b591fc8fb5c6d43190334ba19");
}

TEST(StoredKey, ReadMyEtherWallet) {
    ASSERT_NO_THROW(StoredKey::load(TESTS_ROOT + "/Keystore/Data/myetherwallet.uu"));
}

TEST(StoredKey, InvalidPassword) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key.json");

    ASSERT_THROW(key.payload.decrypt("password"), DecryptionError);
}

TEST(StoredKey, Decrypt) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key.json");
    const auto privateKey = key.payload.decrypt("testpassword");

    EXPECT_EQ(hex(privateKey), "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d");
}

TEST(StoredKey, CreateWallet) {
    const auto privateKey = parse_hex("3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266");
    const auto key = StoredKey(StoredKeyType::privateKey, "password", privateKey);
    const auto decrypted = key.payload.decrypt("password");

    EXPECT_EQ(hex(decrypted), hex(privateKey));
}

TEST(StoredKey, DecodingEthereumAddress) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key.json");

    EXPECT_EQ(key.accounts[0].address, "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b");
}

TEST(StoredKey, DecodingBitcoinAddress) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/key_bitcoin.json");

    EXPECT_EQ(key.accounts[0].address, "3PWazDi9n1Hfyq9gXFxDxzADNL8RNYyK2y");
}

TEST(StoredKey, WatchAddress) {
    const auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/watch.json");

    EXPECT_EQ(key.accounts[0].address, "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b");
    EXPECT_EQ(key.accounts[0].derivationPath.string(), "m/44'/60'/0'/0/0");
}

TEST(StoredKey, MissingAddress) {
    auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/missing-address.json");
    key.fixAddresses("password");

    EXPECT_EQ(key.account(TWCoinTypeEthereum, nullptr)->address, "0x04De84ec355BAe81b51cD53Fdc8AA30A61872C95");
    EXPECT_EQ(key.account(TWCoinTypeBitcoin, nullptr)->address, "bc1qe938ncm8fhdqg27xmxd7lq02jz9xh0x48r22lc");
}

TEST(StoredKey, EtherWalletAddressNo0x) {
    auto key = StoredKey::load(TESTS_ROOT + "/Keystore/Data/ethereum-wallet-address-no-0x.json");
    key.fixAddresses("15748c4e3dca6ae2110535576ab0c398cb79d985707c68ee6c9f9df9d421dd53");
    EXPECT_EQ(key.account(TWCoinTypeEthereum, nullptr)->address, "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309");
}

}} // namespace
