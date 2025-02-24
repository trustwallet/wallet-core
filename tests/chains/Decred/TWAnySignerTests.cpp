
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "HexCoding.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Decred.pb.h"
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>


namespace TW::Decred {

Bitcoin::Proto::SigningInput createInput() {
    const int64_t utxoValue = 39900000;
    const int64_t amount = 10000000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("Dsesp1V6DZDEtcq2behmBVKdYqKMdkh96hL");
    input.set_change_address("DsUoWCAxprdGNtKQqambFbTcSBgH1SHn9Gp");
    input.set_coin_type(TWCoinTypeDecred);

    auto& utxo = *input.add_utxo();

    auto hash = parse_hex("fdbfe9dd703f306794a467f175be5bd9748a7925033ea1cf9889d7cf4dd11550");
    auto script = parse_hex("76a914b75fdec70b2e731795dd123ab40f918bf099fee088ac");
    auto utxoKey = parse_hex("ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764");

    utxo.set_amount(utxoValue);
    utxo.set_script(script.data(), script.size());
    
    auto& outpoint = *utxo.mutable_out_point();
    outpoint.set_hash(hash.data(), hash.size());
    outpoint.set_index(0);

    input.add_private_key(utxoKey.data(), utxoKey.size());
    return input;
}

TEST(TWAnySignerDecred, Signing) {
    auto input = createInput();

    const int64_t utxoValue = 39900000;
    const int64_t amount = 10000000;
    const int64_t fee = 100000;

    auto& plan = *input.mutable_plan();
    plan.set_amount(amount);
    plan.set_available_amount(utxoValue);
    plan.set_fee(fee);
    plan.set_change(utxoValue - amount - fee);
    auto& planUtxo = *plan.add_utxos();
    planUtxo = input.utxo(0);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeDecred);

    ASSERT_EQ(output.error(), Common::Proto::OK);
    ASSERT_EQ(hex(output.encoded()), "0100000001fdbfe9dd703f306794a467f175be5bd9748a7925033ea1cf9889d7cf4dd1155000000000000000000002809698000000000000001976a914989b1aecabf1c24e213cc0f2d8a22ffee25dd4e188ac40b6c6010000000000001976a9142a194fc92e27fef9cc2b057bc9060c580cbb484888ac000000000000000001000000000000000000000000ffffffff6a47304402206ee887c9239e5fff0048674bdfff2a8cfbeec6cd4a3ccebcc12fac44b24cc5ac0220718f7c760818fde18bc5ba8457d43d5a145cc4cf13d2a5557cba9107e9f4558d0121026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6");
}

TEST(TWAnySignerDecred, Plan) {
    auto input = createInput();

    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeDecred);

    EXPECT_EQ(plan.amount(), 10000000);
    EXPECT_EQ(plan.available_amount(), 39900000);
    EXPECT_EQ(plan.fee(), 254);
    EXPECT_EQ(plan.change(), 29899746);
    EXPECT_EQ(plan.utxos_size(), 1);
    EXPECT_EQ(plan.branch_id(), "");
}

TEST(TWAnySignerDecred, PlanAndSign) {
    auto input = createInput();

    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeDecred);

    EXPECT_EQ(plan.amount(), 10000000);
    EXPECT_EQ(plan.available_amount(), 39900000);
    EXPECT_EQ(plan.fee(), 254);
    EXPECT_EQ(plan.change(), 29899746);
    EXPECT_EQ(plan.utxos_size(), 1);
    EXPECT_EQ(plan.branch_id(), "");
    
    // copy over plan fields
    *input.mutable_plan() = plan;

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeDecred);

    ASSERT_EQ(output.error(), Common::Proto::OK);
    ASSERT_EQ(output.encoded().size(), 251ul);
    ASSERT_EQ(hex(output.encoded()), "0100000001fdbfe9dd703f306794a467f175be5bd9748a7925033ea1cf9889d7cf4dd1155000000000000000000002809698000000000000001976a914989b1aecabf1c24e213cc0f2d8a22ffee25dd4e188ace23bc8010000000000001976a9142a194fc92e27fef9cc2b057bc9060c580cbb484888ac000000000000000001000000000000000000000000ffffffff6a47304402203e6ee9e16d6bc36bb4242f7a4cac333a1c2a150ea16143412b88b721f6ae16bf02201019affdf815a5c22e4b0fb7e4685c4707094922d6a41354f9055d3bb0f26e630121026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6");
}

