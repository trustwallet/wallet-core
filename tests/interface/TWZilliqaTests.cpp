// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWBlockchain.h>
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

TEST(ZilliqaTests, Config) {
    ASSERT_EQ(TWBlockchainZilliqa, TWCoinTypeBlockchain(TWCoinTypeZilliqa));
}

TEST(ZilliqaTests, Sign) {

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
