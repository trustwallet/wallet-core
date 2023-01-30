// Copyright © 2017-2022 Trust Wallet.
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
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWAnySignerZen, Sign) {
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

    Bitcoin::Proto::TransactionPlan plan;
    {
        ANY_PLAN(input, plan, TWCoinTypeZen);

        ASSERT_EQ(plan.fee(), 226);

        plan.set_preblockhash(blockHash.data(), (int)blockHash.size());
        plan.set_preblockheight(blockHeight);

        *input.mutable_plan() = plan;
    }

    Bitcoin::Proto::SigningOutput output;
    {
        ANY_SIGN(input, TWCoinTypeZen);
        ASSERT_EQ(output.error(), Common::Proto::OK);
    }

    // Sign
    ASSERT_EQ(hex(output.encoded()),
        "0100000001a39e13b5ab406547e31284cd96fb40ed271813939c195ae7a86cd67fb8a4de62000000006a473044022014d687c0bee0b7b584db2eecbbf73b545ee255c42b8edf0944665df3fa882cfe02203bce2412d93c5a56cb4806ddd8297ff05f8fc121306e870bae33377a58a02f05012102b4ac9056d20c52ac11b0d7e83715dd3eac851cfc9cb64b8546d9ea0d4bb3bdfeffffffff0210270000000000003f76a914a58d22659b1082d1fa8698fc51996b43281bfce788ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b4ce1c0000000000003f76a914cf83669620de8bbdf2cefcdc5b5113195603c56588ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b400000000"
    );
}
