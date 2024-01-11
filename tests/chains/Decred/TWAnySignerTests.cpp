
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

} // namespace
