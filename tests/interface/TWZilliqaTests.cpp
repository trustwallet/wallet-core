// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"

#include "Zilliqa/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWBlockchain.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWZilliqaAddress.h>

#include <gtest/gtest.h>

TEST(Zilliqa, Address) {

    auto string = STRING("zil1mk6pqphhkmaguhalq6n3cq0h38ltcehg0rfmv6");
    EXPECT_TRUE(TWZilliqaAddressIsValidString(string.get()));

    EXPECT_FALSE(TWZilliqaAddressIsValidString(STRING("0x7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C").get()));
    EXPECT_FALSE(TWZilliqaAddressIsValidString(STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4").get()));
    EXPECT_FALSE(TWZilliqaAddressCreateWithString(STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4").get()) != NULL);

    auto address = WRAP(TWZilliqaAddress, TWZilliqaAddressCreateWithString(string.get()));
    auto desc = WRAPS(TWZilliqaAddressDescription(address.get()));
    auto keyHashString = WRAPS(TWZilliqaAddressKeyHash(address.get()));
    auto keyHash = WRAPD(TWDataCreateWithHexString(keyHashString.get()));
    auto addressFromKeyHash = WRAP(TWZilliqaAddress, TWZilliqaAddressCreateWithKeyHash(keyHash.get()));

    assertStringsEqual(desc, "zil1mk6pqphhkmaguhalq6n3cq0h38ltcehg0rfmv6");
    assertStringsEqual(keyHashString, "0xDdb41006F7B6FA8e5FBF06A71c01F789FeBC66e8");
    EXPECT_TRUE(TWZilliqaAddressEqual(address.get(), addressFromKeyHash.get()));
}

TEST(Zilliqa, Signing) {

    uint8_t bytes[] = {0xaf, 0xee, 0xfc, 0xa7, 0x4d, 0x9a, 0x32, 0x5c, 0xf1, 0xd6, 0xb6, 0x91, 0x1d, 0x61, 0xa6, 0x5c, 0x32, 0xaf, 0xa8, 0xe0, 0x2b, 0xd5, 0xe7, 0x8e, 0x2e, 0x4a, 0xc2, 0x91, 0x0b, 0xab, 0x45, 0xf5};
    auto keyData = WRAPD(TWDataCreateWithBytes(bytes, 32));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(keyData.get()));
    auto pubKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);

    auto message = "hello schnorr";
    auto data = WRAPD(TWDataCreateWithBytes((uint8_t *)message, strlen(message)));

    auto signature = WRAPD(TWPrivateKeySignSchnorr(privateKey.get(), data.get(), TWCurveSECP256k1));
    auto expected = TW::parse_hex("d166b1ae7892c5ef541461dc12a50214d0681b63d8037cda29a3fe6af8bb973e4ea94624d85bc0010bdc1b38d05198328fae21254adc2bf5feaf2804d54dba55");
    for (auto i = 0; i < sizeof(expected); i += 1) {
        ASSERT_EQ(TWDataBytes(signature.get())[i], expected[i]);
    }

    ASSERT_TRUE(TWPublicKeyVerifySchnorr(pubKey, signature.get(), data.get()));
}

TEST(Zilliqa, Signing2) {
    // wif: KwLrLzvsh4GYAk6Zzjs5ThciutYCGuHYtXgqhnQKXYSvb8b5YhTW
    // ec: 03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4
    uint8_t bytes[] = {0x03, 0xac, 0x67, 0x42, 0x16, 0xf3, 0xe1, 0x5c, 0x76, 0x1e, 0xe1, 0xa5, 0xe2, 0x55, 0xf0, 0x67, 0x95, 0x36, 0x23, 0xc8, 0xb3, 0x88, 0xb4, 0x45, 0x9e, 0x13, 0xf9, 0x78, 0xd7, 0xc8, 0x46, 0xf4};
    auto keyData = WRAPD(TWDataCreateWithBytes(bytes, 32));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(keyData.get()));
    auto pubKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);

    // auto message = "hello schnorr";
    // 72e9ae224ac72757767c30ae92f0ac39e1aa6b12d8d59c832e2dbaa6d2226c66
    // uint8_t message[] = {0x72, 0xe9, 0xae, 0x22, 0x4a, 0xc7, 0x27, 0x57, 0x76, 0x7c, 0x30, 0xae, 0x92, 0xf0, 0xac, 0x39, 0xe1, 0xaa, 0x6b, 0x12, 0xd8, 0xd5, 0x9c, 0x83, 0x2e, 0x2d, 0xba, 0xa6, 0xd2, 0x22, 0x6c, 0x66};
    uint8_t message[] = {0x66, 0x6c, 0x22, 0xd2, 0xa6, 0xba, 0x2d, 0x2e, 0x83, 0x9c, 0xd5, 0xd8, 0x12, 0x6b, 0xaa, 0xe1, 0x39, 0xac, 0xf0, 0x92, 0xae, 0x30, 0x7c, 0x76, 0x57, 0x27, 0xc7, 0x4a, 0x22, 0xae, 0xe9, 0x72};
    auto data = WRAPD(TWDataCreateWithBytes((uint8_t *)message, 32));

    auto signature = WRAPD(TWPrivateKeySignSchnorr(privateKey.get(), data.get(), TWCurveSECP256k1));

    assertHexEqual(signature, "8edf22b204373c377347dadfbd396ceab7ee35f8c83f04b4cc142cc392e7ca3bffce40b513dee9b0b816f8588e003c442b2f66185b7bb16d18a3e8af000b0b5e");

    ASSERT_TRUE(TWPublicKeyVerifySchnorr(pubKey, signature.get(), data.get()));
}
