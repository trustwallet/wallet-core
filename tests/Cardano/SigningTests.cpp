// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Signer.h"
#include "Cardano/AddressV3.h"

#include "PrivateKey.h"
#include "HexCoding.h"
#include "Cbor.h"

#include <gtest/gtest.h>
#include <vector>


using namespace TW::Cardano;
using namespace TW;
using namespace std;


TEST(CardanoSigning, selectInputsSimple) {
    const auto inputs = std::vector<TxInput>({
        TxInput{{parse_hex("0001"), 0}, 700},
        TxInput{{parse_hex("0002"), 1}, 900},
        TxInput{{parse_hex("0003"), 2}, 300},
        TxInput{{parse_hex("0004"), 3}, 600},
    });

    {   // 2
        const auto s1 = Signer::selectInputsSimple(inputs, 1500);
        ASSERT_EQ(s1.size(), 2);
        EXPECT_EQ(s1[0].amount, 900);
        EXPECT_EQ(s1[1].amount, 700);
    }
    {   // all
        const auto s1 = Signer::selectInputsSimple(inputs, 10000);
        ASSERT_EQ(s1.size(), 4);
        EXPECT_EQ(s1[0].amount, 900);
        EXPECT_EQ(s1[1].amount, 700);
        EXPECT_EQ(s1[2].amount, 600);
        EXPECT_EQ(s1[3].amount, 300);
    }
    {   // 3
        const auto s1 = Signer::selectInputsSimple(inputs, 2000);
        ASSERT_EQ(s1.size(), 3);
    }
    {   // 1
        const auto s1 = Signer::selectInputsSimple(inputs, 500);
        ASSERT_EQ(s1.size(), 1);
    }
    {   // at least 0 is returned
        const auto s1 = Signer::selectInputsSimple(inputs, 0);
        ASSERT_EQ(s1.size(), 1);
    }
}

Proto::SigningInput createSampleInput() {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
    utxo1->mutable_out_point()->set_tx_hash(std::string(txHash1.begin(), txHash1.end()));
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_amount(1500000);
    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0");
    utxo2->mutable_out_point()->set_tx_hash(std::string(txHash2.begin(), txHash2.end()));
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_amount(6500000);

    const auto privateKeyData = parse_hex("484d5bc3375435c9a0b0d121e86d6eba7d55929a263530d75bb1cb1d098bc653f75e5127219208a9d28ca6fecd95530b97470957f72be0648b2c99e2a29cc841fa49248845144b2dbccf6e85c172cd44099f5977e7126209e7c3e4b1fba4a264f0607785e71ace08560b30e3ffaab13f46418493f223e1407c0e6358108bc65351b30406485ad396ac0a390f6d718413fc5256069a36dd7bc2670fc49597d83556a8d4c88d10db7883dad1e70e03bbfbdff3545d33eb4cf2ea8f044a29462cde");
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
    input.mutable_transfer_message()->set_change_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
    input.mutable_transfer_message()->set_amount(7000000);
    input.set_ttl(53333333);
    return input;
}

TEST(CardanoSigning, Plan) {
    auto input = createSampleInput();

    {
        auto signer = Signer(input);
        const auto plan = signer.plan();
        EXPECT_EQ(plan.utxos.size(), 2);
        EXPECT_EQ(plan.availableAmount, 8000000);
        EXPECT_EQ(plan.amount, 7000000);
        EXPECT_EQ(plan.fee, 165489);
        EXPECT_EQ(plan.change, 834511);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
        EXPECT_EQ(plan.error, Common::Proto::OK);
    }
    {   // very small target amount
        input.mutable_transfer_message()->set_amount(1);
        auto signer = Signer(input);
        const auto plan = signer.plan();
        EXPECT_EQ(plan.utxos.size(), 1);
        EXPECT_EQ(plan.availableAmount, 6500000);
        EXPECT_EQ(plan.amount, 1);
        EXPECT_EQ(plan.fee, 163731);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    {   // small target amount
        input.mutable_transfer_message()->set_amount(2000000);
        auto signer = Signer(input);
        const auto plan = signer.plan();
        EXPECT_EQ(plan.utxos.size(), 1);
        EXPECT_EQ(plan.availableAmount, 6500000);
        EXPECT_EQ(plan.amount, 2000000);
        EXPECT_EQ(plan.fee, 163907);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
}

TEST(CardanoSigning, Sign1) {
    const auto input = createSampleInput();

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a006acfc082583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a000cbbcf021a00028671031a032dcd55");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "8b9f26b3d1fa9f971379a72c2d577abc73ca425f5dcb1f2b8ea436a09b6f0f87");
}

TEST(CardanoSigning, SignMessageWithKey) {
    // test case from cardano-crypto.js

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
