// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include "HexCoding.h"
#include "Data.h"
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(ZilliqaSignature, Signing) {
    auto keyData = WRAPD(TWDataCreateWithHexString(STRING("0xafeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5").get()));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(keyData.get()));
    auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));

    auto message = "hello schnorr";
    auto messageData = WRAPD(TWDataCreateWithBytes((uint8_t *)message, strnlen(message, 13)));
    auto signatureData = WRAPD(TWPrivateKeySignZilliqaSchnorr(privateKey.get(), messageData.get()));
    auto signature = data(TWDataBytes(signatureData.get()), TWDataSize(signatureData.get()));

    ASSERT_TRUE(TWPublicKeyVerifyZilliqaSchnorr(pubKey.get(), signatureData.get(), messageData.get()));
    EXPECT_EQ(hex(signature), "d166b1ae7892c5ef541461dc12a50214d0681b63d8037cda29a3fe6af8bb973e4ea94624d85bc0010bdc1b38d05198328fae21254adc2bf5feaf2804d54dba55");
}
