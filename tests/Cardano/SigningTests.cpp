// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/AddressV3.h"

#include "PrivateKey.h"
#include "HexCoding.h"

#include <gtest/gtest.h>


using namespace TW::Cardano;
using namespace TW;
using namespace std;


TEST(CardanoSigning, SignMessageWithKey) {
    // from cardano-crypto.js

    const auto privateKey = PrivateKey(parse_hex(
        "d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"
        "d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"
        "69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "1111111111111111111111111111111111111111111111111111111111111111"
    ));

    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
    EXPECT_EQ(hex(publicKey.bytes), 
        "e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf698"
        "69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"
        "857eed804ff087b97f87848f6493e87257a8c5203cb9f422f6e7a7d8a4d299f3"
        "1111111111111111111111111111111111111111111111111111111111111111"
    );

    const auto sampleMessageStr = "Hello world";
    const auto sampleMessage = data(sampleMessageStr);

    const auto signature = privateKey.sign(sampleMessage, TWCurveED25519Extended);

    const auto sampleRightSignature = "1096ddcfb2ad21a4c0d861ef3fabe18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d829bf0bcf1b631e86f0e";
    EXPECT_EQ(hex(signature), sampleRightSignature);
}
