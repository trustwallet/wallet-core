// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

TEST(VergeTransactionBuilder, BuildWithTime) {
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

    auto tx = Verge::TransactionBuilder::build<Verge::Transaction>(plan, input).payload();
    ASSERT_NE(tx.time, 0ul);
}