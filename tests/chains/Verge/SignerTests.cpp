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


TEST(VergeSigner, Sign) {
    const int64_t amount = 1500000000;
    const int64_t fee = 2000000;
    const std::string toAddress = "DQYMMpqPrnWYZaikKGTQqk5ydUaQw8nkdD";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("DAkEo5pNELZav7MRwBfEwHRG1aChgSUw6c");
    input.set_coin_type(TWCoinTypeVerge);
    input.set_time(1656474580);

    auto txHash0 = parse_hex("a5a6e147da0f1b3f6dfd1081f91b0c6e31f030ae66c4be4cf4b0db0ac8b2407d");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(4294967294);
    utxo0->set_amount(2500000000);

    auto utxoKey0 = PrivateKey(parse_hex("693dfe6f3ed717573eb10c24ebe5eb592fa3c239245cd499c487eb7b8ea7ed3a"));
    auto script0 = Bitcoin::Script::lockScriptForAddress("DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E", TWCoinTypeVerge);
    ASSERT_EQ(hex(script0.bytes), "76a914e4839a523f120882d11eb3dda13a18e11fdcbd4a88ac");
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
        "01000000d4cbbb62017d40b2c80adbb0f44cbec466ae30f0316e0c1bf98110fd6d3f1b0fda47e1a6a5000000006b483045022100bf1d0e5f84e70e699f45dd4822ecdbbfb1687e61ac749354a76f2afa2e13f76602202d4f5cda7177282b58f80163fead42300468670d03c5f4bb1db3b9596f2dcea301210220ee0423797a856fdd2e85876a60bf10f8696e6ae83e744f498f2173237fe23dfeffffff02002f6859000000001976a914d4d05406c3ca73cf887911f80c852a1c0773615088ac009d693a000000001976a9143d7e143a8b3c8a4aa2f51104da380edeb6c3fc2088ac00000000"
    );
}

TEST(VergeSigner, SignAnyoneCanPay) {
    const int64_t amount = 1500000000;
    const int64_t fee = 2000000;
    const std::string toAddress = "DQYMMpqPrnWYZaikKGTQqk5ydUaQw8nkdD";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAnyoneCanPay|TWBitcoinSigHashTypeSingle);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("DAkEo5pNELZav7MRwBfEwHRG1aChgSUw6c");
    input.set_coin_type(TWCoinTypeVerge);
    input.set_time(1656474580);

    auto txHash0 = parse_hex("a5a6e147da0f1b3f6dfd1081f91b0c6e31f030ae66c4be4cf4b0db0ac8b2407d");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(4294967294);
    utxo0->set_amount(2500000000);

    auto script0 = Bitcoin::Script::lockScriptForAddress("vg1qujpe553lzgyg95g7k0w6zwscuy0ae022h4q4zg", TWCoinTypeVerge);
    EXPECT_EQ(hex(script0.bytes), "0014e4839a523f120882d11eb3dda13a18e11fdcbd4a");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());

    auto txHash1 = parse_hex("29bd442521ea303afb09ad2583f589a6527c9218c050882b6b8527bbe4d11766");
    std::reverse(txHash1.begin(), txHash1.end());

    auto utxo1 = input.add_utxo();
    utxo1->mutable_out_point()->set_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_index(0);
    utxo1->mutable_out_point()->set_sequence(4294967294);
    utxo1->set_amount(200000000);

    auto script1 = Bitcoin::Script::lockScriptForAddress("DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E", TWCoinTypeVerge);
    ASSERT_EQ(hex(script1.bytes), "76a914e4839a523f120882d11eb3dda13a18e11fdcbd4a88ac");
    utxo1->set_script(script1.bytes.data(), script1.bytes.size());

    auto utxoKey0 = PrivateKey(parse_hex("693dfe6f3ed717573eb10c24ebe5eb592fa3c239245cd499c487eb7b8ea7ed3a"));
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
        "01000000d4cbbb620001017d40b2c80adbb0f44cbec466ae30f0316e0c1bf98110fd6d3f1b0fda47e1a6a50000000000feffffff02002f6859000000001976a914d4d05406c3ca73cf887911f80c852a1c0773615088ac00ca9a3b000000001976a9143d7e143a8b3c8a4aa2f51104da380edeb6c3fc2088ac02473044022016413b2d31c16d185cdf7c0ae343b14eee586124a8fa65bfaaec6a35eeb54e13022073e3d73d251d97fd951201ab184cdb101627317866e199ac0963b83b17e5f3bf83210220ee0423797a856fdd2e85876a60bf10f8696e6ae83e744f498f2173237fe23d00000000"
    );
}

TEST(VergeSigner, SignSegwit) {
    const int64_t amount = 1500000000;
    const int64_t fee = 2000000;
    const std::string toAddress = "DQYMMpqPrnWYZaikKGTQqk5ydUaQw8nkdD";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("DAkEo5pNELZav7MRwBfEwHRG1aChgSUw6c");
    input.set_coin_type(TWCoinTypeVerge);
    input.set_time(1656474580);

    auto txHash0 = parse_hex("a5a6e147da0f1b3f6dfd1081f91b0c6e31f030ae66c4be4cf4b0db0ac8b2407d");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(4294967294);
    utxo0->set_amount(2500000000);

    auto script0 = Bitcoin::Script::lockScriptForAddress("vg1qujpe553lzgyg95g7k0w6zwscuy0ae022h4q4zg", TWCoinTypeVerge);
    EXPECT_EQ(hex(script0.bytes), "0014e4839a523f120882d11eb3dda13a18e11fdcbd4a");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());

    auto utxoKey0 = PrivateKey(parse_hex("693dfe6f3ed717573eb10c24ebe5eb592fa3c239245cd499c487eb7b8ea7ed3a"));
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
        "01000000d4cbbb620001017d40b2c80adbb0f44cbec466ae30f0316e0c1bf98110fd6d3f1b0fda47e1a6a50000000000feffffff02002f6859000000001976a914d4d05406c3ca73cf887911f80c852a1c0773615088ac009d693a000000001976a9143d7e143a8b3c8a4aa2f51104da380edeb6c3fc2088ac024730440220657132a334ffbb15f6bbcd11da743756534c2c345195e19c007d67224f09703f022036cebc6442e212be80b74d5992cfd70355e603c1e538e84e02fecf49f82f2f8a01210220ee0423797a856fdd2e85876a60bf10f8696e6ae83e744f498f2173237fe23d00000000"
    );
}

TEST(VergeSigner, SignWithError) {
    const int64_t amount = 1500000000;
    const std::string toAddress = "DQYMMpqPrnWYZaikKGTQqk5ydUaQw8nkdD";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("DAkEo5pNELZav7MRwBfEwHRG1aChgSUw6c");
    input.set_coin_type(TWCoinTypeVerge);
    input.set_time(1656474580);

    // Sign
    auto result = Verge::Signer::sign(input);

    ASSERT_NE(result.error(), Common::Proto::OK);

    // PreImage
    auto preResult = Verge::Signer::preImageHashes(input);
    ASSERT_NE(preResult.error(), Common::Proto::OK);
}