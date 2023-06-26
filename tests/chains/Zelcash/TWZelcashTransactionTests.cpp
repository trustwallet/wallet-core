
// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include "Bitcoin/OutPoint.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "Zcash/Transaction.h"
#include "proto/Bitcoin.pb.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWZelcashTransaction, Encode) {
	// We are using Zcash test because it is also the encode used by zelcash
    // Test vector 3 https://github.com/zcash/zips/blob/master/zip-0243.rst
    auto transaction = Zcash::Transaction();
    transaction.lockTime = 0x0004b029;
    transaction.expiryHeight = 0x0004b048;
    transaction.branchId = Zcash::SaplingBranchID;

    auto outpoint0 = Bitcoin::OutPoint(parse_hex("a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9"), 1);
    transaction.inputs.emplace_back(outpoint0, Bitcoin::Script(parse_hex("483045022100a61e5d557568c2ddc1d9b03a7173c6ce7c996c4daecab007ac8f34bee01e6b9702204d38fdc0bcf2728a69fde78462a10fb45a9baa27873e6a5fc45fb5c76764202a01210365ffea3efa3908918a8b8627724af852fc9b86d7375b103ab0543cf418bcaa7f")), 0xfffffffe);

    auto script0 = Bitcoin::Script(parse_hex("76a9148132712c3ff19f3a151234616777420a6d7ef22688ac"));
    transaction.outputs.emplace_back(0x02625a00, script0);

    auto script1 = Bitcoin::Script(parse_hex("76a9145453e4698f02a38abdaa521cd1ff2dee6fac187188ac"));
    transaction.outputs.emplace_back(0x0098958b, script1);

    auto unsignedData = Data{};
    transaction.encode(unsignedData);

    ASSERT_EQ(hex(unsignedData),
        /* header */          "04000080"
        /* versionGroupId */  "85202f89"
        /* vin */             "01""a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9""01000000""6b483045022100a61e5d557568c2ddc1d9b03a7173c6ce7c996c4daecab007ac8f34bee01e6b9702204d38fdc0bcf2728a69fde78462a10fb45a9baa27873e6a5fc45fb5c76764202a01210365ffea3efa3908918a8b8627724af852fc9b86d7375b103ab0543cf418bcaa7f""feffffff"
        /* vout */            "02""005a620200000000""1976a9148132712c3ff19f3a151234616777420a6d7ef22688ac"
                                  "8b95980000000000""1976a9145453e4698f02a38abdaa521cd1ff2dee6fac187188ac"
        /* lockTime */        "29b00400"
        /* expiryHeight */    "48b00400"
        /* valueBalance */    "0000000000000000"
        /* vShieldedSpend */  "00"
        /* vShieldedOutput */ "00"
        /* vJoinSplit */      "00"
    );

    auto scriptCode = Bitcoin::Script(parse_hex("76a914507173527b4c3318a2aecd793bf1cfed705950cf88ac"));
    auto preImage = transaction.getPreImage(scriptCode, 0, TWBitcoinSigHashTypeAll, 0x02faf080);
    ASSERT_EQ(hex(preImage),
        /* header */              "04000080"
        /* versionGroupId */      "85202f89"
        /* hashPrevouts */        "fae31b8dec7b0b77e2c8d6b6eb0e7e4e55abc6574c26dd44464d9408a8e33f11"
        /* hashSequence */        "6c80d37f12d89b6f17ff198723e7db1247c4811d1a695d74d930f99e98418790"
        /* hashOutputs */         "d2b04118469b7810a0d1cc59568320aad25a84f407ecac40b4f605a4e6868454"
        /* hashJoinSplits */      "0000000000000000000000000000000000000000000000000000000000000000"
        /* hashShieldedSpends */  "0000000000000000000000000000000000000000000000000000000000000000"
        /* hashShieldedOutputs */ "0000000000000000000000000000000000000000000000000000000000000000"
        /* lockTime */            "29b00400"
        /* expiryHeight */        "48b00400"
        /* valueBalance */        "0000000000000000"
        /* hashType */            "01000000"
        /* prevout */             "a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9""01000000"
        /* scriptCode */          "1976a914507173527b4c3318a2aecd793bf1cfed705950cf88ac"
        /* amount */              "80f0fa0200000000"
        /* sequence */            "feffffff"
    );

    auto sighash = transaction.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAll, 0x02faf080, Bitcoin::BASE);
    ASSERT_EQ(hex(sighash), "f3148f80dfab5e573d5edfe7a850f5fd39234f80b5429d3a57edcc11e34c585b");
}

TEST(TWZelcashTransaction, Signing) {
    // tx on mainnet
    // https://explorer.zel.zelcore.io/tx/ac5e4683ca4859daea1e91302f43e76a12d60c3e5fa955a55ee8629260655ddf
    const int64_t amount = 144995480;
    const int64_t fee = 2260;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_coin_type(TWCoinTypeZelcash);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("t1UPSwfMYLe18ezbCqnR5QgdJGznzCUYHkj");

    auto hash0 = DATA("5e2d98ab1f4a2bf089c6d850b876d479e2bc1716d908cce66c962aa887118815");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(144997740);
    auto script0 = parse_hex("76a9140170e9c060b0609774ac76144d56f59e4a15066988ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("eda043f40029e67edc6e9edba61f47795e03ad57169074ac81e898c04cc45b29");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    auto branchId = Data(Zcash::SaplingBranchID.begin(), Zcash::SaplingBranchID.end());

    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeZelcash);
    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(0);
    plan.set_branch_id(branchId.data(), branchId.size());

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan;

    Bitcoin::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeZelcash);

    ASSERT_EQ(output.error(), Common::Proto::OK);
    ASSERT_EQ(hex(output.encoded()),
        "04000080"
        "85202f89"
        "01"
            "5e2d98ab1f4a2bf089c6d850b876d479e2bc1716d908cce66c962aa887118815""00000000""6a473044022002f181eeeab108a94a99a5aa0f4aaaf2236f8ea8c0db39ebfdcff8511f55df2102206c67f325099ca5b0a82e7d5d2b5b600cf4e3c8aec90ae7655b89e14282319a6a012103eeb334e219744f26ce6bffbdc2122bc13060606a2f696a5891fb101a82c80551""ffffffff"
        "01"
            "9874a40800000000""1976a91473562bc6a1db9dc6effebc1ef4379942feb3cf2c88ac"
        "00000000"
        "00000000"
        "0000000000000000"
        "00"
        "00"
        "00"
    );
}
