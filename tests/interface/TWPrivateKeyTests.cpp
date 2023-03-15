// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

TEST(TWPrivateKeyTests, GetSharedKey) {
    const auto privateKeyHex = "9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0";
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privateKeyHex).get()));
    ASSERT_TRUE(privateKey.get() != nullptr);

    const auto publicKeyHex = "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992";
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyHex).get(), TWPublicKeyTypeSECP256k1));
    EXPECT_TRUE(publicKey != nullptr);

    const auto derivedData = WRAPD(TWPrivateKeyGetSharedKey(privateKey.get(), publicKey.get(), TWCurveSECP256k1));
    ASSERT_EQ(TW::hex(*((TW::Data*)derivedData.get())), "ef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a");
}

/**
 * Valid test vector from Wycherproof project
 * Source: https://github.com/google/wycheproof/blob/master/testvectors/ecdh_secp256k1_test.json#L31
 */
TEST(TWPrivateKeyTests, GetSharedKeyWycherproof) {
    // Stripped left-padded zeroes from: `00f4b7ff7cccc98813a69fae3df222bfe3f4e28f764bf91b4a10d8096ce446b254`
    const auto privateKeyHex = "f4b7ff7cccc98813a69fae3df222bfe3f4e28f764bf91b4a10d8096ce446b254";
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privateKeyHex).get()));
    ASSERT_TRUE(privateKey.get() != nullptr);

    // Decoded from ASN.1 & uncompressed `3056301006072a8648ce3d020106052b8104000a03420004d8096af8a11e0b80037e1ee68246b5dcbb0aeb1cf1244fd767db80f3fa27da2b396812ea1686e7472e9692eaf3e958e50e9500d3b4c77243db1f2acd67ba9cc4`
    const auto publicKeyHex = "02d8096af8a11e0b80037e1ee68246b5dcbb0aeb1cf1244fd767db80f3fa27da2b";
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyHex).get(), TWPublicKeyTypeSECP256k1));
    EXPECT_TRUE(publicKey != nullptr);

    // SHA-256 of encoded x-coordinate `02544dfae22af6af939042b1d85b71a1e49e9a5614123c4d6ad0c8af65baf87d65`
    const auto derivedData = WRAPD(TWPrivateKeyGetSharedKey(privateKey.get(), publicKey.get(), TWCurveSECP256k1));
    ASSERT_EQ(TW::hex(*((TW::Data*)derivedData.get())), "81165066322732362ca5d3f0991d7f1f7d0aad7ea533276496785d369e35159a");
}

TEST(TWPrivateKeyTests, GetSharedKeyBidirectional) {
    const auto privateKeyHex1 = "9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0";
    const auto privateKey1 = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privateKeyHex1).get()));
    ASSERT_TRUE(privateKey1.get() != nullptr);
    auto publicKey1 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey1.get(), true));

    const auto privateKeyHex2 = "ef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a";
    const auto privateKey2 = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privateKeyHex2).get()));
    ASSERT_TRUE(privateKey2.get() != nullptr);
    auto publicKey2 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey2.get(), true));

    const auto derivedData1 = WRAPD(TWPrivateKeyGetSharedKey(privateKey1.get(), publicKey2.get(), TWCurveSECP256k1));
    const auto derivedData2 = WRAPD(TWPrivateKeyGetSharedKey(privateKey2.get(), publicKey1.get(), TWCurveSECP256k1));
    ASSERT_EQ(TW::hex(*((TW::Data*)derivedData1.get())), TW::hex(*((TW::Data*)derivedData2.get())));
}

TEST(TWPrivateKeyTests, GetSharedKeyError) {
    const auto privateKeyHex = "9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0";
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privateKeyHex).get()));
    ASSERT_TRUE(privateKey.get() != nullptr);

    const auto publicKeyHex = "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992";
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyHex).get(), TWPublicKeyTypeSECP256k1));
    EXPECT_TRUE(publicKey != nullptr);

    const auto derivedData = WRAPD(TWPrivateKeyGetSharedKey(privateKey.get(), publicKey.get(), TWCurveED25519));
    EXPECT_TRUE(derivedData == nullptr);
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
