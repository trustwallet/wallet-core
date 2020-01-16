// Copyright © 2017-2020 Trust Wallet.
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
#include <TrustWalletCore/TWAnyAddress.h>

#include <gtest/gtest.h>

TEST(Zilliqa, Address) {

    auto string = STRING("zil1mk6pqphhkmaguhalq6n3cq0h38ltcehg0rfmv6");
    EXPECT_TRUE(TWAnyAddressIsValidString(string.get(), TWCoinTypeZilliqa));

    EXPECT_FALSE(TWAnyAddressIsValidString(STRING("0x7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C").get(), TWCoinTypeZilliqa));
    EXPECT_FALSE(TWAnyAddressIsValidString(STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4").get(), TWCoinTypeZilliqa));
    EXPECT_FALSE(TWAnyAddressCreateWithString(STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4").get(), TWCoinTypeZilliqa) != NULL);

    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeZilliqa));
    auto desc = WRAPS(TWAnyAddressDescription(address.get()));
    auto keyHash = WRAPD(TWAnyAddressData(address.get()));
    auto keyHashString = WRAPS(TWStringCreateWithUTF8Bytes((const char *)TWDataBytes(keyHash.get())));

    assertStringsEqual(desc, "zil1mk6pqphhkmaguhalq6n3cq0h38ltcehg0rfmv6");
    assertStringsEqual(keyHashString, "0xDdb41006F7B6FA8e5FBF06A71c01F789FeBC66e8");
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