TEST(TWAnySignerDecred, SupportsJSON) {
    ASSERT_FALSE(TWAnySignerSupportsJSON(TWCoinTypeDecred));
}

TEST(TWAnySignerDecred, SignV2) {
    auto privateKey = parse_hex("99ed469e6b7d9f188962940d9d0f9fd8582c6c37e52394348f177ff0526b8a03");
    auto txId = parse_hex("c5cc3b1fc20c9e43a7d1127ba7e4802d04c16515a7eaaad58a1bc388acacfeae");
    std::reverse(txId.begin(), txId.end());
    int64_t inAmount = 100'000'000;
    int64_t outAmount1 = 10'000'000;
    int64_t outAmount2 = 5'000'000;
    auto senderAddress = "DscNJ2Ki7HUPHrLGF2teBxSda3uxKSY7Fm6";
    auto toAddress = "Dsofok7qyhDLVRXcTqYdFgmGsUFSiHonbWH";

    BitcoinV2::Proto::SigningInput signing;
    signing.add_private_keys(privateKey.data(), privateKey.size());

    auto& chainInfo = *signing.mutable_chain_info();
    chainInfo.set_p2pkh_prefix(63);
    chainInfo.set_p2sh_prefix(26);

    auto& builder = *signing.mutable_builder();
    builder.set_version(BitcoinV2::Proto::TransactionVersion::UseDefault);
    builder.set_input_selector(BitcoinV2::Proto::InputSelector::SelectDescending);
    builder.set_fixed_dust_threshold(546);
    builder.set_fee_per_vb(10);

    auto& in = *builder.add_inputs();
    auto& inOutPoint = *in.mutable_out_point();
    inOutPoint.set_hash(txId.data(), txId.size());
    inOutPoint.set_vout(0);
    in.set_value(inAmount);
    in.set_receiver_address(senderAddress);
    in.set_sighash_type(TWBitcoinSigHashTypeAll);

    // 0.1 DCR to another address.
    auto& out1 = *builder.add_outputs();
    out1.set_value(outAmount1);
    out1.set_to_address(toAddress);

    // 0.05 DCR to self.
    auto& out2 = *builder.add_outputs();
    out2.set_value(outAmount2);
    out2.set_to_address(senderAddress);

    // Send remaining amount to self by my address.
    auto& changeOut = *builder.mutable_change_output();
    changeOut.set_to_address(senderAddress);

    Bitcoin::Proto::SigningInput legacy;
    legacy.set_coin_type(TWCoinTypeDecred);
    *legacy.mutable_signing_v2() = signing;

    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(legacy, plan, TWCoinTypeDecred);
    EXPECT_EQ(plan.error(), Common::Proto::OK);
    ASSERT_TRUE(plan.has_planning_result_v2());
    EXPECT_EQ(plan.planning_result_v2().error(), Common::Proto::SigningError::OK)
                    << plan.planning_result_v2().error_message();
    EXPECT_EQ(plan.planning_result_v2().vsize_estimate(), 289ul);

    Proto::SigningOutput output;
    ANY_SIGN(legacy, TWCoinTypeDecred);

    EXPECT_EQ(output.error(), Common::Proto::OK);
    ASSERT_TRUE(output.has_signing_result_v2());
    EXPECT_EQ(output.signing_result_v2().error(), Common::Proto::SigningError::OK)
                    << output.signing_result_v2().error_message();
    EXPECT_EQ(hex(output.signing_result_v2().encoded()), "0100000001aefeacac88c31b8ad5aaeaa71565c1042d80e4a77b12d1a7439e0cc21f3bccc50000000000ffffffff03809698000000000000001976a914f90f06478396b97df24c012b922f953fa894676188ac404b4c000000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988acf6f310050000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988ac00000000000000000100e1f5050000000000000000ffffffff6b483045022100c5464db9df8196c563db40ba1f7650291c755713ad87920a70c38c15db17bc3d02201c6bc9e4f8e43fdd548b0be387dd7baf7270bced8c9da4148128a655bed52e1a01210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b");
    EXPECT_EQ(hex(output.signing_result_v2().txid()), "686a9057b6cf4d8aec1dd4ec0963b6f8e172d9fe90784456d2f93c403f163409");
}

} // namespace
