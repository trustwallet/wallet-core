// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Bitcoin/Transaction.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionSigner.h"
#include "Zen/Address.h"
#include "Zen/Signer.h"
#include "Zen/TransactionBuilder.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(ZenTransactionBuilder, Build) {
    const int64_t amount = 10000;
    const std::string toAddress = "zngBGZGKaaBamofSuFw5WMnvU2HQAtwGeb5";

    auto blockHash = parse_hex("0000000004561422697a29d424d925334db5ef2e80232306a1ad3fd35f72dc81");
    std::reverse(blockHash.begin(), blockHash.end());
    auto blockHeight = 1147624;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg");
    input.set_coin_type(TWCoinTypeZen);

    Data opScript = parse_hex("00010203");
    input.set_output_op_return(std::string(opScript.begin(), opScript.end()));

    auto eo = input.add_extra_outputs();
    eo->set_to_address("znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg");
    eo->set_amount(7000);

    auto txHash0 = parse_hex("62dea4b87fd66ca8e75a199c93131827ed40fb96cd8412e3476540abb5139ea3");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(17600);

    auto utxoKey0 = PrivateKey(parse_hex("3a8e0a528f62f4ca2c77744c8a571def2845079b50105a9f7ef6b1b823def67a"));
    auto utxoAddr0 = TW::deriveAddress(TWCoinTypeZen, utxoKey0);
    ASSERT_EQ(utxoAddr0, "znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg");
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, TWCoinTypeZen, blockHash, blockHeight);
    ASSERT_EQ(hex(script0.bytes), "76a914cf83669620de8bbdf2cefcdc5b5113195603c56588ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b4");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto plan = Bitcoin::TransactionSigner<Bitcoin::Transaction, Zen::TransactionBuilder>::plan(input);
    ASSERT_EQ(plan.fee, 294);
    plan.preBlockHash = blockHash;
    plan.preBlockHeight = blockHeight;

    // plan1
    auto result = Zen::TransactionBuilder::build<Bitcoin::Transaction>(plan, input).payload();
  
    ASSERT_GT(result.outputs.size(), 0ul);
    ASSERT_EQ(result.outputs[0].value, plan.amount);

    // plan2
    result = Zen::TransactionBuilder::build<Bitcoin::Transaction>(plan, input).payload();
  
    ASSERT_EQ(result.outputs.size(), 4ul);
    ASSERT_EQ(result.outputs[3].value, 7000);
}

TEST(ZenTransactionBuilder, BuildScript) {
    auto blockHash = parse_hex("0000000004561422697a29d424d925334db5ef2e80232306a1ad3fd35f72dc81");
    std::reverse(blockHash.begin(), blockHash.end());
    auto blockHeight = 1147624;

    // invalid address
    auto result = Zen::TransactionBuilder::prepareOutputWithScript(
        "DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E",
        10000, TWCoinTypeZen, blockHash, blockHeight);
    ASSERT_FALSE(result.has_value());
}