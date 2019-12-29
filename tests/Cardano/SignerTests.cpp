// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Signer.h"
#include "Cardano/Address.h"
#include "proto/Cardano.pb.h"

#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Cardano;
using namespace std;


const Data privateKey_b8c3 = parse_hex("b8c31abcc41d931ae881be11da9e4d9242b1f01cae4e69fa29d5ba1f89f9c1549ec844c6b39c70fa6d3a254fe57c1efee1a75eb9755e0b751e96dd288deabc881ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f");

// Helper to fill an utxo
void setUtxo(Proto::UnspentTransaction* utxo, const string& txid, int32_t index, uint64_t amount) {
    Data txidData = parse_hex(txid);
    utxo->mutable_out_point()->set_txid(txidData.data(), txidData.size());
    utxo->mutable_out_point()->set_index(index);
    utxo->set_amount(amount);
}

TEST(CardanoSigner, SignMessage) {
    // from cardano-crypto.js test
    auto privateKey = PrivateKey(
        parse_hex("d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"),
        parse_hex("d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"),
        parse_hex("69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000")
    );
    Data message = TW::data("Hello world");
    Data signature = privateKey.sign(message, TWCurveED25519Extended);
    EXPECT_EQ(
        "1096ddcfb2ad21a4c0d861ef3fabe18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d829bf0bcf1b631e86f0e",
        hex(signature)
    );
}

TEST(CardanoSigner, PlanAndSign) {
    // Plan and sign a transaction
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(167994, plan.fee());
    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d30f46ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f58403646fec783197bdf3d83858d8303f9d72e09a5df882202716c7737e55148c1c2106c550890c0ab001f7a27cb75fabe827f81132486bec41e17861aba1a069508",
        hex(output.encoded())
    );
    EXPECT_EQ("4374315285f5af17c5fd37d78bcaa94534968e925c06e293e10d2348d9420fac", output.transaction_id());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PlanTransaction) {
    // plan transaction, estimate fee
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    string txid = "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14";
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);

    EXPECT_EQ(1000000, plan.amount());
    EXPECT_EQ(167994, plan.fee());
    EXPECT_EQ(13832006, plan.change());
    ASSERT_EQ(1, plan.utxo_size());
    EXPECT_EQ(txid, hex(plan.utxo(0).out_point().txid()));
    EXPECT_EQ(6, plan.utxo(0).out_point().index());
    EXPECT_EQ(15000000, plan.utxo(0).amount());
    EXPECT_EQ("", plan.error());
}

TEST(CardanoSigner, PlanTransactionUseMax) {
    // plan transaction, with max available
    Proto::SigningInput input;
    input.set_amount(1000000); // not relevsant, use_max
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    string txid = "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14";
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    input.set_use_max_amount(true);
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);

    EXPECT_EQ(14834159, plan.amount());
    EXPECT_EQ(165841, plan.fee());
    EXPECT_EQ(0, plan.change());
    ASSERT_EQ(1, plan.utxo_size());
    EXPECT_EQ(txid, hex(plan.utxo(0).out_point().txid()));
    EXPECT_EQ(6, plan.utxo(0).out_point().index());
    EXPECT_EQ(15000000, plan.utxo(0).amount());
    EXPECT_EQ("", plan.error());
}

