// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Transaction.h"
#include "Cardano/AddressV3.h"
#include <TrustWalletCore/TWCardano.h>

#include "HexCoding.h"
#include "Cbor.h"

#include <gtest/gtest.h>


using namespace TW::Cardano;
using namespace TW;
using namespace std;


Transaction createTx() {
    Transaction tx;
    tx.inputs.emplace_back(parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767"), 1);
    tx.inputs.emplace_back(parse_hex("554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0"), 0);
    tx.outputs.emplace_back(
        AddressV3("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23").data(),
        2000000
    );
    tx.outputs.emplace_back(
        AddressV3("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5").data(),
        16749189
    );
    tx.fee = 165555;
    tx.ttl = 53333345;
    return tx;
}

TEST(CardanoTransaction, Encode) {
    const Transaction tx = createTx();

    const auto encoded = tx.encode();
    EXPECT_EQ(hex(encoded), "a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000018282583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a001e848082583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a00ff9285021a000286b3031a032dcd61");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.getMapElements().size(), 4);
        EXPECT_EQ(decode.dumpToString(), "{0: [[h\"f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767\", 1], [h\"554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0\", 0]], 1: [[h\"01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\", 2000000], [h\"01558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5\", 16749189]], 2: 165555, 3: 53333345}");
    }
}

TEST(CardanoTransaction, GetId) {
    const Transaction tx = createTx();

    const auto txid = tx.getId();
    EXPECT_EQ(hex(txid), "cc262713a3e15a0fa245b062f33ffc6c2aa5a64c3ae7bfa793414069914e1bbf");
}

TEST(CardanoTransaction, minAdaAmount) {
    const auto policyId = "9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77";

    {   // ADA only
        const auto tb = TokenBundle();
        EXPECT_EQ(tb.minAdaAmount(), 1000000);
    }
    {   // 1 policyId, 1 6-char asset name
        const auto tb = TokenBundle({TokenAmount(policyId, "TOKEN1", 0)});
        EXPECT_EQ(tb.minAdaAmount(), 1444443);
    }
    {   // 2 policyId, 2 4-char asset names
        auto tb = TokenBundle();
        tb.add(TokenAmount("012345678901234567890POLICY1", "TOK1", 20));
        tb.add(TokenAmount("012345678901234567890POLICY2", "TOK2", 20));
        EXPECT_EQ(tb.minAdaAmount(), 1629628);
    }
    {   // 10 policyId, 10 6-char asset names
        auto tb = TokenBundle();
        for (auto i = 0; i < 10; ++i) {
            string policyId1 =  + "012345678901234567890123456" + std::to_string(i);
            string name = "ASSET" + std::to_string(i);
            tb.add(TokenAmount(policyId1, name, 0));
        }
        EXPECT_EQ(tb.minAdaAmount(), 3370367);
    }

    EXPECT_EQ(TokenBundle::minAdaAmountHelper(0, 0, 0), 1000000); // ADA only
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(1, 0, 0), 1370369); // 1 policyId, no asset name
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(1, 1, 1), 1444443); // 1 policyId, 1 1-char asset name
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(1, 1, 6), 1444443); // 1 policyId, 1 6-char asset name
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(1, 1, 32), 1555554); // 1 policyId, 1 32-char asset name
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(1, 110, 110*32), 23777754); // 1 policyId, 110 32-char asset name
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(2, 2, 8), 1629628); // 2 policyId, 2 4-char asset names
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(3, 5, 20), 1999998); // 3 policyId, 5 4-char asset names
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(10, 10, 10*6), 3370367); // 10 policyId, 10 6-char asset names
    EXPECT_EQ(TokenBundle::minAdaAmountHelper(60, 60, 60*32), 21222201); // 60 policyId, 60 32-char asset names
}

TEST(TWCardanoTransaction, minAdaAmount) {
    {   // ADA-only
        const auto bundleProto = TokenBundle().toProto();
        const auto bundleProtoData = data(bundleProto.SerializeAsString());
        EXPECT_EQ(TWCardanoMinAdaAmount(&bundleProtoData), 1000000);
    }
    {   // 2 policyId, 2 4-char asset names
        auto bundle = TokenBundle();
        bundle.add(TokenAmount("012345678901234567890POLICY1", "TOK1", 20));
        bundle.add(TokenAmount("012345678901234567890POLICY2", "TOK2", 20));
        const auto bundleProto = bundle.toProto();
        const auto bundleProtoData = data(bundleProto.SerializeAsString());
        EXPECT_EQ(TWCardanoMinAdaAmount(&bundleProtoData), 1629628);
    }
}
