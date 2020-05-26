// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Bitcoin.pb.h"
#include "../interface/TWTestUtilities.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(GroestlcoinSigning, SignP2WPKH) {
    Proto::SigningInput input;
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

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeGroestlcoin);
    ASSERT_EQ(hex(output.encoded()), "010000000001019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f0100000000ffffffff02c40900000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48751080000000000001976a91498af0aaca388a7e1024f505c033626d908e3b54a88ac0247304402202d879d7b1ab767a0efcfa4bc4e813a4178184b507d204fba2af133fce304949302200cde754af4e6b4b54b8e8a2bd09f55ba03b49a28554e9d3f9de1581db245ccca012102e9c9b9b76e982ad8fa9a7f48470eafbeeba9bf6d287579318c517db5157d936e00000000");
}

TEST(GroestlcoinSigning, SignP2PKH) {
    Proto::SigningInput input;
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

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeGroestlcoin);
    ASSERT_EQ(hex(output.encoded()), "01000000019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f000000006a47304402204c270caaa29410e78d65c6a9e8995eb96ad5b50cda83a6900253b1bfcc15681302201b81c5deb524d9ef757c8acbd226b15aedde0b765278a7386210c6b59593f4d2012103b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91ffffffff02c4090000000000001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09cee70800000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48700000000");
}

TEST(GroestlcoinSigning, SignP2SH_P2WPKH) {
    // TX outputs
    Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(5'000);
    input.set_byte_fee(1);
    input.set_to_address("Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");
    input.set_change_address("grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne");

    // TX input
    auto utxoKey0 = PrivateKey(parse_hex("302fc195a8fc96c5a581471e67e4c1ac2efda252f76ad5c77a53764c70d58f91"));
    auto pubKey0 = utxoKey0.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto utxoPubkeyHash = Hash::ripemd(Hash::sha256(pubKey0.bytes));
    EXPECT_EQ(hex(utxoPubkeyHash.begin(), utxoPubkeyHash.end()), "2fc7d70acef142d1f7b5ef2f20b1a9b759797674");
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto redeemScript = Script::buildPayToWitnessPublicKeyHash(utxoPubkeyHash);
    auto scriptHash = Hash::ripemd(Hash::sha256(redeemScript.bytes));
    ASSERT_EQ(hex(scriptHash.begin(), scriptHash.end()), "0055b0c94df477ee6b9f75185dfc9aa8ce2e52e4");
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[hex(scriptHash.begin(), scriptHash.end())] = scriptString;

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Script(parse_hex("a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e487"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(10'000);
    auto hash0 = DATA("fdae0772d7d1d33804a6b1ca0e391668b116bb7a70028427d3d82232189ce863"); // UTXO hash backwards
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeGroestlcoin);

    ASSERT_EQ(hex(output.encoded()), "01000000000101fdae0772d7d1d33804a6b1ca0e391668b116bb7a70028427d3d82232189ce86300000000171600142fc7d70acef142d1f7b5ef2f20b1a9b759797674ffffffff0288130000000000001976a91498af0aaca388a7e1024f505c033626d908e3b54a88ace1120000000000001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09ce0247304402205737621d7f1a9e0174a00583d23f364a1724d9820685956be36979404edf8ec1022032062f7f85f41c755de90bbc8ba60a276a6a2b02b825c0dd72edb9ef8eeb5dd1012102fb6ad115761ea928f1367befb2bee79c0b3497314b45e0b734cd150f0601706c00000000");
}

TEST(GroestlcoinSigning, PlanP2WPKH) {
    Proto::SigningInput input;
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

    EXPECT_EQ(plan.amount(), 2500);
    EXPECT_EQ(plan.available_amount(), 4774);
    EXPECT_EQ(plan.fee(), 145);
    EXPECT_EQ(plan.change(), 2129);
    EXPECT_EQ(plan.utxos_size(), 1);
    EXPECT_EQ(plan.branch_id(), "");
}
