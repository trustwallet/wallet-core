// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Verge/Signer.h"
#include "Verge/Transaction.h"
#include "Verge/TransactionBuilder.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

using namespace TW;
using namespace TW::Bitcoin;


TEST(NeblSigner, Sign) {
    const int64_t amount = 1500000000;
    const int64_t fee = 2000000;
    const std::string toAddress = "NRrKgiZfT7jUdS3geoEBproA7hzZnDQAdr";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    input.set_coin_type(TWCoinTypeNebl);
    input.set_time(1656474580);

    auto txHash0 = parse_hex("a5a6e147da0f1b3f6dfd1081f91b0c6e31f030ae66c4be4cf4b0db0ac8b2407d");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(4294967294);
    utxo0->set_amount(2500000000);

    auto utxoKey0 = PrivateKey(parse_hex("4222aae79af41eade7b07ce6fd44d926ea8e3f95e51a06e85f8bdec89680cbd9"));
    auto script0 = Bitcoin::Script::lockScriptForAddress("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc", TWCoinTypeNebl);
    ASSERT_EQ(hex(script0.bytes), "76a914ae40b2142aba5ddd10f74d9440bfda8a36cbad5b88ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto plan = TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = 980000000;

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto result = Verge::Signer::sign(input);
    
    ASSERT_EQ(hex(result.encoded()),
        "01000000d4cbbb62017d40b2c80adbb0f44cbec466ae30f0316e0c1bf98110fd6d3f1b0fda47e1a6a5000000006a47304402207f77f7ed50ec56447fd108b2a9a693b2ac9f62e99b59dfa914f242510943187602204618fd9195050c763eb93644e51344f6c00e4dd93aa41bb42bce42c9e4cc53b6012103787a4c5ff72dce6d97f9b6360dc302b2d8a833e8c570dcc124a96e5f564bb524feffffff02002f6859000000001976a914412033ed457c72ca70bab5fbfdc03256bd2ce07d88ac009d693a000000001976a914ae40b2142aba5ddd10f74d9440bfda8a36cbad5b88ac00000000"
    );
}

TEST(NeblSigner, SignAnyoneCanPay) {
    const int64_t amount = 1500000000;
    const int64_t fee = 2000000;
    const std::string toAddress = "NRrKgiZfT7jUdS3geoEBproA7hzZnDQAdr";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAnyoneCanPay|TWBitcoinSigHashTypeSingle);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    input.set_coin_type(TWCoinTypeNebl);
    input.set_time(1656474580);

    auto txHash0 = parse_hex("a5a6e147da0f1b3f6dfd1081f91b0c6e31f030ae66c4be4cf4b0db0ac8b2407d");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(4294967294);
    utxo0->set_amount(2500000000);

    auto script0 = Bitcoin::Script::lockScriptForAddress("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc", TWCoinTypeNebl);
    EXPECT_EQ(hex(script0.bytes), "76a914ae40b2142aba5ddd10f74d9440bfda8a36cbad5b88ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());

    auto txHash1 = parse_hex("29bd442521ea303afb09ad2583f589a6527c9218c050882b6b8527bbe4d11766");
    std::reverse(txHash1.begin(), txHash1.end());

    auto utxo1 = input.add_utxo();
    utxo1->mutable_out_point()->set_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_index(0);
    utxo1->mutable_out_point()->set_sequence(4294967294);
    utxo1->set_amount(200000000);

    auto script1 = Bitcoin::Script::lockScriptForAddress("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc", TWCoinTypeNebl);
    ASSERT_EQ(hex(script1.bytes), "76a914ae40b2142aba5ddd10f74d9440bfda8a36cbad5b88ac");
    utxo1->set_script(script1.bytes.data(), script1.bytes.size());

    auto utxoKey0 = PrivateKey(parse_hex("4222aae79af41eade7b07ce6fd44d926ea8e3f95e51a06e85f8bdec89680cbd9"));
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto plan = TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = 1000000000;

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto result = Bitcoin::TransactionSigner<Verge::Transaction, Verge::TransactionBuilder>::sign(input);
    ASSERT_TRUE(result) << std::to_string(result.error());
    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(serialized);
    ASSERT_EQ(hex(serialized),
        "01000000d4cbbb62017d40b2c80adbb0f44cbec466ae30f0316e0c1bf98110fd6d3f1b0fda47e1a6a5000000006b483045022100827715814a853164cf04b7106ef508eeadc5d868616756d5f598eab977c3ea2d022024ab244bfda0d31a4454ec70bedc07391fd956c94bb80f6164dc45188e0507d6832103787a4c5ff72dce6d97f9b6360dc302b2d8a833e8c570dcc124a96e5f564bb524feffffff02002f6859000000001976a914412033ed457c72ca70bab5fbfdc03256bd2ce07d88ac00ca9a3b000000001976a914ae40b2142aba5ddd10f74d9440bfda8a36cbad5b88ac00000000"
    );
}

TEST(NeblSigner, SignWithError) {
    const int64_t amount = 1500000000;
    const std::string toAddress = "NRrKgiZfT7jUdS3geoEBproA7hzZnDQAdr";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    input.set_coin_type(TWCoinTypeNebl);
    input.set_time(1656474580);

    // Sign
    auto result = Verge::Signer::sign(input);

    ASSERT_NE(result.error(), Common::Proto::OK);

    // PreImage
    auto preResult = Verge::Signer::preImageHashes(input);
    ASSERT_NE(preResult.error(), Common::Proto::OK);
}

