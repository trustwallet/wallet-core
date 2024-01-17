// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>

#include <gtest/gtest.h>

namespace TW::Bitcoin::TWScriptTests {

// clang-format off
const auto PayToScriptHash = WRAP(TWBitcoinScript, TWBitcoinScriptCreateWithData(DATA("a914" "4733f37cf4db86fbc2efed2500b4f4e49f312023" "87").get()));
const auto PayToWitnessScriptHash = WRAP(TWBitcoinScript, TWBitcoinScriptCreateWithData(DATA("0020" "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db").get()));
const auto PayToWitnessPublicKeyHash = WRAP(TWBitcoinScript, TWBitcoinScriptCreateWithData(DATA("0014" "79091972186c449eb1ded22b78e40d009bdf0089").get()));
const auto PayToPublicKeySecp256k1 = WRAP(TWBitcoinScript, TWBitcoinScriptCreateWithData(DATA("21" "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432" "ac").get()));
const auto PayToPublicKeyHash = WRAP(TWBitcoinScript, TWBitcoinScriptCreateWithData(DATA("76a914" "79091972186c449eb1ded22b78e40d009bdf0089" "88ac").get()));
// clang-format on

TEST(TWBitcoinScript, Create) {
    auto data = DATA("a9144733f37cf4db86fbc2efed2500b4f4e49f31202387");
    {
        auto script = WRAP(TWBitcoinScript, TWBitcoinScriptCreate());
        ASSERT_TRUE(script.get() != nullptr);
    }
    {
        auto script = WRAP(TWBitcoinScript, TWBitcoinScriptCreateWithData(data.get()));
        ASSERT_TRUE(script.get() != nullptr);
        ASSERT_EQ(TWBitcoinScriptSize(script.get()), 23ul);
    }
    {
        auto script = WRAP(TWBitcoinScript, TWBitcoinScriptCreateWithBytes(TWDataBytes(data.get()), TWDataSize(data.get())));
        ASSERT_TRUE(script.get() != nullptr);
        ASSERT_EQ(TWBitcoinScriptSize(script.get()), 23ul);

        auto scriptCopy = WRAP(TWBitcoinScript, TWBitcoinScriptCreateCopy(script.get()));
        ASSERT_TRUE(scriptCopy.get() != nullptr);
        ASSERT_EQ(TWBitcoinScriptSize(scriptCopy.get()), 23ul);
    }
}

TEST(TWBitcoinScript, Equals) {
    auto data = DATA("a9144733f37cf4db86fbc2efed2500b4f4e49f31202387");
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptCreateWithBytes(TWDataBytes(data.get()), TWDataSize(data.get())));
    auto scriptCopy = WRAP(TWBitcoinScript, TWBitcoinScriptCreateCopy(script.get()));
    ASSERT_TRUE(TWBitcoinScriptEqual(script.get(), scriptCopy.get()));
}

TEST(TWBitcoinScript, IsPayToScriptHash) {
    ASSERT_TRUE(TWBitcoinScriptIsPayToScriptHash(PayToScriptHash.get()));
}

TEST(TWBitcoinScript, IsPayToWitnessScriptHash) {
    ASSERT_TRUE(TWBitcoinScriptIsPayToWitnessScriptHash(PayToWitnessScriptHash.get()));
}

TEST(TWBitcoinScript, IsPayToWitnessPublicKeyHash) {
    ASSERT_TRUE(TWBitcoinScriptIsPayToWitnessPublicKeyHash(PayToWitnessPublicKeyHash.get()));
}

TEST(TWBitcoinScript, IsWitnessProgram) {
    ASSERT_TRUE(TWBitcoinScriptIsWitnessProgram(PayToWitnessScriptHash.get()));
    ASSERT_TRUE(TWBitcoinScriptIsWitnessProgram(PayToWitnessPublicKeyHash.get()));
    ASSERT_FALSE(TWBitcoinScriptIsWitnessProgram(PayToScriptHash.get()));
}

TEST(TWBitcoinScript, MatchPayToPubkey) {
    const auto res = WRAPD(TWBitcoinScriptMatchPayToPubkey(PayToPublicKeySecp256k1.get()));
    ASSERT_TRUE(res.get() != nullptr);
    const auto hexRes = WRAPS(TWStringCreateWithHexData(res.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexRes.get()), "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");

    ASSERT_EQ(TWBitcoinScriptMatchPayToPubkey(PayToScriptHash.get()), nullptr);
}

TEST(TWBitcoinScript, MatchPayToPubkeyHash) {
    const auto res = WRAPD(TWBitcoinScriptMatchPayToPubkeyHash(PayToPublicKeyHash.get()));
    ASSERT_TRUE(res.get() != nullptr);
    const auto hexRes = WRAPS(TWStringCreateWithHexData(res.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexRes.get()), "79091972186c449eb1ded22b78e40d009bdf0089");

    ASSERT_EQ(TWBitcoinScriptMatchPayToPubkeyHash(PayToScriptHash.get()), nullptr);
}

