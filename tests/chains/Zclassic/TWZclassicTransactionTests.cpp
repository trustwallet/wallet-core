// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Zclassic/Transaction.h"
#include "Zclassic/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "Data.h"
#include "Coin.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <gtest/gtest.h>

using namespace TW;

TEST(TWZclassicTransaction, BasicSigning) {
    const int64_t amount = 488000;
    const int64_t fee = 6000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_coin_type(TWCoinTypeZclassic);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS");

    auto hash0 = DATA("53685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(494000);
    auto script0 = parse_hex("76a914f84c7f4dd3c3dc311676444fdead6e6d290d50e388ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    auto plan = Zclassic::TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = 0;

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    auto result = Bitcoin::TransactionSigner<Zclassic::Transaction, Zclassic::TransactionBuilder>::sign(input);
    ASSERT_TRUE(result) << std::to_string(result.error());
    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(serialized);
    ASSERT_GT(serialized.size(), 0);
}
