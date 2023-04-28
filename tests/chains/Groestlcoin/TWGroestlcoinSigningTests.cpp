// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Script.h"
#include "Groestlcoin/Signer.h"
#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Bitcoin.pb.h"
#include "TestUtilities.h"
#include "../Bitcoin/TxComparisonHelper.h"
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(GroestlcoinSigning, SignP2WPKH) {
    Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeGroestlcoin);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(2500);
    input.set_byte_fee(1);
    input.set_to_address("31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P");
    input.set_change_address("Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");

    auto utxoKey0 = parse_hex("dc334e7347f2f9f72fce789b11832bdf78adf0158bc6617e6d2d2a530a0d4bc6");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Script(parse_hex("00147557920fbc32a1ef4ef26bae5e8ce3f95abf09ce"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(4774);
    auto hash0 = parse_hex("9568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f");
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    Proto::TransactionPlan plan;
    {
        // try plan first
        ANY_PLAN(input, plan, TWCoinTypeGroestlcoin);
        EXPECT_TRUE(verifyPlan(plan, {4774}, 2500, 145));
    }

    // Supply plan for signing, to match fee of previously-created real TX
    *input.mutable_plan() = plan;
    input.mutable_plan()->set_fee(226);
    input.mutable_plan()->set_change(2048);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeGroestlcoin);

    // https://blockbook.groestlcoin.org/tx/40b539c578934c9863a93c966e278fbeb3e67b0da4eb9e3030092c1b717e7a64
    EXPECT_EQ(output.transaction().outputs_size(), 2);
    EXPECT_EQ(output.transaction().outputs(0).value(), 2500);
    EXPECT_EQ(output.transaction().outputs(1).value(), 2048);
    ASSERT_EQ(hex(output.encoded()), "010000000001019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f0100000000ffffffff02c40900000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48700080000000000001976a91498af0aaca388a7e1024f505c033626d908e3b54a88ac024830450221009bbd0228dcb7343828633ded99d216555d587b74db40c4a46f560187eca222dd022032364cf6dbf9c0213076beb6b4a20935d4e9c827a551c3f6f8cbb22d8b464467012102e9c9b9b76e982ad8fa9a7f48470eafbeeba9bf6d287579318c517db5157d936e00000000");
}

TEST(GroestlcoinSigning, SignP2PKH) {
    Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeGroestlcoin);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(2500);
    input.set_byte_fee(1);
    input.set_to_address("grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne");
    input.set_change_address("31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P");

    auto utxoKey0 = parse_hex("3c3385ddc6fd95ba7282051aeb440bc75820b8c10db5c83c052d7586e3e98e84");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Script(parse_hex("76a91498af0aaca388a7e1024f505c033626d908e3b54a88ac"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(5000);
    auto hash0 = parse_hex("9568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f");
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    Proto::TransactionPlan plan;
    {
        // try plan first
        ANY_PLAN(input, plan, TWCoinTypeGroestlcoin);
        EXPECT_TRUE(verifyPlan(plan, {5000}, 2500, 221));
    }

    // Supply plan for signing, to match fee of previously-created real TX
    *input.mutable_plan() = plan;
    input.mutable_plan()->set_fee(226);
    input.mutable_plan()->set_change(2274);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeGroestlcoin);

    // https://blockbook.groestlcoin.org/tx/74a0dd12bc178cfcc1e0982a2a5b2c01a50e41abbb63beb031bcd21b3e28eac0
    EXPECT_EQ(output.transaction().outputs_size(), 2);
    EXPECT_EQ(output.transaction().outputs(0).value(), 2500);
    EXPECT_EQ(output.transaction().outputs(1).value(), 2274);
    ASSERT_EQ(hex(output.encoded()), "01000000019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f000000006a47304402202163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa3702202bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d19012103b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91ffffffff02c4090000000000001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09cee20800000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48700000000");
}

