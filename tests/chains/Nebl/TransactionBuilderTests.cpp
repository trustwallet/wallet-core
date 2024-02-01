// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Verge/Transaction.h"
#include "Verge/TransactionBuilder.h"
#include "Bitcoin/TransactionPlan.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(NeblTransactionBuilder, BuildWithTime) {
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

    auto tx = Verge::TransactionBuilder::build<Verge::Transaction>(plan, input).payload();
    ASSERT_NE(tx.time, 0ul);
}