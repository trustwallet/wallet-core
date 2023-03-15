// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include "PublicKey.h"
#include "PrivateKey.h"
#include "HexCoding.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWPublicKeyTests, Create) {
    const auto publicKeyHex = "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1";
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyHex).get(), TWPublicKeyTypeSECP256k1));
    EXPECT_TRUE(publicKey != nullptr);
    const auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));
    EXPECT_EQ(hex(*((Data*)(publicKeyData.get()))), publicKeyHex);
}

TEST(TWPublicKeyTests, CreateFromPrivateSecp256k1) {
    const PrivateKey key(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));

    EXPECT_EQ(publicKey.get()->impl.bytes.size(), TWPublicKeyCompressedSize);
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));
    EXPECT_EQ(hex(*((Data*)(publicKeyData.get()))), "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    EXPECT_EQ(*((std::string*)(WRAPS(TWPublicKeyDescription(publicKey.get())).get())), "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    EXPECT_TRUE(TWPublicKeyIsValid(publicKey.get(), TWPublicKeyTypeSECP256k1));
    EXPECT_TRUE(TWPublicKeyIsCompressed(publicKey.get()));
}

TEST(TWPublicKeyTests, CreateInvalid) {
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("deadbeef").get(), TWPublicKeyTypeSECP256k1));
    EXPECT_EQ(publicKey, nullptr);
}

TEST(TWPublicKeyTests, CompressedExtended) {
    const PrivateKey key(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    EXPECT_EQ(TWPublicKeyKeyType(publicKey.get()), TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(publicKey.get()->impl.bytes.size(), 33ul);
    EXPECT_EQ(TWPublicKeyIsCompressed(publicKey.get()), true);
    EXPECT_TRUE(TWPublicKeyIsValid(publicKey.get(), TWPublicKeyTypeSECP256k1));

    auto extended = WRAP(TWPublicKey, TWPublicKeyUncompressed(publicKey.get()));
    EXPECT_EQ(TWPublicKeyKeyType(extended.get()), TWPublicKeyTypeSECP256k1Extended);
    EXPECT_EQ(extended.get()->impl.bytes.size(), 65ul);
    EXPECT_EQ(TWPublicKeyIsCompressed(extended.get()), false);
    EXPECT_TRUE(TWPublicKeyIsValid(extended.get(), TWPublicKeyTypeSECP256k1Extended));

    auto compressed = WRAP(TWPublicKey, TWPublicKeyCompressed(extended.get()));
    //EXPECT_TRUE(compressed == publicKey.get());
    EXPECT_EQ(TWPublicKeyKeyType(compressed.get()), TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(compressed.get()->impl.bytes.size(), 33ul);
    EXPECT_EQ(TWPublicKeyIsCompressed(compressed.get()), true);
    EXPECT_TRUE(TWPublicKeyIsValid(compressed.get(), TWPublicKeyTypeSECP256k1));
}

TEST(TWPublicKeyTests, Verify) {
    const PrivateKey key(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });

    const char* message = "Hello";
    auto messageData = WRAPD(TWDataCreateWithBytes((const uint8_t*)message, strlen(message)));
    auto digest = WRAPD(TWHashKeccak256(messageData.get()));

    auto signature = WRAPD(TWPrivateKeySign(privateKey.get(), digest.get(), TWCurveSECP256k1));

    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), false));
    ASSERT_TRUE(TWPublicKeyVerify(publicKey.get(), signature.get(), digest.get()));
}

TEST(TWPublicKeyTests, VerifyAsDER) {
    const PrivateKey key = PrivateKey(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });

    const char* message = "Hello";
    auto messageData = WRAPD(TWDataCreateWithBytes((const uint8_t*)message, strlen(message)));
    auto digest = WRAPD(TWHashKeccak256(messageData.get()));

    auto signature = WRAPD(TWPrivateKeySignAsDER(privateKey.get(), digest.get()));

    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), false));

    ASSERT_TRUE(TWPublicKeyVerifyAsDER(publicKey.get(), signature.get(), digest.get()));

    ASSERT_FALSE(TWPublicKeyVerify(publicKey.get(), signature.get(), digest.get()));
}

TEST(TWPublicKeyTests, VerifyEd25519) {
    const PrivateKey key(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });

    const char* message = "Hello";
    auto messageData = WRAPD(TWDataCreateWithBytes((const uint8_t*)message, strlen(message)));
    auto digest = WRAPD(TWHashSHA256(messageData.get()));

    auto signature = WRAPD(TWPrivateKeySign(privateKey.get(), digest.get(), TWCurveED25519));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey.get()));

    auto signature2 = WRAPD(TWPrivateKeySign(privateKey.get(), digest.get(), TWCurveED25519Blake2bNano));
    auto publicKey2 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519Blake2b(privateKey.get()));

    ASSERT_TRUE(TWPublicKeyVerify(publicKey.get(), signature.get(), digest.get()));
    ASSERT_TRUE(TWPublicKeyVerify(publicKey2.get(), signature2.get(), digest.get()));
}

TEST(TWPublicKeyTests, Recover) {
    const auto message = DATA("de4e9524586d6fce45667f9ff12f661e79870c4105fa0fb58af976619bb11432");
    const auto signature = DATA("00000000000000000000000000000000000000000000000000000000000000020123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef80");
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyRecover(signature.get(), message.get()));
    EXPECT_TRUE(publicKey.get() != nullptr);
    EXPECT_EQ(TWPublicKeyKeyType(publicKey.get()), TWPublicKeyTypeSECP256k1Extended);
    const auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));
    EXPECT_EQ(hex(*((Data*)(publicKeyData.get()))), 
        "0456d8089137b1fd0d890f8c7d4a04d0fd4520a30b19518ee87bd168ea12ed8090329274c4c6c0d9df04515776f2741eeffc30235d596065d718c3973e19711ad0");
}

TEST(TWPublicKeyTests, RecoverInvalid) {
    const auto deadbeef = DATA("deadbeef");
    const auto publicKey = WRAP(TWPublicKey, TWPublicKeyRecover(deadbeef.get(), deadbeef.get()));
    EXPECT_EQ(publicKey.get(), nullptr);
}
