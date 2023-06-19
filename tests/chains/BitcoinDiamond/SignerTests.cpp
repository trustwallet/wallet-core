// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionSigner.h"
#include "BitcoinDiamond/Signer.h"
#include "BitcoinDiamond/TransactionBuilder.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"
#include "proto/Bitcoin.pb.h"

#include <TrustWalletCore/TWCoinType.h>
#include <gtest/gtest.h>

using namespace TW;
namespace TW::BitcoinDiamond::tests {

TEST(BitcoinDiamondSigner, Sign) {
    const int64_t amount = 17615;
    const int64_t fee = 10000;
    const std::string toAddress = "1HevQVTSEc8cEpDm65UpfCdj5erd4xxBhx";

    auto toScript = Bitcoin::Script::lockScriptForAddress(toAddress, TWCoinTypeBitcoinDiamond);
    ASSERT_EQ(hex(toScript.bytes), "76a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac");

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

    auto utxoKey0 =
        PrivateKey(parse_hex("d2b9f2846d3adcead910ee0124a3ba7ae29e8a4729787d27f9bea1f532928eee"));
    auto utxoAddr0 = TW::deriveAddress(TWCoinTypeBitcoinDiamond, utxoKey0);
    ASSERT_EQ(utxoAddr0, "1G15VvshDxwFTnahZZECJfFwEkq9fP79o8");
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, TWCoinTypeBitcoinDiamond);
    ASSERT_EQ(hex(script0.bytes), "76a914a48da46386ce52cccad178de900c71f06130c31088ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto plan = TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = 0;
    plan.preBlockHash =
        parse_hex("4bfa9d92e1e99e72597e1e9162bdaaab624f1bb6faa83b2f46c6777caf8d6699");
    std::reverse(plan.preBlockHash.begin(), plan.preBlockHash.end());

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto result = BitcoinDiamond::Signer::sign(input);

    ASSERT_EQ(
        hex(result.encoded()),
        "0c00000099668daf7c77c6462f3ba8fab61b4f62abaabd62911e7e59729ee9e1929dfa4b01034f4667301711e8"
        "a69236a93476ed798f9c11aaae472da5b315191a0453461d000000006a473044022078e0d3a9e1eb270ab02c15"
        "f8fcf1d3bfc95a324839690b7de4f011a4266132ff02204679e8103c4d3f0bb5192a5f53cc273732fd0e8392ab"
        "3b00dc708fd24d0160b3012102485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3"
        "ffffffff01cf440000000000001976a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac00000000");
}

TEST(BitcoinDiamondSigner, SignSegwit) {
    const int64_t amount = 17615;
    const int64_t fee = 10000;
    const std::string toAddress = "1HevQVTSEc8cEpDm65UpfCdj5erd4xxBhx";

    auto toScript = Bitcoin::Script::lockScriptForAddress(toAddress, TWCoinTypeBitcoinDiamond);
    ASSERT_EQ(hex(toScript.bytes), "76a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac");

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

    auto utxoKey0 =
        PrivateKey(parse_hex("d2b9f2846d3adcead910ee0124a3ba7ae29e8a4729787d27f9bea1f532928eee"));
    auto utxoAddr0 =
        TW::deriveAddress(TWCoinTypeBitcoinDiamond, utxoKey0, TWDerivationBitcoinSegwit);
    ASSERT_EQ(utxoAddr0, "bcd1q5jx6gcuxeefvejk30r0fqrr37psnpscslrrd0y");
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, TWCoinTypeBitcoinDiamond);
    ASSERT_EQ(hex(script0.bytes), "0014a48da46386ce52cccad178de900c71f06130c310");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto plan = TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = 0;
    plan.preBlockHash =
        parse_hex("4bfa9d92e1e99e72597e1e9162bdaaab624f1bb6faa83b2f46c6777caf8d6699");
    std::reverse(plan.preBlockHash.begin(), plan.preBlockHash.end());

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto result = BitcoinDiamond::Signer::sign(input);