TEST(TWBitcoinScript, MatchPayToScriptHash) {
    const auto res = WRAPD(TWBitcoinScriptMatchPayToScriptHash(PayToScriptHash.get()));
    ASSERT_TRUE(res.get() != nullptr);
    const auto hexRes = WRAPS(TWStringCreateWithHexData(res.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexRes.get()), "4733f37cf4db86fbc2efed2500b4f4e49f312023");

    ASSERT_EQ(TWBitcoinScriptMatchPayToScriptHash(PayToPublicKeySecp256k1.get()), nullptr);
}

TEST(TWBitcoinScript, MatchPayToWitnessPublicKeyHash) {
    const auto res = WRAPD(TWBitcoinScriptMatchPayToWitnessPublicKeyHash(PayToWitnessPublicKeyHash.get()));
    ASSERT_TRUE(res.get() != nullptr);
    const auto hexRes = WRAPS(TWStringCreateWithHexData(res.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexRes.get()), "79091972186c449eb1ded22b78e40d009bdf0089");

    ASSERT_EQ(TWBitcoinScriptMatchPayToWitnessPublicKeyHash(PayToPublicKeySecp256k1.get()), nullptr);
}

TEST(TWBitcoinScript, MatchPayToWitnessScriptHash) {
    const auto res = WRAPD(TWBitcoinScriptMatchPayToWitnessScriptHash(PayToWitnessScriptHash.get()));
    ASSERT_TRUE(res.get() != nullptr);
    const auto hexRes = WRAPS(TWStringCreateWithHexData(res.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexRes.get()), "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db");

    ASSERT_EQ(TWBitcoinScriptMatchPayToWitnessScriptHash(PayToPublicKeySecp256k1.get()), nullptr);
}

TEST(TWBitcoinScript, Encode) {
    const auto res = WRAPD(TWBitcoinScriptEncode(PayToScriptHash.get()));
    ASSERT_TRUE(res.get() != nullptr);
    const auto hexRes = WRAPS(TWStringCreateWithHexData(res.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexRes.get()), "17a9144733f37cf4db86fbc2efed2500b4f4e49f31202387");
}

TEST(TWBitcoinScript, BuildPayToPublicKey) {
    const auto pubkey = DATA("03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432");
    const auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToPublicKey(pubkey.get()));
    ASSERT_TRUE(script.get() != nullptr);
    const auto hex = WRAPS(TWStringCreateWithHexData(WRAPD(TWBitcoinScriptData(script.get())).get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hex.get()), "21"
                                               "03c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432"
                                               "ac");
}

TEST(TWBitcoinScript, BuildPayToWitnessPubkeyHash) {
    const auto hash = DATA("79091972186c449eb1ded22b78e40d009bdf0089");
    const auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToWitnessPubkeyHash(hash.get()));
    ASSERT_TRUE(script.get() != nullptr);
    const auto hex = WRAPS(TWStringCreateWithHexData(WRAPD(TWBitcoinScriptData(script.get())).get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hex.get()), "0014"
                                               "79091972186c449eb1ded22b78e40d009bdf0089");
}

TEST(TWBitcoinScript, BuildPayToWitnessScriptHash) {
    const auto hash = DATA("ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db");
    const auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToWitnessScriptHash(hash.get()));
    ASSERT_TRUE(script.get() != nullptr);
    const auto hex = WRAPS(TWStringCreateWithHexData(WRAPD(TWBitcoinScriptData(script.get())).get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hex.get()), "0020"
                                               "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db");
}

TEST(TWBitcoinScript, ScriptHash) {
    auto pkData = DATA("cf5007e19af3641199f21f3fa54dff2fa2627471");
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToPublicKeyHash(pkData.get()));

    auto data = WRAPD(TWBitcoinScriptData(script.get()));

    auto scriptHex = WRAPS(TWStringCreateWithHexData(data.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(scriptHex.get()), "76a914cf5007e19af3641199f21f3fa54dff2fa262747188ac");

    auto scriptHash = WRAPD(TWBitcoinScriptScriptHash(script.get()));

    auto hexData = WRAPS(TWStringCreateWithHexData(scriptHash.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexData.get()), "c470d22e69a2a967f2cec0cd5a5aebb955cdd395");
}

TEST(TWBitcoinScript, RedeemScript) {
    auto pkData = DATA("cf5007e19af3641199f21f3fa54dff2fa2627471");
    auto embeddedScript = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToPublicKeyHash(pkData.get()));

    auto scriptHash = WRAPD(TWBitcoinScriptScriptHash(embeddedScript.get()));

    auto scriptPub1 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToScriptHash(scriptHash.get()));

    auto data = WRAPD(TWBitcoinScriptData(scriptPub1.get()));

    auto hexData = WRAPS(TWStringCreateWithHexData(data.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexData.get()), "a914c470d22e69a2a967f2cec0cd5a5aebb955cdd39587");
}

