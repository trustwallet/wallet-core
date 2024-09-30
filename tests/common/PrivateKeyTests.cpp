// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include <TrezorCrypto/rand.h>
#include "PublicKey.h"
#include "PublicKeyLegacy.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace std;

namespace TW::tests {

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

string TestInvalidExtended(const Data& data, const Data& ext, const Data& chainCode, const Data& data2, const Data& ext2, const Data& chainCode2) {
    try {
        auto privateKey = PrivateKey(data, ext, chainCode, data2, ext2, chainCode2);
        return hex(privateKey.bytes);
    } catch (invalid_argument& ex) {
        // expected exception
        return string("EXCEPTION: ") + string(ex.what());
    }
}

TEST(PrivateKey, CreateExtendedInvalid) {
    {
        string res = TestInvalidExtended(
            parse_hex("deadbeed"),
            parse_hex("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"),
            parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"));
        EXPECT_EQ("EXCEPTION: Invalid private key or extended key data", res);
    }
    {
        string res = TestInvalidExtended(
            parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"),
            parse_hex("deadbeed"),
            parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"));
        EXPECT_EQ("EXCEPTION: Invalid private key or extended key data", res);
    }
    {
        string res = TestInvalidExtended(
            parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"),
            parse_hex("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"),
            parse_hex("deadbeed"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"));
        EXPECT_EQ("EXCEPTION: Invalid private key or extended key data", res);
    }
    {
        string res = TestInvalidExtended(
            parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"),
            parse_hex("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"),
            parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"),
            parse_hex("deadbeed"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"),
            parse_hex("1111111111111111111111111111111111111111111111111111111111111111"));
        EXPECT_EQ("EXCEPTION: Invalid private key or extended key data", res);
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
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
        EXPECT_EQ(
            "4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867",
            hex(publicKey.bytes));
    }
    {
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(
            "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1",
            hex(publicKey.bytes));
    }
    {
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        EXPECT_EQ(
            "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91",
            hex(publicKey.bytes));
    }
    {
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeNIST256p1Extended);
        EXPECT_EQ(
            "046d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab918b4fe46ccbf56701fb210d67d91c5779468f6b3fdc7a63692b9b62543f47ae",
            hex(publicKey.bytes));
    }
}

TEST(PrivateKey, Cleanup) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = new PrivateKey(privKeyData);
    auto ptr = privateKey->bytes.data();
    ASSERT_EQ(hex(privKeyData), hex(data(ptr, 32)));

    privateKey->cleanup();

    // Memory cleaned (filled with 0s).  They may be overwritten by something else; we check that it is not equal to original, most of it has changed.
    ASSERT_EQ(hex(data(ptr, 32)), "0000000000000000000000000000000000000000000000000000000000000000");

    delete privateKey;

    // Note: it would be good to check the memory area after deletion of the object, but this is not possible
}

TEST(PrivateKey, GetType) {
    EXPECT_EQ(PrivateKey::getType(TWCurveSECP256k1), TWPrivateKeyTypeDefault);
    EXPECT_EQ(PrivateKey::getType(TWCurveNIST256p1), TWPrivateKeyTypeDefault);
    EXPECT_EQ(PrivateKey::getType(TWCurveED25519), TWPrivateKeyTypeDefault);
    EXPECT_EQ(PrivateKey::getType(TWCurveCurve25519), TWPrivateKeyTypeDefault);

    EXPECT_EQ(PrivateKey::getType(TWCurveED25519ExtendedCardano), TWPrivateKeyTypeCardano);
}

TEST(PrivateKey, PrivateKeyExtended) {
    // Non-extended: both keys are 32 bytes.
    auto privateKeyNonext = PrivateKey(parse_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    EXPECT_EQ("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5", hex(privateKeyNonext.bytes));
    auto publicKeyNonext = privateKeyNonext.getPublicKey(TWPublicKeyTypeED25519);
    EXPECT_EQ(32ul, publicKeyNonext.bytes.size());

    const auto fullkey =
        "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"
        "309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"
        "bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"
        "639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05"
        "d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7b"
        "ed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a";
    // Extended keys: private key is 2x3x32 bytes, public key is 2x64 bytes
    auto privateKeyExt = PrivateKey(parse_hex(fullkey));
    EXPECT_EQ(fullkey, hex(privateKeyExt.bytes));
    EXPECT_EQ("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744", hex(privateKeyExt.key()));
    EXPECT_EQ("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff", hex(privateKeyExt.extension()));
    EXPECT_EQ("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4", hex(privateKeyExt.chainCode()));
    EXPECT_EQ("639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05", hex(privateKeyExt.secondKey()));
    EXPECT_EQ("d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7b", hex(privateKeyExt.secondExtension()));
    EXPECT_EQ("ed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a", hex(privateKeyExt.secondChainCode()));

    auto publicKeyExt = privateKeyExt.getPublicKey(TWPublicKeyTypeED25519Cardano);
    EXPECT_EQ(2 * 64ul, publicKeyExt.bytes.size());

    // Try other constructor for extended key
    auto privateKeyExtOne = PrivateKey(
        parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"),
        parse_hex("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"),
        parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"),
        parse_hex("639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05"),
        parse_hex("d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7b"),
        parse_hex("ed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a"));
    EXPECT_EQ(fullkey, hex(privateKeyExt.bytes));
}

TEST(PrivateKey, PrivateKeyExtendedError) {
    // TWPublicKeyTypeED25519Cardano pubkey with non-extended private: error
    auto privateKeyNonext = PrivateKey(parse_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    try {
        auto publicKeyError = privateKeyNonext.getPublicKey(TWPublicKeyTypeED25519Cardano);
    } catch (invalid_argument& ex) {
        // expected exception
        return;
    }
    FAIL() << "Should throw Invalid empty key extension";
}

TEST(PrivateKey, SignSECP256k1) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(privKeyData);
    Data messageData = TW::data("hello");
    Data hash = Hash::keccak256(messageData);
    Data actual = privateKey.sign(hash, TWCurveSECP256k1);

    EXPECT_EQ(
        "8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901",
        hex(actual));
}

TEST(PrivateKey, SignExtended) {
    const auto privateKeyExt = PrivateKey(parse_hex(
        "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"
        "639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7bed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a"));
    Data messageData = TW::data("hello");
    Data hash = Hash::keccak256(messageData);
    Data actual = privateKeyExt.sign(hash, TWCurveED25519ExtendedCardano);

    EXPECT_EQ(
        "375df53b6a4931dcf41e062b1c64288ed4ff3307f862d5c1b1c71964ce3b14c99422d0fdfeb2807e9900a26d491d5e8a874c24f98eec141ed694d7a433a90f08",
        hex(actual));
}

TEST(PrivateKey, SignSchnorr) {
    const auto privateKey = PrivateKey(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const Data messageData = TW::data("hello schnorr");
    const Data digest = Hash::sha256(messageData);
    const auto signature = privateKey.signZilliqa(digest);
    EXPECT_EQ(hex(signature),
              "b8118ccb99563fe014279c957b0a9d563c1666e00367e9896fe541765246964f64a53052513da4e6dc20fdaf69ef0d95b4ca51c87ad3478986cf053c2dd0b853");
}

TEST(PrivateKey, SignNIST256p1) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(privKeyData);
    Data messageData = TW::data("hello");
    Data hash = Hash::keccak256(messageData);
    Data actual = privateKey.sign(hash, TWCurveNIST256p1);

    EXPECT_EQ(
        "8859e63a0c0cc2fc7f788d7e78406157b288faa6f76f76d37c4cd1534e8d83c468f9fd6ca7dde378df594625dcde98559389569e039282275e3d87c26e36447401",
        hex(actual));
}

TEST(PrivateKey, SignNIST256p1VerifyLegacy) {
    for (auto i = 0; i < 1000; ++i) {
        Data secret(32);
        random_buffer(secret.data(), 32);

        Data msg(32);
        random_buffer(msg.data(), 32);

        PrivateKey privateKey(secret);
        auto signature = privateKey.sign(msg, TWCurveNIST256p1);

        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeNIST256p1);
        EXPECT_TRUE(TrezorCrypto::verifyNist256p1Signature(publicKey.bytes, signature, msg))
            << "Error verifying nist256p1 signature" << std::endl
            << "Private key: " << hex(secret) << std::endl
            << "Message: " << hex(msg);
    }
}

int isCanonical([[maybe_unused]] uint8_t by, [[maybe_unused]] uint8_t sig[64]) {
    return 1;
}

TEST(PrivateKey, SignCanonicalSECP256k1) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(privKeyData);
    Data messageData = TW::data("hello");
    Data hash = Hash::keccak256(messageData);
    Data actual = privateKey.sign(hash, TWCurveSECP256k1, isCanonical);

    EXPECT_EQ(
        "208720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de9",
        hex(actual));
}

TEST(PrivateKey, SignShortDigest) {
    Data privKeyData = parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    auto privateKey = PrivateKey(privKeyData);
    Data shortDigest = TW::data("12345");
    {
        Data actual = privateKey.sign(shortDigest, TWCurveSECP256k1);
        EXPECT_EQ(actual.size(), 0ul);
    }
    {
        Data actual = privateKey.sign(shortDigest, TWCurveNIST256p1);
        EXPECT_EQ(actual.size(), 0ul);
    }
    {
        Data actual = privateKey.sign(shortDigest, TWCurveSECP256k1, isCanonical);
        EXPECT_EQ(actual.size(), 0ul);
    }
}

} // namespace TW::tests
