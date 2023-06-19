// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Common.pb.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;


TEST(TWAnySignerBitcoinDiamond, Sign) {
    const int64_t amount = 17615;
    const int64_t fee = 10000;
    const std::string toAddress = "1HevQVTSEc8cEpDm65UpfCdj5erd4xxBhx";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("1G15VvshDxwFTnahZZECJfFwEkq9fP79o8");
    input.set_coin_type(TWCoinTypeBitcoinDiamond);

    auto txHash0 = parse_hex("1d4653041a1915b3a52d47aeaa119c8f79ed7634a93692a6e811173067464f03");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(27615);

    auto script0 = parse_hex("76a914a48da46386ce52cccad178de900c71f06130c31088ac");
    utxo0->set_script(script0.data(), (int)script0.size());

    auto utxoKey0 = PrivateKey(parse_hex("d2b9f2846d3adcead910ee0124a3ba7ae29e8a4729787d27f9bea1f532928eee"));
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    Bitcoin::Proto::TransactionPlan plan;
    {
        ANY_PLAN(input, plan, TWCoinTypeBitcoinDiamond);

        plan.set_amount(amount);
        plan.set_fee(fee);
        plan.set_change(0);
        auto preBlockHash = parse_hex("4bfa9d92e1e99e72597e1e9162bdaaab624f1bb6faa83b2f46c6777caf8d6699");
        std::reverse(preBlockHash.begin(), preBlockHash.end());
        plan.set_preblockhash(preBlockHash.data(), (int)preBlockHash.size());
    }
    
    *input.mutable_plan() = plan;

    Bitcoin::Proto::SigningOutput output;
    {
        ANY_SIGN(input, TWCoinTypeBitcoinDiamond);
        ASSERT_EQ(output.error(), Common::Proto::OK);
    }
    // Sign
    ASSERT_EQ(hex(output.encoded()), "0c00000099668daf7c77c6462f3ba8fab61b4f62abaabd62911e7e59729ee9e1929dfa4b01034f4667301711e8a69236a93476ed798f9c11aaae472da5b315191a0453461d000000006a473044022078e0d3a9e1eb270ab02c15f8fcf1d3bfc95a324839690b7de4f011a4266132ff02204679e8103c4d3f0bb5192a5f53cc273732fd0e8392ab3b00dc708fd24d0160b3012102485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3ffffffff01cf440000000000001976a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac00000000");
}