TEST(TWBitcoinScript, LockScriptForP2PKHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("1Cu32FVupVCgHkMMRJdYJugxwo2Aprgk7H").get(), TWCoinTypeBitcoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a9148280b37df378db99f66f85c95a783a76ac7a6d5988ac");

    auto scriptPub2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("16TZ8J6Q5iZKBWizWzFAYnrsaox5Z5aBRV").get(), TWCoinTypeBitcoin));
    auto scriptPub2Data = WRAPD(TWBitcoinScriptData(scriptPub2.get()));
    assertHexEqual(scriptPub2Data, "76a9143bde42dbee7e4dbe6a21b2d50ce2f0167faa815988ac");
}

TEST(TWBitcoinScript, LockScriptForP2SHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("37rHiL4DN2wkt8pgCAUfYJRxhir98ZGN1y").get(), TWCoinTypeBitcoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a9144391adbec172cad6a9fc3eebca36aeec6640abda87");

    auto scriptPub2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("3HV63hgTNAgdiEp4FbJRPSVrjaV4ZoX4Bs").get(), TWCoinTypeBitcoin));
    auto scriptPub2Data = WRAPD(TWBitcoinScriptData(scriptPub2.get()));
    assertHexEqual(scriptPub2Data, "a914ad40768af6419a20bdb94d83c06b6c8c94721dc087");
}

TEST(TWBitcoinScript, LockScriptForP2WPKHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("bc1q6hppaw7uld68amnnu5vpp5dd5u7k92c2vtdtkq").get(), TWCoinTypeBitcoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "0014d5c21ebbdcfb747eee73e51810d1ada73d62ab0a");

    auto scriptPub2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("bc1qqw0jllft9pcr7r5uw0x08njkft0thd0g5yus0x").get(), TWCoinTypeBitcoin));
    auto scriptPub2Data = WRAPD(TWBitcoinScriptData(scriptPub2.get()));
    assertHexEqual(scriptPub2Data, "0014039f2ffd2b28703f0e9c73ccf3ce564adebbb5e8");
}

TEST(TWBitcoinScript, LockScriptForP2WSHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("bc1qcuqamesrt803xld4l2j2vxx8rxmrx7aq82mkw7xwxh643wzqjlnqutkcv2").get(), TWCoinTypeBitcoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "0020c701dde60359df137db5faa4a618c719b6337ba03ab76778ce35f558b84097e6");
}

TEST(TWBitcoinScript, LockScriptForCashAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("bitcoincash:pzclklsyx9f068hd00a0vene45akeyrg7vv0053uqf").get(), TWCoinTypeBitcoinCash));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a914b1fb7e043152fd1eed7bfaf66679ad3b6c9068f387");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("bitcoincash:qpk05r5kcd8uuzwqunn8rlx5xvuvzjqju5rch3tc0u").get(), TWCoinTypeBitcoinCash));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a9146cfa0e96c34fce09c0e4e671fcd43338c14812e588ac");
}

TEST(TWBitcoinSigHashType, HashTypeForCoin) {
    EXPECT_EQ(TWBitcoinScriptHashTypeForCoin(TWCoinTypeBitcoin), (uint32_t)TWBitcoinSigHashTypeAll);
    EXPECT_EQ(TWBitcoinScriptHashTypeForCoin(TWCoinTypeLitecoin), (uint32_t)TWBitcoinSigHashTypeAll);
    EXPECT_EQ(TWBitcoinScriptHashTypeForCoin(TWCoinTypeZcash), (uint32_t)TWBitcoinSigHashTypeAll);
    EXPECT_EQ(TWBitcoinScriptHashTypeForCoin(TWCoinTypeBitcoinCash), (uint32_t)TWBitcoinSigHashTypeAll | (uint32_t)TWBitcoinSigHashTypeFork);
    EXPECT_EQ(TWBitcoinScriptHashTypeForCoin(TWCoinTypeECash), (uint32_t)TWBitcoinSigHashTypeAll | (uint32_t)TWBitcoinSigHashTypeFork);
    EXPECT_EQ(TWBitcoinScriptHashTypeForCoin(TWCoinTypeBitcoinGold), (uint32_t)TWBitcoinSigHashTypeAll | (uint32_t)TWBitcoinSigHashTypeForkBTG);
}

TEST(TWBitcoinSigHashType, IsSingle) {
    EXPECT_TRUE(TWBitcoinSigHashTypeIsSingle(TWBitcoinSigHashTypeSingle));
    EXPECT_FALSE(TWBitcoinSigHashTypeIsSingle(TWBitcoinSigHashTypeAll));
    EXPECT_FALSE(TWBitcoinSigHashTypeIsSingle(TWBitcoinSigHashTypeNone));
    EXPECT_FALSE(TWBitcoinSigHashTypeIsSingle(TWBitcoinSigHashTypeFork));
}

TEST(TWBitcoinSigHashType, IsNone) {
    EXPECT_TRUE(TWBitcoinSigHashTypeIsNone(TWBitcoinSigHashTypeNone));
    EXPECT_FALSE(TWBitcoinSigHashTypeIsNone(TWBitcoinSigHashTypeSingle));
    EXPECT_FALSE(TWBitcoinSigHashTypeIsNone(TWBitcoinSigHashTypeAll));
    EXPECT_FALSE(TWBitcoinSigHashTypeIsNone(TWBitcoinSigHashTypeFork));
}

} // namespace TW::Bitcoin::TWScriptTests
