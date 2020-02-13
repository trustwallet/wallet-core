// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PublicKey.h"

#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "interface/TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(PublicKeyTests, CreateFromPrivateSecp256k1) {
    const Data key = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(key);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(publicKey.bytes.size(), 33);
    EXPECT_EQ(hex(publicKey.bytes), "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    EXPECT_EQ(publicKey.isCompressed(), true);
    EXPECT_TRUE(PublicKey::isValid(publicKey.bytes, TWPublicKeyTypeSECP256k1));
}

TEST(PublicKeyTests, CreateFromDataSecp256k1) {
    const Data key = parse_hex("0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
    PublicKey publicKey(key, TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(hex(publicKey.bytes), hex(key));
}

TEST(PublicKeyTests, CreateInvalid) {
    const Data keyInvalid = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32af"); // too short
    try {
        PublicKey publicKey(keyInvalid, TWPublicKeyTypeSECP256k1);
    } catch (const std::invalid_argument&) {
        return; // OK
    }
    FAIL() << "Missing expected exception";
}

TEST(PublicKeyTests, CompressedExtended) {
    const Data key = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(key);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(publicKey.type, TWPublicKeyTypeSECP256k1);
    EXPECT_EQ(publicKey.bytes.size(), 33);
    EXPECT_EQ(publicKey.isCompressed(), true);
    EXPECT_TRUE(PublicKey::isValid(publicKey.bytes, TWPublicKeyTypeSECP256k1));
    EXPECT_EQ(hex(publicKey.bytes), std::string("0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1"));

    auto extended = publicKey.extended();
    EXPECT_EQ(extended.type, TWPublicKeyTypeSECP256k1Extended);
    EXPECT_EQ(extended.bytes.size(), 65);
    EXPECT_EQ(extended.isCompressed(), false);
    EXPECT_TRUE(PublicKey::isValid(extended.bytes, TWPublicKeyTypeSECP256k1Extended));
    EXPECT_EQ(hex(extended.bytes), std::string("0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91"));

    auto compressed = extended.compressed();
    EXPECT_EQ(compressed.type, TWPublicKeyTypeSECP256k1);
    EXPECT_TRUE(compressed == publicKey);
    EXPECT_EQ(compressed.bytes.size(), 33);
    EXPECT_EQ(compressed.isCompressed(), true);
    EXPECT_TRUE(PublicKey::isValid(compressed.bytes, TWPublicKeyTypeSECP256k1));
    EXPECT_EQ(hex(compressed.bytes), std::string("0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1"));
}

TEST(PublicKeyTests, Verify) {
    const Data key = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(key);

    const char* message = "Hello";
    const Data messageData = TW::data(message);
    const Data digest = Hash::keccak256(messageData);

    auto signature = privateKey.sign(digest, TWCurveSECP256k1);

    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    EXPECT_TRUE(publicKey.verify(signature, digest));
}

TEST(PublicKeyTests, VerifyEd25519) {
    const auto key = PrivateKey(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    auto privateKey = PrivateKey(key);

    const char* message = "Hello";
    const Data messageData = TW::data(message);
    const Data digest = Hash::sha256(messageData);

    auto signature = privateKey.sign(digest, TWCurveED25519);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);

    auto signature2 = privateKey.sign(digest, TWCurveED25519Blake2bNano);
    auto publicKey2 = privateKey.getPublicKey(TWPublicKeyTypeED25519Blake2b);

    EXPECT_TRUE(publicKey.verify(signature, digest));
    EXPECT_TRUE(publicKey2.verify(signature2, digest));
}
