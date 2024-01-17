// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Waves/Address.h"
#include "proto/Waves.pb.h"
#include "Waves/Transaction.h"

#include <Waves/Signer.h>
#include <gtest/gtest.h>

namespace TW::Waves::tests {

using json = nlohmann::json;
using namespace std;

TEST(WavesTransaction, serialize) {
    const auto privateKey =
        PrivateKey(parse_hex("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1526641218066));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto& message = *input.mutable_transfer_message();
    message.set_amount(int64_t(100000000));
    message.set_asset("");
    message.set_fee(int64_t(100000000));
    message.set_fee_asset(Transaction::WAVES);
    message.set_to("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx");
    message.set_attachment("falafel");
    auto tx1 = Transaction(
        input,
        /* pub_key */
        parse_hex("d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef22"));
    auto serialized1 = tx1.serializeToSign();
    ASSERT_EQ(hex(serialized1), "0402d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef"
                                "2200000000016372e852120000000005f5e1000000000005f5e1000157cdc9381c"
                                "071beb5abd27738d5cd36cf75f3cbfdd69e8e6bb000766616c6166656c");

    auto input2 = Proto::SigningInput();
    input2.set_timestamp(int64_t(1));
    input2.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto& message2 = *input2.mutable_transfer_message();
    message2.set_amount(int64_t(1));
    message2.set_asset("DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    message2.set_fee(int64_t(1));
    message2.set_fee_asset("DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    message2.set_to("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx");
    message2.set_attachment("");

    auto tx2 = Transaction(
        input2,
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
    const auto privateKey =
        PrivateKey(parse_hex("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1526641218066));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto& message = *input.mutable_transfer_message();
    message.set_amount(int64_t(100000000));
    message.set_asset("");
    message.set_fee(int64_t(100000000));
    message.set_fee_asset("");
    message.set_to("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx");
    message.set_attachment("falafelfalafelfalafelfalafelfalafelfalafelfalafel"
                           "falafelfalafelfalafelfalafelfalafelfalafelfalafel"
                           "falafelfalafelfalafelfalafelfalafelfalafel");
    auto tx1 = Transaction(
        input,
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
    const auto address = Address(publicKeyCurve25519);

    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1526641218066));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto& message = *input.mutable_transfer_message();
    message.set_amount(int64_t(10000000));
    message.set_asset("DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    message.set_fee(int64_t(100000000));
    message.set_fee_asset("DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD82zq");
    message.set_to(address.string());
    message.set_attachment("falafel");
    auto tx1 = Transaction(
        input,
        /* pub_key */
        parse_hex("559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d"));

    auto signature = Signer::sign(privateKey, tx1);

    auto json = tx1.buildJson(signature);

    ASSERT_EQ(json["type"], TransactionType::transfer);
    ASSERT_EQ(json["version"], TransactionVersion::V2);
    ASSERT_EQ(json["fee"], int64_t(100000000));
    ASSERT_EQ(json["senderPublicKey"], "6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU");
    ASSERT_EQ(json["timestamp"], int64_t(1526641218066));
    ASSERT_EQ(json["proofs"].dump(), "[\"5ynN2NUiFHkQzw9bK8R7dZcNfTWMAtcWRJsrMvFFM6dUT3fSnPCCX7CTajNU8bJCB"
                                     "H69vU1mnwfx4zpDtF1SkzKg\"]");
    ASSERT_EQ(json["recipient"], "3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds");
    ASSERT_EQ(json["assetId"], "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    ASSERT_EQ(json["feeAssetId"], "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD82zq");
    ASSERT_EQ(json["amount"], int64_t(10000000));
    ASSERT_EQ(json["attachment"], "4t2Xazb2SX");
    ASSERT_EQ(json.dump(), "{\"amount\":10000000,\"assetId\":\"DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq\",\"attachment\":\"4t2Xazb2SX\",\"fee\":100000000,\"feeAssetId\":\"DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD82zq\",\"proofs\":[\"5ynN2NUiFHkQzw9bK8R7dZcNfTWMAtcWRJsrMvFFM6dUT3fSnPCCX7CTajNU8bJCBH69vU1mnwfx4zpDtF1SkzKg\"],\"recipient\":\"3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds\",\"senderPublicKey\":\"6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU\",\"timestamp\":1526641218066,\"type\":4,\"version\":2}");
}

} // namespace TW::Waves::tests
