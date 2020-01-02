// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"
#include "Hash.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace std;


TEST(PrivateKey, CreateValid) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    EXPECT_TRUE(PrivateKey::isValid(privKeyData, TWCurveSECP256k1));
    auto privateKey = PrivateKey(privKeyData);
    EXPECT_EQ(hex(privKeyData), hex(privateKey.bytes));
}

string TestInvalid(const Data& privKeyData) {
    try {
        auto privateKey = PrivateKey(privKeyData);
        return hex(privateKey.bytes);
    } catch (invalid_argument& ex) {
        // expected exception
        return string("EXCEPTION: ") + string(ex.what());
    }
}

TEST(PrivateKey, InvalidShort) {
    string res = TestInvalid(parse_hex("deadbeef"));
    EXPECT_EQ("EXCEPTION: Invalid private key data", res);
}

TEST(PrivateKey, InvalidAllZeros) {
    string res = TestInvalid(Data(32));
    EXPECT_EQ("EXCEPTION: Invalid private key data", res);
}

TEST(PrivateKey, InvalidSECP256k1) {
    {
        auto privKeyData = parse_hex("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
        auto valid = PrivateKey::isValid(privKeyData, TWCurveSECP256k1);
        EXPECT_EQ(valid, false);
    }
    {
        auto privKeyData = parse_hex("0000000000000000000000000000000000000000000000000000000000000000");
        auto valid = PrivateKey::isValid(privKeyData, TWCurveSECP256k1);
        EXPECT_EQ(valid, false);
    }
}

TEST(PrivateKey, Valid) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    EXPECT_TRUE(PrivateKey::isValid(privKeyData, TWCurveSECP256k1));
    EXPECT_TRUE(PrivateKey::isValid(privKeyData, TWCurveED25519));
}

TEST(PrivateKey, PublicKey) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(privKeyData);
    {
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
        EXPECT_EQ(
            "4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867",
            hex(publicKey.bytes)
        );
    }
    {
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(
            "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1",
            hex(publicKey.bytes)
        );
    }
    {
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        EXPECT_EQ(
            "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91",
            hex(publicKey.bytes)
        );
    }
}

TEST(PrivateKey, ClearMemory) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = new PrivateKey(privKeyData);
    auto ptr = privateKey->bytes.data();
    ASSERT_EQ(hex(privKeyData), hex(data(ptr, 32)));

    delete privateKey;
    privateKey = nullptr;

    // Memory cleaned (filled with 0s).  They may be overwritten by something else; we check that it is not equal to original, most of it has changed.
    ASSERT_NE(hex(privKeyData), hex(data(ptr, 32)));
    int countDifferent = 0;
    for (auto i = 0; i < privKeyData.size(); ++i) {
        countDifferent += (int)(ptr[i] != privKeyData[i]);
    }
    ASSERT_GE(countDifferent, 32*2/3);
}

TEST(PrivateKey, PrivateKeyExtended) {
    // Non-extended: both keys are 32 bytes.
    auto privateKeyNonext = PrivateKey(parse_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"
    ));
    EXPECT_EQ("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5", hex(privateKeyNonext.bytes));
    auto publicKeyNonext = privateKeyNonext.getPublicKey(TWPublicKeyTypeED25519);
    EXPECT_EQ(32, publicKeyNonext.bytes.size());

    // Extended keys: private key is 3x32 bytes, public key is 64 bytes
    auto privateKeyExt = PrivateKey(parse_hex(
        "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"
    ));
    EXPECT_EQ("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744", hex(privateKeyExt.bytes));
    EXPECT_EQ("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff", hex(privateKeyExt.extensionBytes));
    EXPECT_EQ("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4", hex(privateKeyExt.chainCodeBytes));
    auto publicKeyExt = privateKeyExt.getPublicKey(TWPublicKeyTypeED25519Extended);
    EXPECT_EQ(64, publicKeyExt.bytes.size());

    // Try other constructor for extended key
    auto privateKeyExtOne = PrivateKey(parse_hex(
        "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"
    ));
    EXPECT_EQ("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744", hex(privateKeyExtOne.bytes));
    EXPECT_EQ("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff", hex(privateKeyExtOne.extensionBytes));
    EXPECT_EQ("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4", hex(privateKeyExtOne.chainCodeBytes));
}

TEST(PrivateKey, PrivateKeyExtendedError) {
    // TWPublicKeyTypeED25519Extended pubkey with non-extended private: error
    auto privateKeyNonext = PrivateKey(parse_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"
    ));
    try {
        auto publicKeyError = privateKeyNonext.getPublicKey(TWPublicKeyTypeED25519Extended);
    } catch (invalid_argument& ex) {
        // expected exception
        return;
    }
    FAIL() << "Should throw Invalid empty key extension";
}

TEST(PrivateKey, Sign) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(privKeyData);
    Data messageData = TW::data("hello");
    Data result(32);
    Data hash = Hash::keccak256(messageData);
    Data actual = privateKey.sign(hash, TWCurveSECP256k1);

    EXPECT_EQ(
        "8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901",
        hex(actual)
    );
}