TEST(CardanoSigner, Plan3Input) {
    // plan a transaction, with 5 available inputs, and 3 chosen
    Proto::SigningInput input;
    input.set_amount((uint64_t)(25.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000001", 1, (uint64_t)(10.1 * 1000000));
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000002", 2, (uint64_t)(10.2 * 1000000));
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000003", 3, (uint64_t)(10.3 * 1000000));
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000004", 4, (uint64_t)(10.4 * 1000000));
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000004", 5, (uint64_t)(10.5 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);

    EXPECT_EQ(25000000, plan.amount());
    EXPECT_EQ(171685, plan.fee());
    EXPECT_EQ(5528315, plan.change());
    EXPECT_EQ(51500000, plan.available_amount());
    EXPECT_EQ("", plan.error());
    // 3 utxos selected
    ASSERT_EQ(3, plan.utxo_size());
    // check amounts, sum(utxo)
    uint64_t sumUtxo = 0;
    for(int i = 0; i < 3; ++i) { sumUtxo += plan.utxo(i).amount(); }
    EXPECT_EQ(30700000, sumUtxo);
    EXPECT_EQ(sumUtxo, plan.amount() + plan.fee() + plan.change());
}

TEST(CardanoSigner, SignTx_d498) {
    // real-world transaction with non-default fee
    // see this tx: https://cardanoexplorer.com/tx/d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());

    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    // set non-default fee, 1000000 + 169884 + 13830116 = 15000000
    EXPECT_EQ(167994, plan.fee());
    EXPECT_EQ(13832006, plan.change());
    EXPECT_EQ(15000000, plan.utxo(0).amount());
    plan.set_fee((uint64_t)(0.169884 * 1000000));
    plan.set_change(13830116);

    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d307e4ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f5840a60e06de3986d77c6f126d98c3107749ec13f8593a1d33e00e3c0c2ac5d2d41c3bcbd52afe2439b8afb816da2e6701fdd01092fa8d305f08b8fd41f249282709",
        hex(output.encoded())
    );
    EXPECT_EQ("d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f", output.transaction_id());
    EXPECT_EQ(169884, output.fee());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PrepareUnsignedTx_d498) {
    // see this tx: https://cardanoexplorer.com/tx/d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));

    Proto::TransactionPlan plan = Signer::planTransactionWithFee(input, 169884);
    EXPECT_EQ("", plan.error());
    Data unisgnedEncodedCborData = Signer::prepareUnsignedTx(input, plan);
    EXPECT_EQ(
        "839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d307e4ffa0",
        hex(unisgnedEncodedCborData)
    );

    Data txId = Hash::blake2b(unisgnedEncodedCborData, 32);
    EXPECT_EQ("d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f", hex(txId));

    PrivateKey fromPriKey = PrivateKey(privateKey_b8c3);
    PublicKey fromPubKey = fromPriKey.getPublicKey(TWPublicKeyTypeED25519Extended);
    EXPECT_EQ(
        "835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f",
        hex(fromPubKey.bytes)
    );
    Address fromAddr = Address(fromPubKey);
    EXPECT_EQ("Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx", fromAddr.string());
}

TEST(CardanoSigner, SignTx_8283) {
    // real transaction, with non-default fee
    // see this tx: https://cardanoexplorer.com/tx/ebf58670ee1512d597876fe632e92790bd70568374bdbe5a69c5d8ed107607ee
    Proto::SigningInput input;
    input.set_amount((uint64_t)(8.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEYxKHuuYNsYkpR64XNZz7Xm8vMep7mZ2rrP9HgqbbeX1uMxmGd");
    input.set_change_address("Ae2tdPwUPEYz548sTWdiTxBx13kxECHH4cmYtxQgPgEaQwmkymYFZzGkPrH");
    setUtxo(input.add_utxo(), "a49bad3f69bbab0e4d3e51991ce7a1116c0fd322a7731246b92df455e67e6861", 0, (uint64_t)(10.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());

    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    // set non-default fee, 8000000 + 169884 + 1830116 = 10000000
    EXPECT_EQ(167994, plan.fee());
    EXPECT_EQ(1832006, plan.change());
    EXPECT_EQ(10000000, plan.utxo(0).amount());
    plan.set_fee((uint64_t)(0.169884 * 1000000));
    plan.set_change(1830116);

    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d8185824825820a49bad3f69bbab0e4d3e51991ce7a1116c0fd322a7731246b92df455e67e686100ff9f8282d818582183581c0f1fdc075b3e562ff4c8dc6979b136aeedd087a7d3bf554f774b58eba0001abb7c9a121a007a12008282d818582183581c20b55a0bd5cbc39b58cdd8977e863aca7d387a5a0fc37a473eb72042a0001aa6fa2f8a1a001bece4ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f584045b3fe46b52c839566e697cbe72e248cd0f01e10c7c5696c8f202fe9776602b63c5878f0e27869d1d9ff69c9773211406970742f7cc316690b9d9133794e4f0e",
        hex(output.encoded())
    );
    EXPECT_EQ("ebf58670ee1512d597876fe632e92790bd70568374bdbe5a69c5d8ed107607ee", output.transaction_id());
    EXPECT_EQ(169884, output.fee());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, SignNoChange) {
    // if change is 0, do not include change output
    Proto::SigningInput input;
    input.set_amount((uint64_t)(14.832006 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(167994, plan.fee());
    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a00e25186ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f5840dfbfef76b1f790bf6f2d5ab4f3aab3086b962d26777c8637224dc039df9a95adc18e6a20d226fb072d1842dca1efe80fbb64907c16e239095f68b85d185f7b02",
        hex(output.encoded())
    );
    EXPECT_EQ("5c6b8768bb5af3c7a055bb79576d44d2ab8c1f483e77c62f0a9a054e189b6877", output.transaction_id());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PlanNegativeInvalidAddr) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("__INVALID_ADDRESS__");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ(0, plan.amount());
    EXPECT_EQ(0, plan.fee());
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("nvalid address") != string::npos);
}

TEST(CardanoSigner, PlanNegativeInsufficientBalance) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1000.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ(0, plan.amount());
    EXPECT_EQ(0, plan.fee());
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("nsufficient balance") != string::npos);
}

TEST(CardanoSigner, PlanNegativeNoPrivKey) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("enough private keys") != string::npos);
}

TEST(CardanoSigner, PlanNegativeNoUtxo) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("nsufficient balance") != string::npos);
}

TEST(CardanoSigner, PlanNegativeLowBalance) {
    // balance lower than feee
    Proto::SigningInput input;
    input.set_amount((uint64_t)(0.000001 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(0.000009 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("nsufficient balance") != string::npos);
}

TEST(CardanoSigner, SignNegativeEmptyPlan) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan emptyPlan;
    Proto::SigningOutput output = Signer::sign(input, emptyPlan);
    EXPECT_NE("", output.error());
    EXPECT_TRUE(output.error().find("ero fee") != string::npos);
}

TEST(CardanoSigner, SignNegativeAmountMismatch) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000));
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(167994, plan.fee());
    plan.set_fee(2000000);

    Proto::SigningOutput output = Signer::sign(input, plan);
    EXPECT_NE("", output.error());
    EXPECT_TRUE(output.error().find("mount mismatch") != string::npos);
}
