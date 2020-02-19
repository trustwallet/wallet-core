// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Decred/Address.h"
#include "Decred/Signer.h"
#include "Decred/Transaction.h"
#include "Decred/OutPoint.h"

#include "proto/Bitcoin.pb.h"
#include "proto/Decred.pb.h"

#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <Bitcoin/OpCodes.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Decred;

const auto privateKeyStr = "22a47fa09a223f2aa079edf85a7c2d4f8720ee63e502ee2869afab7de234b80c";

TEST(DecredSigner, Sign) {
    const auto privateKey = PrivateKey(parse_hex(privateKeyStr));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto keyhash = Hash::ripemd(Hash::blake256(publicKey.bytes));

    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH");


    // For this example, create a fake transaction that represents what would
    // ordinarily be the real transaction that is being spent. It contains a
    // single output that pays to address in the amount of 1 DCR.
    auto originTx = Transaction();

    auto txInOrigin = TransactionInput();
    txInOrigin.previousOutput = OutPoint(std::array<byte, 32>{}, UINT32_MAX, 0);
    txInOrigin.valueIn = 100'000'000;
    txInOrigin.script = Bitcoin::Script(Data{OP_0, OP_0});
    originTx.inputs.push_back(txInOrigin);

    auto txOutOrigin = TransactionOutput();
    txOutOrigin.value = 100'000'000;
    txOutOrigin.script = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    originTx.outputs.push_back(txOutOrigin);

    ASSERT_EQ(hex(originTx.hash()), "0ff6ff7c6774a56ccc51598b11724c9c441cadc52978ddb5f08f3511a0cc777a");


    // Setup input
    Bitcoin::Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(100'000'000);
    input.set_byte_fee(1);
    input.set_to_address("DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH");
    input.set_change_address("DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH");

    auto utxoKey0 = parse_hex("22a47fa09a223f2aa079edf85a7c2d4f8720ee63e502ee2869afab7de234b80c");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(100'000'000);
    utxo0->mutable_out_point()->set_hash(originTx.hash().data(), 32);
    utxo0->mutable_out_point()->set_index(0);


	// Create the transaction to redeem the fake transaction.
    auto redeemTx = Transaction();

    auto txIn = TransactionInput();
    txIn.previousOutput = OutPoint(originTx.hash(), 0, 0);
    txIn.valueIn = 100'000'000;
    redeemTx.inputs.push_back(txIn);

    auto txOut = TransactionOutput();
    redeemTx.outputs.push_back(txOut);


    // Sign
    auto signer = Signer(std::move(input));
    signer.transaction = redeemTx;
    signer.txPlan.utxos.push_back(*utxo0);
    signer.txPlan.amount = 100'000'000;
    const auto result = signer.sign();

    ASSERT_TRUE(result);

    const auto expectedSignature = "47304402201ac7bdf56a9d12f3bc09cf7b47cdfafc1348628f659e37b455d497cb6e7a748802202b3630eedee1bbc9248424e4a1b8671e14631a069f36ac8860dee0bb9ea1541f0121"
        "02a673638cb9587cb68ea08dbef685c6f2d2a751a8b3c6f2a7e9a4999e6e4bfaf5";
    EXPECT_EQ(hex(result.payload().inputs[0].script.bytes), expectedSignature);

    const auto expectedEncoded =
        "0100" // Serialize type
        "0000" // Version
        "01"   // Inputs
            "0ff6ff7c6774a56ccc51598b11724c9c441cadc52978ddb5f08f3511a0cc777a" // Hash
            "00000000" // Index
            "00" // Tree
            "ffffffff" // Sequence
        "01"   // Outputs
            "0000000000000000" // Value
            "0000" // Version
            "00" // Script
        "00000000" // Lock time
        "00000000" // Expiry
        "01"
            "00e1f50500000000" // Value
            "00000000" // Block height
            "ffffffff" // Block index
            "6a47304402201ac7bdf56a9d12f3bc09cf7b47cdfafc1348628f659e37b455d497cb6e7a748802202b3630eedee1bbc9248424e4a1b8671e14631a069f36ac8860dee0bb9ea1541f012102a673638cb9587cb68ea08dbef685c6f2d2a751a8b3c6f2a7e9a4999e6e4bfaf5";
    auto encoded = Data();
    result.payload().encode(encoded);
    EXPECT_EQ(hex(encoded), expectedEncoded);
}

