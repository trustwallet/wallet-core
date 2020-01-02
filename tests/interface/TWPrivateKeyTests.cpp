// Copyright © 2017-2020 Trust Wallet.
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

#include <gtest/gtest.h>

TEST(PrivateKeyTests, CreateInvalid) {
    uint8_t bytes[] = {0xde, 0xad, 0xbe, 0xef};
    auto data = WRAPD(TWDataCreateWithBytes(bytes, 4));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(data.get()));

    ASSERT_EQ(privateKey.get(), nullptr);
}

TEST(PrivateKeyTests, AllZeros) {
    auto bytes = TW::Data(32);
    auto data = WRAPD(TWDataCreateWithBytes(bytes.data(), bytes.size()));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(data.get()));

    ASSERT_EQ(privateKey.get(), nullptr);
}

TEST(PrivateKeyTests, Invalid) {
    auto bytes = TW::parse_hex("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
    auto valid = TW::PrivateKey::isValid(bytes, TWCurveSECP256k1);

    auto bytes2 = TW::parse_hex("0000000000000000000000000000000000000000000000000000000000000000");
    auto valid2 = TW::PrivateKey::isValid(bytes2, TWCurveSECP256k1);

    ASSERT_EQ(valid, false);
    ASSERT_EQ(valid2, false);
}

TEST(PrivateKeyTests, IsValid) {
    uint8_t bytes[] = {0xaf, 0xee, 0xfc, 0xa7, 0x4d, 0x9a, 0x32, 0x5c, 0xf1, 0xd6, 0xb6, 0x91, 0x1d, 0x61, 0xa6, 0x5c, 0x32, 0xaf, 0xa8, 0xe0, 0x2b, 0xd5, 0xe7, 0x8e, 0x2e, 0x4a, 0xc2, 0x91, 0x0b, 0xab, 0x45, 0xf5};
    auto data = WRAPD(TWDataCreateWithBytes(bytes, 32));

    ASSERT_TRUE(TWPrivateKeyIsValid(data.get(), TWCurveSECP256k1));
    ASSERT_TRUE(TWPrivateKeyIsValid(data.get(), TWCurveED25519));
}

TEST(PrivateKeyTests, PublicKey) {
    uint8_t bytes[] = {0xaf, 0xee, 0xfc, 0xa7, 0x4d, 0x9a, 0x32, 0x5c, 0xf1, 0xd6, 0xb6, 0x91, 0x1d, 0x61, 0xa6, 0x5c, 0x32, 0xaf, 0xa8, 0xe0, 0x2b, 0xd5, 0xe7, 0x8e, 0x2e, 0x4a, 0xc2, 0x91, 0x0b, 0xab, 0x45, 0xf5};
    auto data = WRAPD(TWDataCreateWithBytes(bytes, 32));
    auto privateKey = TWPrivateKeyCreateWithData(data.get());
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey, false);

    uint8_t expected[] = {0x04, 0x99, 0xc6, 0xf5, 0x1a, 0xd6, 0xf9, 0x8c, 0x9c, 0x58, 0x3f, 0x8e, 0x92, 0xbb, 0x77, 0x58, 0xab, 0x2c, 0xa9, 0xa0, 0x41, 0x10, 0xc0, 0xa1, 0x12, 0x6e, 0xc4, 0x3e, 0x54, 0x53, 0xd1, 0x96, 0xc1, 0x66, 0xb4, 0x89, 0xa4, 0xb7, 0xc4, 0x91, 0xe7, 0x68, 0x8e, 0x6e, 0xbe, 0xa3, 0xa7, 0x1f, 0xc3, 0xa1, 0xa4, 0x8d, 0x60, 0xf9, 0x8d, 0x5c, 0xe8, 0x4c, 0x93, 0xb6, 0x5e, 0x42, 0x3f, 0xde, 0x91};
    for (auto i = 0; i < sizeof(expected); i += 1) {
        ASSERT_EQ(publicKey->impl.bytes[i], expected[i]);
    }
}

TEST(PrivateKeyTests, ClearMemory) {
    auto privKey = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
    auto privKeyData = TW::parse_hex(privKey);
    auto data = WRAPD(TWDataCreateWithBytes(privKeyData.data(), privKeyData.size()));
    auto privateKey = TWPrivateKeyCreateWithData(data.get());
    auto ptr = privateKey->impl.bytes.data();
    ASSERT_EQ(privKey, TW::hex(TW::data(ptr, 32)));

    TWPrivateKeyDelete(privateKey);

    // Memory cleaned (filled with 0s).  They may be overwritten by something else; we check that it is not equal to original, most of it has changed.
    ASSERT_NE(privKey, TW::hex(TW::data(ptr, 32)));
    int countDifferent = 0;
    for (auto i = 0; i < privKeyData.size(); ++i) {
        countDifferent += (int)(ptr[i] != privKeyData[i]);
    }
    ASSERT_GE(countDifferent, 32*2/3);
}

TEST(PrivateKeyTests, Sign) {
    uint8_t bytes[] = {0xaf, 0xee, 0xfc, 0xa7, 0x4d, 0x9a, 0x32, 0x5c, 0xf1, 0xd6, 0xb6, 0x91, 0x1d, 0x61, 0xa6, 0x5c, 0x32, 0xaf, 0xa8, 0xe0, 0x2b, 0xd5, 0xe7, 0x8e, 0x2e, 0x4a, 0xc2, 0x91, 0x0b, 0xab, 0x45, 0xf5};
    auto keyData = WRAPD(TWDataCreateWithBytes(bytes, 32));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(keyData.get()));

    auto message = "hello";
    auto data = WRAPD(TWDataCreateWithBytes((uint8_t *)message, strlen(message)));
    auto hash = WRAPD(TWHashKeccak256(data.get()));

    auto actual = WRAPD(TWPrivateKeySign(privateKey.get(), hash.get(), TWCurveSECP256k1));

    uint8_t expected[] = {0x87, 0x20, 0xa4, 0x6b, 0x5b, 0x39, 0x63, 0x79, 0x0d, 0x94, 0xbc, 0xc6, 0x1a, 0xd5, 0x7c, 0xa0, 0x2f, 0xd1, 0x53, 0x58, 0x43, 0x15, 0xbf, 0xa1, 0x61, 0xed, 0x34, 0x55, 0xe3, 0x36, 0xba, 0x62, 0x4d, 0x68, 0xdf, 0x01, 0x0e, 0xd9, 0x34, 0xb8, 0x79, 0x2c, 0x5b, 0x6a, 0x57, 0xba, 0x86, 0xc3, 0xda, 0x31, 0xd0, 0x39, 0xf9, 0x61, 0x2b, 0x44, 0xd1, 0xbf, 0x05, 0x41, 0x32, 0x25, 0x4d, 0xe9, 0x01};
    for (auto i = 0; i < sizeof(expected); i += 1) {
        ASSERT_EQ(TWDataBytes(actual.get())[i], expected[i]);
    }
}
