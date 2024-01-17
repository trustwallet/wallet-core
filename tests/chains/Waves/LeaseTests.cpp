// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Waves/Address.h"
#include "Waves/Transaction.h"
#include "proto/Waves.pb.h"

#include <Waves/Signer.h>
#include <gtest/gtest.h>

using json = nlohmann::json;

using namespace std;
using namespace TW;

namespace TW::Waves::tests {

TEST(WavesLease, serialize) {
    const auto privateKey =
        PrivateKey(parse_hex("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1526646497465));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto& message = *input.mutable_lease_message();
    message.set_amount(int64_t(100000000));
    message.set_fee(int64_t(100000));
    message.set_to("3P9DEDP5VbyXQyKtXDUt2crRPn5B7gs6ujc");
    auto tx1 = Transaction(
        input,
        /* pub_key */
        parse_hex("425f57a8cb5439e4e912e66376f7041565d029ae4437dae1a3ebe15649d43461"));
    auto serialized1 = tx1.serializeToSign();
    ASSERT_EQ(hex(serialized1), "080200425f57a8cb5439e4e912e66376f7041565d029ae4437dae1a3ebe15649d4346101574"
                                "fdfcd1bfb19114bd2ac369e32013c70c6d03a4627879cbf0000000005f5e100000000000001"
                                "86a0000001637338e0b9");
}

TEST(WavesLease, CancelSerialize) {
    const auto privateKey =
        PrivateKey(parse_hex("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1568831000826));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto& message = *input.mutable_cancel_lease_message();
    message.set_fee(int64_t(100000));
    message.set_lease_id("44re3UEDw1QwPFP8dKzfuGHVMNBejUW9NbhxG6b4KJ1T");
    auto tx1 = Transaction(
        input,
        /* pub_key */
        parse_hex("425f57a8cb5439e4e912e66376f7041565d029ae4437dae1a3ebe15649d43461"));
    auto serialized1 = tx1.serializeToSign();
    ASSERT_EQ(hex(serialized1), "090257425f57a8cb5439e4e912e66376f7041565d029ae4437dae1a3ebe15649d"
                                "4346100000000000186a00000016d459d50fa2d8fee08efc97f79bcd97a4d977c"
                                "76183580d723909af2b50e72b02f1e36707e");
}

TEST(WavesLease, jsonSerialize) {
    const auto privateKey = PrivateKey(parse_hex(
        "9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    const auto publicKeyCurve25519 =
        privateKey.getPublicKey(TWPublicKeyTypeCURVE25519);
    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1568973547102));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto& message = *input.mutable_lease_message();
    message.set_amount(int64_t(100000));
    message.set_fee(int64_t(100000));
    message.set_to("3P9DEDP5VbyXQyKtXDUt2crRPn5B7gs6ujc");
    auto tx1 = Transaction(input,
                           /* pub_key */
                           parse_hex("559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d"));

    auto signature = Signer::sign(privateKey, tx1);
    auto json = tx1.buildJson(signature);

    ASSERT_EQ(json["type"], TransactionType::lease);
    ASSERT_EQ(json["version"], TransactionVersion::V2);
    ASSERT_EQ(json["fee"], int64_t(100000));
    ASSERT_EQ(json["senderPublicKey"],
              "6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU");
    ASSERT_EQ(json["timestamp"], int64_t(1568973547102));
    ASSERT_EQ(json["proofs"].dump(),
              "[\"4opce9e99827upK3m3D3NicnvBqbMLtAJ4Jc8ksTLiScqBgjdqzr9JyXG"
              "C1NAGZUbkqJvix9bNrBokrxtGruwmu3\"]");
    ASSERT_EQ(json["recipient"], "3P9DEDP5VbyXQyKtXDUt2crRPn5B7gs6ujc");
    ASSERT_EQ(json["amount"], int64_t(100000));
    ASSERT_EQ(json.dump(),
              "{\"amount\":100000,\"fee\":100000,\"proofs\":["
              "\"4opce9e99827upK3m3D3NicnvBqbMLtAJ4Jc8ksTLiScqBgjdqzr9JyXGC1NAGZUbkqJ"
              "vix9bNrBokrxtGruwmu3\"],\"recipient\":"
              "\"3P9DEDP5VbyXQyKtXDUt2crRPn5B7gs6ujc\",\"senderPublicKey\":"
              "\"6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU\",\"timestamp\":"
              "1568973547102,\"type\":8,\"version\":2}");
}

TEST(WavesLease, jsonCancelSerialize) {
    const auto privateKey = PrivateKey(parse_hex(
        "9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    const auto publicKeyCurve25519 =
        privateKey.getPublicKey(TWPublicKeyTypeCURVE25519);
    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1568973547102));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto& message = *input.mutable_cancel_lease_message();
    message.set_lease_id("DKhmXrCsBwf6WVhGh8bYVBnjtAXGpk2K4Yd3CW4u1huG");
    message.set_fee(int64_t(100000));
    auto tx1 = Transaction(input,
                           /* pub_key */
                           parse_hex("559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d"));
    auto signature = Signer::sign(privateKey, tx1);
    auto json = tx1.buildJson(signature);

    ASSERT_EQ(json["type"], TransactionType::cancelLease);
    ASSERT_EQ(json["version"], TransactionVersion::V2);
    ASSERT_EQ(json["fee"], int64_t(100000));
    ASSERT_EQ(json["senderPublicKey"],
              "6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU");
    ASSERT_EQ(json["leaseId"], "DKhmXrCsBwf6WVhGh8bYVBnjtAXGpk2K4Yd3CW4u1huG");
    ASSERT_EQ(json["chainId"], 87);
    ASSERT_EQ(json["timestamp"], int64_t(1568973547102));
    ASSERT_EQ(json["proofs"].dump(),
              "[\"Mwhh7kdbhPv9vtnPh6pjEcHTFJ5h5JtAziwFpqH8Ykw1yWYie4Nquh"
              "eYtAWPbRowgpDVBxvG1rTrv82LnFdByQY\"]");
    ASSERT_EQ(json.dump(),
              "{\"chainId\":87,\"fee\":100000,\"leaseId\":\"DKhmXrCsBwf6WVhGh8bYVBnjtAXGpk2K4Yd3CW4u1huG\","
              "\"proofs\":[\"Mwhh7kdbhPv9vtnPh6pjEcHTFJ5h5JtAziwFpqH8Ykw1yWYie4NquheYtAWP"
              "bRowgpDVBxvG1rTrv82LnFdByQY\"],\"senderPublicKey\":"
              "\"6mA8eQjie53kd4jbZrwL3ZhMBqCX6nzit1k55tR2X7zU\",\"timestamp\":"
              "1568973547102,\"type\":9,\"version\":2}");
}

} // namespace TW::Waves::tests
