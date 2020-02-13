// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "PublicKey.h"
#include "PrivateKey.h"
#include "HexCoding.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWPublicKeyTests, CreateFromPrivateSecp256k1) {
    const PrivateKey key(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);

    EXPECT_EQ(publicKey->impl.bytes.size(), TWPublicKeyCompressedSize);
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey));
    EXPECT_EQ(hex(*((Data*)(publicKeyData.get()))), "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    EXPECT_EQ(*((std::string*)(WRAPS(TWPublicKeyDescription(publicKey)).get())), "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    EXPECT_TRUE(TWPublicKeyIsValid(publicKey, TWPublicKeyTypeSECP256k1));
    EXPECT_TRUE(TWPublicKeyIsCompressed(publicKey));

    TWPublicKeyDelete(publicKey);
}

TEST(TWPublicKeyTests, CompressedExtended) {
    const PrivateKey key(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    EXPECT_EQ(TWPublicKeyKeyType(publicKey), TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(publicKey->impl.bytes.size(), 33);
    EXPECT_EQ(TWPublicKeyIsCompressed(publicKey), true);
    EXPECT_TRUE(TWPublicKeyIsValid(publicKey, TWPublicKeyTypeSECP256k1));

    auto extended = TWPublicKeyUncompressed(publicKey);
    EXPECT_EQ(TWPublicKeyKeyType(extended), TWPublicKeyTypeSECP256k1Extended);
    EXPECT_EQ(extended->impl.bytes.size(), 65);
    EXPECT_EQ(TWPublicKeyIsCompressed(extended), false);
    EXPECT_TRUE(TWPublicKeyIsValid(extended, TWPublicKeyTypeSECP256k1Extended));

    auto compressed = TWPublicKeyCompressed(extended);
    //EXPECT_TRUE(compressed == publicKey);
    EXPECT_EQ(TWPublicKeyKeyType(compressed), TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(compressed->impl.bytes.size(), 33);
    EXPECT_EQ(TWPublicKeyIsCompressed(compressed), true);
    EXPECT_TRUE(TWPublicKeyIsValid(compressed, TWPublicKeyTypeSECP256k1));
}

TEST(TWPublicKeyTests, Verify) {
    const PrivateKey key(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });

    const char* message = "Hello";
    auto messageData = WRAPD(TWDataCreateWithBytes((const uint8_t*)message, strlen(message)));
    auto digest = WRAPD(TWHashKeccak256(messageData.get()));

    auto signature = WRAPD(TWPrivateKeySign(privateKey.get(), digest.get(), TWCurveSECP256k1));

    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), false);
    ASSERT_TRUE(TWPublicKeyVerify(publicKey, signature.get(), digest.get()));
}

TEST(TWPublicKeyTests, VerifyEd25519) {
    const PrivateKey key(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto privateKey = WRAP(TWPrivateKey, new TWPrivateKey{ key });

    const char* message = "Hello";
    auto messageData = WRAPD(TWDataCreateWithBytes((const uint8_t*)message, strlen(message)));
    auto digest = WRAPD(TWHashSHA256(messageData.get()));

    auto signature = WRAPD(TWPrivateKeySign(privateKey.get(), digest.get(), TWCurveED25519));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(privateKey.get());

    auto signature2 = WRAPD(TWPrivateKeySign(privateKey.get(), digest.get(), TWCurveED25519Blake2bNano));
    auto publicKey2 = TWPrivateKeyGetPublicKeyEd25519Blake2b(privateKey.get());

    ASSERT_TRUE(TWPublicKeyVerify(publicKey, signature.get(), digest.get()));
    ASSERT_TRUE(TWPublicKeyVerify(publicKey2, signature2.get(), digest.get()));
}