TEST(DecredSigner, Plan) {
    const auto privateKey = PrivateKey(parse_hex(privateKeyStr));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto keyhash = Hash::ripemd(Hash::blake256(publicKey.bytes));
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH");

    auto originTx = Transaction();

    auto txInOrigin = TransactionInput();
    txInOrigin.previousOutput = OutPoint(std::array<byte, 32>{}, UINT32_MAX, 0);
    txInOrigin.valueIn = 100'000'000;
    txInOrigin.script = Bitcoin::Script(Data{OP_0, OP_0});
    originTx.inputs.push_back(txInOrigin);

    auto txOutOrigin = TransactionOutput();
    txOutOrigin.value = 100'000'000;
    txOutOrigin.script = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    originTx.outputs.push_back(txOutOrigin);

    EXPECT_EQ(hex(originTx.hash()), "0ff6ff7c6774a56ccc51598b11724c9c441cadc52978ddb5f08f3511a0cc777a");

    // Setup input
    Bitcoin::Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(40'000'000);
    input.set_byte_fee(1);
    input.set_to_address("DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH");
    input.set_change_address("DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH");

    auto utxoKey0 = parse_hex(privateKeyStr);
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(100'000'000);
    utxo0->mutable_out_point()->set_hash(originTx.hash().data(), 32);
    utxo0->mutable_out_point()->set_index(0);

	// Create the transaction to redeem the fake transaction.
    auto redeemTx = Transaction();

    auto txIn = TransactionInput();
    txIn.previousOutput = OutPoint(originTx.hash(), 0, 0);
    txIn.valueIn = 100'000'000;
    redeemTx.inputs.push_back(txIn);

    auto txOut = TransactionOutput();
    redeemTx.outputs.push_back(txOut);

    // Plan
    const auto plan = Signer::plan(input);

    EXPECT_EQ(plan.amount(), 40'000'000);
    EXPECT_EQ(plan.available_amount() , 100'000'000);
    EXPECT_EQ(plan.fee() , 226);
    EXPECT_EQ(plan.change() , 59'999'774);
    ASSERT_EQ(plan.utxos_size() , 1);
    EXPECT_EQ(hex(plan.utxos(0).out_point().hash()), "0ff6ff7c6774a56ccc51598b11724c9c441cadc52978ddb5f08f3511a0cc777a");
    EXPECT_EQ(plan.utxos(0).out_point().index(), 0);
    EXPECT_EQ(plan.utxos(0).out_point().sequence(), 0);
    EXPECT_EQ(hex(plan.utxos(0).script()), "76a914f5eba6730a4052ddeef0a93d93d24004f49db51e88ac");
    EXPECT_EQ(plan.utxos(0).amount() , 100'000'000);
    EXPECT_EQ(plan.branch_id() , "");
}

TEST(DecredSigner, SignP2WPKH) {
    // Build transaction
    auto unsignedTx = Transaction();
    unsignedTx.version = 1;
    unsignedTx.lockTime = 0x11;

    auto hash1 = parse_hex("fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f");
    auto outpoint1 = OutPoint(hash1, 0, 0);
    auto txInput1 = TransactionInput();
    txInput1.previousOutput = outpoint1;
    txInput1.sequence = 0xffffffee;
    unsignedTx.inputs.emplace_back(txInput1);

    auto hash2 = parse_hex("ef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a");
    auto outpoint2 = OutPoint(hash2, 1, 0);
    auto txInput2 = TransactionInput();
    txInput2.previousOutput = outpoint2;
    txInput2.sequence = UINT32_MAX;
    unsignedTx.inputs.emplace_back(txInput2);

    auto outScript0 = Bitcoin::Script(parse_hex("76a9148280b37df378db99f66f85c95a783a76ac7a6d5988ac"));
    unsignedTx.outputs.emplace_back(112'340'000, 0, outScript0);

    auto outScript1 = Bitcoin::Script(parse_hex("76a9143bde42dbee7e4dbe6a21b2d50ce2f0167faa815988ac"));
    unsignedTx.outputs.emplace_back(223'450'000, 0, outScript1);

    // Setup input
    Bitcoin::Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(335'790'000);
    input.set_byte_fee(1);
    input.set_to_address("DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH");
    input.set_change_address("DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH");

    auto utxoKey0 = parse_hex("bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxoKey1 = parse_hex("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9");
    input.add_private_key(utxoKey1.data(), utxoKey1.size());

    auto scriptPub1 = Bitcoin::Script(parse_hex("00141d0f172a0ecb48aee1be1f2687d2963ae33f71a1"));
    auto scriptHash = std::vector<uint8_t>();
    scriptPub1.matchPayToWitnessPublicKeyHash(scriptHash);
    auto scriptHashHex = hex(scriptHash.begin(), scriptHash.end());
    EXPECT_EQ(scriptHashHex, "1d0f172a0ecb48aee1be1f2687d2963ae33f71a1");

    auto redeemScript = Bitcoin::Script::buildPayToPublicKeyHash(scriptHash);
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[scriptHashHex] = scriptString;

    auto utxo0 = input.add_utxo();
    auto utxo0Script = parse_hex("2103c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432ac");
    utxo0->set_script(utxo0Script.data(), utxo0Script.size());
    utxo0->set_amount(625'000'000);
    utxo0->mutable_out_point()->set_hash(hash1.data(), hash1.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    auto utxo1 = input.add_utxo();
    auto utxo1Script = parse_hex("00141d0f172a0ecb48aee1be1f2687d2963ae33f71a1");
    utxo1->set_script(utxo0Script.data(), utxo0Script.size());
    utxo1->set_amount(600'000'000);
    utxo1->mutable_out_point()->set_hash(hash1.data(), hash1.size());
    utxo1->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    // Sign
    auto result = Signer::sign(std::move(input));

    auto signedTx = result.encoded();
    // txid = "03b30d55430f08365d19a62d3bd32e459ab50984fbcf22921ecc85f1e09dc6ed"
    // witid = "20bc58d07d91a3bae9e6f4d617d8f6271723d1a7673e486cc0ecaf9e758e2c22"
    EXPECT_EQ(
        hex(signedTx),
        "0100000001fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000000ffffffff02b0bf03140000000000001976a914f5eba6730a4052ddeef0a93d93d24004f49db51e88acaefd3c110000000000001976a914f5eba6730a4052ddeef0a93d93d24004f49db51e88ac000000000000000001000000000000000000000000ffffffff49483045022100ddb62cb3f7d626115291942b2637527ccf25530547ee814b56a50bd33b3c35da02202543eaa8deddd165e361105b68f8d71a945c4ea9bd9ca44ac80912cbd6713b7201"
    );
    EXPECT_EQ(result.transaction_id(), "966037bee2ade19fc1f1a56ee995d07f59a9969d4ed338136ae168e4ae27e975");
    EXPECT_EQ(result.error(), "");
}
