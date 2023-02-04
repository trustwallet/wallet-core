// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

#include "Bitcoin/Signer.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(TWAnySignerNebl, Sign) {
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

    Bitcoin::Proto::TransactionPlan plan;
    {
        ANY_PLAN(input, plan, TWCoinTypeNebl);

        plan.set_amount(amount);
        plan.set_fee(fee);
        plan.set_change(980000000);

        *input.mutable_plan() = plan;
    }

    Bitcoin::Proto::SigningOutput output;
    {
        ANY_SIGN(input, TWCoinTypeNebl);
        ASSERT_EQ(output.error(), Common::Proto::OK);
    }

    // Sign
    ASSERT_EQ(hex(output.encoded()),
        "01000000d4cbbb62017d40b2c80adbb0f44cbec466ae30f0316e0c1bf98110fd6d3f1b0fda47e1a6a5000000006a47304402207f77f7ed50ec56447fd108b2a9a693b2ac9f62e99b59dfa914f242510943187602204618fd9195050c763eb93644e51344f6c00e4dd93aa41bb42bce42c9e4cc53b6012103787a4c5ff72dce6d97f9b6360dc302b2d8a833e8c570dcc124a96e5f564bb524feffffff02002f6859000000001976a914412033ed457c72ca70bab5fbfdc03256bd2ce07d88ac009d693a000000001976a914ae40b2142aba5ddd10f74d9440bfda8a36cbad5b88ac00000000"
    );
}