TEST(GroestlcoinSigning, SignWithError) {
    Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(2500);
    input.set_byte_fee(1);
    input.set_to_address("grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne");
    input.set_change_address("31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeGroestlcoin);

    ASSERT_NE(output.error(), Common::Proto::OK);

    auto result = Groestlcoin::Signer::preImageHashes(input);
    ASSERT_NE(result.error(), Common::Proto::OK);
}

TEST(GroestlcoinSigning, SignP2SH_P2WPKH) {
    // TX outputs
    Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeGroestlcoin);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(5'000);
    input.set_byte_fee(1);
    input.set_to_address("Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");
    input.set_change_address("grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne");

    // TX input
    auto utxoKey0 = PrivateKey(parse_hex("302fc195a8fc96c5a581471e67e4c1ac2efda252f76ad5c77a53764c70d58f91"));
    auto pubKey0 = utxoKey0.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto utxoPubkeyHash = Hash::ripemd(Hash::sha256(pubKey0.bytes));
    EXPECT_EQ(hex(utxoPubkeyHash), "2fc7d70acef142d1f7b5ef2f20b1a9b759797674");
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto redeemScript = Script::buildPayToWitnessPublicKeyHash(utxoPubkeyHash);
    auto scriptHash = Hash::ripemd(Hash::sha256(redeemScript.bytes));
    ASSERT_EQ(hex(scriptHash), "0055b0c94df477ee6b9f75185dfc9aa8ce2e52e4");
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[hex(scriptHash)] = scriptString;

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Script(parse_hex("a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e487"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(10'000);
    auto hash0 = DATA("fdae0772d7d1d33804a6b1ca0e391668b116bb7a70028427d3d82232189ce863"); // UTXO hash backwards
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    Proto::TransactionPlan plan;
    {
        // try plan first
        ANY_PLAN(input, plan, TWCoinTypeGroestlcoin);
        EXPECT_TRUE(verifyPlan(plan, {10'000}, 5000, 167));
    }

    // Supply plan for signing, to match fee of previously-created real TX
    *input.mutable_plan() = plan;
    input.mutable_plan()->set_fee(226);
    input.mutable_plan()->set_change(4774);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeGroestlcoin);

    // https://blockbook.groestlcoin.org/tx/8f4ecc7844e19aa1d3183e47eee89d795f9e7c875a55ec0203946d6c9eb06895
    EXPECT_EQ(output.transaction().outputs_size(), 2);
    EXPECT_EQ(output.transaction().outputs(0).value(), 5000);
    EXPECT_EQ(output.transaction().outputs(1).value(), 4774);
    ASSERT_EQ(hex(output.encoded()), "01000000000101fdae0772d7d1d33804a6b1ca0e391668b116bb7a70028427d3d82232189ce86300000000171600142fc7d70acef142d1f7b5ef2f20b1a9b759797674ffffffff0288130000000000001976a91498af0aaca388a7e1024f505c033626d908e3b54a88aca6120000000000001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09ce024730440220614df57babf74029afaa6dda202afa47d3555cca7a0f20a22e466aeb7029e7d002207974b4c16f346811aff6720d09b9c58d0c4e01e8d258c3d203cc3c1ad228c61a012102fb6ad115761ea928f1367befb2bee79c0b3497314b45e0b734cd150f0601706c00000000");
}

TEST(GroestlcoinSigning, PlanP2WPKH) {
    Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeGroestlcoin);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(2500);
    input.set_byte_fee(1);
    input.set_to_address("31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P");
    input.set_change_address("Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");

    auto utxoKey0 = parse_hex("dc334e7347f2f9f72fce789b11832bdf78adf0158bc6617e6d2d2a530a0d4bc6");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Script(parse_hex("00147557920fbc32a1ef4ef26bae5e8ce3f95abf09ce"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(4774);
    auto hash0 = parse_hex("9568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f");
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeGroestlcoin);

    EXPECT_TRUE(verifyPlan(plan, {4774}, 2500, 145));
    EXPECT_EQ(plan.branch_id(), "");
}

} // namespace TW::Bitcoin