    ASSERT_EQ(hex(result.encoded()),
              "0c00000099668daf7c77c6462f3ba8fab61b4f62abaabd62911e7e59729ee9e1929dfa4b000101034f46"
              "67301711e8a69236a93476ed798f9c11aaae472da5b315191a0453461d0000000000ffffffff01cf4400"
              "00000000001976a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac02483045022100e9cd497e"
              "a0156dbe72bd0e052869c8feb9bea182907023bcc447b98655a5e1080220767e85892df6e4c952bd62b1"
              "8c76317f623c5e4f1a8bf4cef6b5e1193e17cf6e012102485a209514cc896f8ed736e205bc4c35bd5299"
              "ef3f9e84054475336b964c02a300000000");
}

TEST(BitcoinDiamondSigner, SignAnyoneCanPay) {
    const int64_t amount = 17615;
    const int64_t fee = 10000;
    const std::string toAddress = "1HevQVTSEc8cEpDm65UpfCdj5erd4xxBhx";

    auto toScript = Bitcoin::Script::lockScriptForAddress(toAddress, TWCoinTypeBitcoinDiamond);
    ASSERT_EQ(hex(toScript.bytes), "76a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac");

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAnyoneCanPay | TWBitcoinSigHashTypeSingle);
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

    auto utxoKey0 =
        PrivateKey(parse_hex("d2b9f2846d3adcead910ee0124a3ba7ae29e8a4729787d27f9bea1f532928eee"));
    auto utxoAddr0 =
        TW::deriveAddress(TWCoinTypeBitcoinDiamond, utxoKey0, TWDerivationBitcoinSegwit);
    ASSERT_EQ(utxoAddr0, "bcd1q5jx6gcuxeefvejk30r0fqrr37psnpscslrrd0y");
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, TWCoinTypeBitcoinDiamond);
    ASSERT_EQ(hex(script0.bytes), "0014a48da46386ce52cccad178de900c71f06130c310");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto plan = TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = 0;
    plan.preBlockHash =
        parse_hex("4bfa9d92e1e99e72597e1e9162bdaaab624f1bb6faa83b2f46c6777caf8d6699");
    std::reverse(plan.preBlockHash.begin(), plan.preBlockHash.end());

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto result = BitcoinDiamond::Signer::sign(input);

    ASSERT_EQ(hex(result.encoded()),
              "0c00000099668daf7c77c6462f3ba8fab61b4f62abaabd62911e7e59729ee9e1929dfa4b000101034f46"
              "67301711e8a69236a93476ed798f9c11aaae472da5b315191a0453461d0000000000ffffffff01cf4400"
              "00000000001976a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac02483045022100d659b1e0"
              "8f30e133658eca9d97158723b49658bbe28930e361fa274bd11a0b090220587436eaaf3b9397d14af18f"
              "a8b4c77a7d7d51bc4733a2821bf03865704966d7832102485a209514cc896f8ed736e205bc4c35bd5299"
              "ef3f9e84054475336b964c02a300000000");
}

TEST(BitcoinDiamondSigner, SignWithError) {
    const int64_t amount = 17615;
    const std::string toAddress = "1HevQVTSEc8cEpDm65UpfCdj5erd4xxBhx";

    auto toScript = Bitcoin::Script::lockScriptForAddress(toAddress, TWCoinTypeBitcoinDiamond);
    ASSERT_EQ(hex(toScript.bytes), "76a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac");

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("1G15VvshDxwFTnahZZECJfFwEkq9fP79o8");
    input.set_coin_type(TWCoinTypeBitcoinDiamond);

    auto inputData = data(input.SerializeAsString());

    // PreImageHash
    auto preData = TransactionCompiler::preImageHashes(TWCoinTypeBitcoinDiamond, inputData);

    TW::Bitcoin::Proto::PreSigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(preData.data(), (int)preData.size()));

    ASSERT_NE(output.error(), Common::Proto::OK);

    // Sign
    auto result = BitcoinDiamond::Signer::sign(input);
    ASSERT_NE(result.error(), Common::Proto::OK);
}

} // namespace TW::BitcoinDiamond::tests
