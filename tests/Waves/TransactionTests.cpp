// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Waves/Address.h"
#include "Waves/Transaction.h"

#include <Waves/Signer.h>
#include <gtest/gtest.h>

using json = nlohmann::json;

using namespace std;
using namespace TW;
using namespace TW::Waves;

TEST(WavesTransaction, serialize) {
    auto tx1 = Transaction(
        /* amount */ 100000000,
        /* amount asset */ "",
        /* fee */ 100000000,
        /* fee asset*/ Transaction::WAVES,
        /* to */ Address("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx"),
        /* attachment */ parse_hex("66616c6166656c"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef22"));
    auto serialized1 = tx1.serializeToSign();
    ASSERT_EQ(hex(serialized1), "0402d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef"
                                "2200000000016372e852120000000005f5e1000000000005f5e1000157cdc9381c"
                                "071beb5abd27738d5cd36cf75f3cbfdd69e8e6bb000766616c6166656c");

    auto tx2 = Transaction(
        /* amount */ 1,
        /* amount asset */ "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq",
        /* fee */ 1,
        /* fee asset*/ "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq",
        /* to */ Address("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx"),
        /* attachment */ Data(),
        /* timestamp */ 1,
        /* pub_key */
        parse_hex("d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef22"));
    auto serialized2 = tx2.serializeToSign();
    ASSERT_EQ(hex(serialized2),
              "0402d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef2201bae8ddc9955fa6"
              "f69f8e7b155efcdb97bc3bb3a95db4c4604408cec245cd187201bae8ddc9955fa6f69f8e7b155efcdb97"
              "bc3bb3a95db4c4604408cec245cd18720000000000000001000000000000000100000000000000010157"
              "cdc9381c071beb5abd27738d5cd36cf75f3cbfdd69e8e6bb0000");
}

TEST(WavesTransaction, failedSerialize) {
    // 141 bytes attachment
    auto tx1 = Transaction(
        /* amount */ 100000000,
        /* amount asset */ "",
        /* fee */ 100000000,
        /* fee asset*/ "",
        /* to */ Address("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx"),
        /* attachment */
        parse_hex("66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166"
                  "656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c"
                  "6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66"
                  "616c6166656c66616c6166656c66616c6166656c01"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef22"));
    EXPECT_THROW(tx1.serializeToSign(), invalid_argument);
}

TEST(WavesTransaction, jsonSerialize) {
    const auto privateKey =
        PrivateKey(parse_hex("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    const auto publicKeyCurve25519 = privateKey.getPublicKey(TWPublicKeyTypeCURVE25519);
    ASSERT_EQ(hex(Data(publicKeyCurve25519.bytes.begin(), publicKeyCurve25519.bytes.end())),
              "559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d");
    // 3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds
    const auto address = Address(publicKeyCurve25519);

    auto tx1 = Transaction(
        /* amount */ 10000000,
        /* amount asset */ "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq",
        /* fee */ 100000000,
        /* fee asset*/ "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD82zq",
        /* to */ address,
        /* attachment */ parse_hex("66616c6166656c"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d"));

    auto signature = Signer::sign(privateKey, tx1);

    auto json = tx1.buildJson(signature);

    ASSERT_EQ(json["type"], TransactionType::transfer);
    ASSERT_EQ(json["version"], TransactionVersion::V2);
    ASSERT_EQ(json["fee"], 100000000);
    ASSERT_EQ(json["senderPublicKey"], "6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU");
    ASSERT_EQ(json["timestamp"], 1526641218066);
    ASSERT_EQ(json["proofs"].dump(), "[\"5ynN2NUiFHkQzw9bK8R7dZcNfTWMAtcWRJsrMvFFM6dUT3fSnPCCX7CTajNU8bJCB"
                              "H69vU1mnwfx4zpDtF1SkzKg\"]");
    ASSERT_EQ(json["recipient"], "3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds");
    ASSERT_EQ(json["assetId"], "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    ASSERT_EQ(json["feeAssetId"], "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD82zq");
    ASSERT_EQ(json["amount"], 10000000);
    ASSERT_EQ(json["attachment"], "4t2Xazb2SX");
    ASSERT_EQ(json.dump(), "{\"amount\":10000000,\"assetId\":\"DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq\",\"attachment\":\"4t2Xazb2SX\",\"fee\":100000000,\"feeAssetId\":\"DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD82zq\",\"proofs\":[\"5ynN2NUiFHkQzw9bK8R7dZcNfTWMAtcWRJsrMvFFM6dUT3fSnPCCX7CTajNU8bJCBH69vU1mnwfx4zpDtF1SkzKg\"],\"recipient\":\"3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds\",\"senderPublicKey\":\"6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU\",\"timestamp\":1526641218066,\"type\":4,\"version\":2}");
}