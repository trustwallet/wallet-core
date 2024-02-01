// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <gtest/gtest.h>

const auto key1Hex = "22667b69166481c9f334756f49c8dddfd72c6bcdd68a7386886e97a82f741130";

TEST(TWPrivateKeyTests, Create) {
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(key1Hex).get()));
    ASSERT_TRUE(privateKey.get() != nullptr);

    const auto data = WRAPD(TWPrivateKeyData(privateKey.get()));
    EXPECT_EQ(TW::hex(TW::data(TWDataBytes(data.get()), TWDataSize(data.get()))), key1Hex);
}

TEST(TWPrivateKeyTests, CreateNewRandom) {
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreate());
    ASSERT_TRUE(privateKey.get() != nullptr);
}

TEST(TWPrivateKeyTests, CreateInvalid) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("deadbeef").get()));
    ASSERT_EQ(privateKey.get(), nullptr);
}

TEST(TWPrivateKeyTests, CreateCopy) {
    const auto privateKey1 = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(key1Hex).get()));
    ASSERT_TRUE(privateKey1.get() != nullptr);
    const auto privateKey2 = WRAP(TWPrivateKey, TWPrivateKeyCreateCopy(privateKey1.get()));
    ASSERT_TRUE(privateKey2.get() != nullptr);
}

TEST(TWPrivateKeyTests, AllZeros) {
    auto bytes = TW::Data(32);
    auto data = WRAPD(TWDataCreateWithBytes(bytes.data(), bytes.size()));
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(data.get()));

    ASSERT_EQ(privateKey.get(), nullptr);
}

TEST(TWPrivateKeyTests, Invalid) {
    auto bytes = TW::parse_hex("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
    auto valid = TW::PrivateKey::isValid(bytes, TWCurveSECP256k1);

    auto bytes2 = TW::parse_hex("0000000000000000000000000000000000000000000000000000000000000000");
    auto valid2 = TW::PrivateKey::isValid(bytes2, TWCurveSECP256k1);

    ASSERT_EQ(valid, false);
    ASSERT_EQ(valid2, false);
}

TEST(TWPrivateKeyTests, IsValid) {
    const auto data = DATA("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");

    ASSERT_TRUE(TWPrivateKeyIsValid(data.get(), TWCurveSECP256k1));
    ASSERT_TRUE(TWPrivateKeyIsValid(data.get(), TWCurveED25519));
}

TEST(TWPrivateKeyTests, PublicKey) {
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5").get()));
    {
        const auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), false));
        ASSERT_EQ(TW::hex(publicKey.get()->impl.bytes), "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91");
    }
    {
        const auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyNist256p1(privateKey.get()));
        ASSERT_EQ(TW::hex(publicKey.get()->impl.bytes), "026d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab");
    }
    {
        const auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyCurve25519(privateKey.get()));
        ASSERT_EQ(TW::hex(publicKey.get()->impl.bytes), "686cfce9108566dd43fc6aa75e31f9a9f319c9e9c04d6ad0a52505b86bc17c3a");
    }
    {
        const auto publicKey =  WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeEthereum));
        ASSERT_EQ(TW::hex(publicKey.get()->impl.bytes), "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91");

        auto pubkeyType = TWCoinTypePublicKeyType(TWCoinTypeEthereum);
        const auto publicKeyByType =  WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyByType(privateKey.get(), pubkeyType));

        ASSERT_EQ(TW::hex(publicKey.get()->impl.bytes), TW::hex(publicKeyByType.get()->impl.bytes));
    }
    {
        const auto publicKey =  WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeNEO));
        ASSERT_EQ(TW::hex(publicKey.get()->impl.bytes), "026d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab");
    }
    {
        const auto publicKey =  WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeWaves));
        ASSERT_EQ(TW::hex(publicKey.get()->impl.bytes), "686cfce9108566dd43fc6aa75e31f9a9f319c9e9c04d6ad0a52505b86bc17c3a");
    }
}

TEST(TWPrivateKeyTests, Sign) {
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5").get()));

    const auto message = "hello";
    const auto data = WRAPD(TWDataCreateWithBytes((uint8_t *)message, strlen(message)));
    const auto hash = WRAPD(TWHashKeccak256(data.get()));

    const auto actual = WRAPD(TWPrivateKeySign(privateKey.get(), hash.get(), TWCurveSECP256k1));

    ASSERT_EQ(TW::hex(*((TW::Data*)actual.get())),
        "8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901");
}

TEST(TWPrivateKeyTests, SignAsDER) {
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5").get()));

    const auto message = "hello";
    const auto data = WRAPD(TWDataCreateWithBytes((uint8_t *)message, strlen(message)));
    const auto hash = WRAPD(TWHashKeccak256(data.get()));

    auto actual = WRAPD(TWPrivateKeySignAsDER(privateKey.get(), hash.get()));

    ASSERT_EQ(TW::hex(*((TW::Data*)actual.get())),
        "30450221008720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba6202204d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de9");
}
