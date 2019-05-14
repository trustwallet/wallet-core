// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>

#include <gtest/gtest.h>

TEST(BitcoinScript, ScriptHash) {
    auto pkData = DATA("cf5007e19af3641199f21f3fa54dff2fa2627471");
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToPublicKeyHash(pkData.get()));

    auto data = WRAPD(TWBitcoinScriptData(script.get()));

    auto scriptHex = WRAPS(TWStringCreateWithHexData(data.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(scriptHex.get()), "76a914cf5007e19af3641199f21f3fa54dff2fa262747188ac");

    auto scriptHash = WRAPD(TWBitcoinScriptScriptHash(script.get()));

    auto hexData = WRAPS(TWStringCreateWithHexData(scriptHash.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexData.get()), "c470d22e69a2a967f2cec0cd5a5aebb955cdd395");
}

TEST(BitcoinScript, RedeemScript) {
    auto pkData = DATA("cf5007e19af3641199f21f3fa54dff2fa2627471");
    auto embeddedScript = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToPublicKeyHash(pkData.get()));

    auto scriptHash = WRAPD(TWBitcoinScriptScriptHash(embeddedScript.get()));

    auto scriptPub1 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToScriptHash(scriptHash.get()));

    auto data = WRAPD(TWBitcoinScriptData(scriptPub1.get()));

    auto hexData = WRAPS(TWStringCreateWithHexData(data.get()));
    ASSERT_STREQ(TWStringUTF8Bytes(hexData.get()), "a914c470d22e69a2a967f2cec0cd5a5aebb955cdd39587");
}

TEST(BitcoinScript, LockScriptForP2PKHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("1Cu32FVupVCgHkMMRJdYJugxwo2Aprgk7H").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a9148280b37df378db99f66f85c95a783a76ac7a6d5988ac");

    auto scriptPub2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("16TZ8J6Q5iZKBWizWzFAYnrsaox5Z5aBRV").get()));
    auto scriptPub2Data = WRAPD(TWBitcoinScriptData(scriptPub2.get()));
    assertHexEqual(scriptPub2Data, "76a9143bde42dbee7e4dbe6a21b2d50ce2f0167faa815988ac");
}

TEST(BitcoinScript, LockScriptForP2SHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("37rHiL4DN2wkt8pgCAUfYJRxhir98ZGN1y").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a9144391adbec172cad6a9fc3eebca36aeec6640abda87");

    auto scriptPub2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("3HV63hgTNAgdiEp4FbJRPSVrjaV4ZoX4Bs").get()));
    auto scriptPub2Data = WRAPD(TWBitcoinScriptData(scriptPub2.get()));
    assertHexEqual(scriptPub2Data, "a914ad40768af6419a20bdb94d83c06b6c8c94721dc087");
}

TEST(BitcoinScript, LockScriptForP2WPKHAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("bc1q6hppaw7uld68amnnu5vpp5dd5u7k92c2vtdtkq").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "0014d5c21ebbdcfb747eee73e51810d1ada73d62ab0a");

    auto scriptPub2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("bc1qqw0jllft9pcr7r5uw0x08njkft0thd0g5yus0x").get()));
    auto scriptPub2Data = WRAPD(TWBitcoinScriptData(scriptPub2.get()));
    assertHexEqual(scriptPub2Data, "0014039f2ffd2b28703f0e9c73ccf3ce564adebbb5e8");
}

TEST(BitcoinScript, LockScriptForCashAddress) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("bitcoincash:pzclklsyx9f068hd00a0vene45akeyrg7vv0053uqf").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a914b1fb7e043152fd1eed7bfaf66679ad3b6c9068f387");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("bitcoincash:qpk05r5kcd8uuzwqunn8rlx5xvuvzjqju5rch3tc0u").get()));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a9146cfa0e96c34fce09c0e4e671fcd43338c14812e588ac");
}
