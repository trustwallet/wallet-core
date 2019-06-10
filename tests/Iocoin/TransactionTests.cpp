// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Iocoin/Address.h"
#include "Iocoin/Transaction.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Iocoin.pb.h"

#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Iocoin;

TEST(IocoinTransaction, SigHash) {
    const auto privateKey = PrivateKey(parse_hex("24f1c568d3ddd0a770d81c8562b250e108388f65b6dfc73a71a4edaa9bacc9b5"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto keyhash = Hash::ripemd(Hash::sha256(publicKey.bytes));

    const auto address = TW::Iocoin::Address(publicKey, 0x67);
    ASSERT_EQ(address.string(), "inTdSfZRVkmGYVLF14PpSKFKP5QR3tX1Qr"); 

    auto tx = Transaction();

    Bitcoin::OutPoint previousOutput = Bitcoin::OutPoint(std::array<byte, 32>{}, UINT32_MAX);

    Proto::UnspentTransaction utxo;
    utxo.set_amount(100000000);
    utxo.mutable_out_point()->set_hash(previousOutput.hash.data(), previousOutput.hash.size());
    utxo.mutable_out_point()->set_index(previousOutput.index);
    auto script = Bitcoin::Script(utxo.script().begin(), utxo.script().end());

    auto txIn = TransactionInput(previousOutput, script);
    tx.inputs.push_back(txIn);

    auto txOutOrigin = Bitcoin::TransactionOutput();
    txOutOrigin.value = 100000000;
    txOutOrigin.script = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    tx.outputs.push_back(txOutOrigin);

    Data data; 
    tx.encode(data);
    ASSERT_EQ(hex(data), "0100000000000000010000000000000000000000000000000000000000000000000000000000000000ffffffff00ffffffff0100e1f505000000001976a914d782027fe48fa405263dff9f7920d9a14357801b88ac00000000");

    auto sighash = tx.getSignatureHash(script, 0, TWSignatureHashTypeAll); 
    ASSERT_EQ(hex(sighash), "f47e1b35ccc286e9c5a5c975b10af823de487002a2b9032df4a6c75aa61d799a");
}
TEST(IocoinTransaction, SerializeInput1) {
    const auto privateKey = PrivateKey(parse_hex("24f1c568d3ddd0a770d81c8562b250e108388f65b6dfc73a71a4edaa9bacc9b5"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto keyhash = Hash::ripemd(Hash::sha256(publicKey.bytes));

    const auto address = TW::Iocoin::Address(publicKey, 0x67);
    ASSERT_EQ(address.string(), "inTdSfZRVkmGYVLF14PpSKFKP5QR3tX1Qr"); 

    auto tx = Transaction();

    Bitcoin::OutPoint previousOutput = Bitcoin::OutPoint(std::array<byte, 32>{}, UINT32_MAX);

    Proto::UnspentTransaction utxo;
    utxo.set_amount(100000000);
    utxo.mutable_out_point()->set_hash(previousOutput.hash.data(), previousOutput.hash.size());
    utxo.mutable_out_point()->set_index(previousOutput.index);
    auto script = Bitcoin::Script(utxo.script().begin(), utxo.script().end());

    auto txIn = TransactionInput(previousOutput, script);
    tx.inputs.push_back(txIn);

    auto txOutOrigin = Bitcoin::TransactionOutput();
    txOutOrigin.value = 100000000;
    txOutOrigin.script = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    tx.outputs.push_back(txOutOrigin);

    Data data; 
    tx.encode(data);
    ASSERT_EQ(hex(data), "0100000000000000010000000000000000000000000000000000000000000000000000000000000000ffffffff00ffffffff0100e1f505000000001976a914d782027fe48fa405263dff9f7920d9a14357801b88ac00000000");

    Data serialized;
    tx.serializeInput(0, script, 0, TWSignatureHashTypeAll, serialized); 
    ASSERT_EQ(hex(serialized), "0000000000000000000000000000000000000000000000000000000000000000ffffffff00");
    tx.serializeInput(0, script, 0, TWSignatureHashTypeAnyoneCanPay, serialized); 
    ASSERT_EQ(hex(serialized), "0000000000000000000000000000000000000000000000000000000000000000ffffffff000000000000000000000000000000000000000000000000000000000000000000ffffffff00");
}
TEST(IocoinTransaction, SerializeInput2) {
    const auto privateKey = PrivateKey(parse_hex("24f1c568d3ddd0a770d81c8562b250e108388f65b6dfc73a71a4edaa9bacc9b5"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto keyhash = Hash::ripemd(Hash::sha256(publicKey.bytes));

    const auto address = TW::Iocoin::Address(publicKey, 0x67);
    ASSERT_EQ(address.string(), "inTdSfZRVkmGYVLF14PpSKFKP5QR3tX1Qr"); 

    auto tx = Transaction();

    Bitcoin::OutPoint previousOutput1 = Bitcoin::OutPoint(std::array<byte, 32>{}, UINT32_MAX);

    Proto::UnspentTransaction utxo1;
    utxo1.set_amount(100000000);
    utxo1.mutable_out_point()->set_hash(previousOutput1.hash.data(), previousOutput1.hash.size());
    utxo1.mutable_out_point()->set_index(previousOutput1.index);
    auto script1 = Bitcoin::Script(utxo1.script().begin(), utxo1.script().end());

    auto txIn1 = TransactionInput(previousOutput1, script1);
    Bitcoin::OutPoint previousOutput2 = Bitcoin::OutPoint(std::array<byte, 32>{}, UINT32_MAX);

    Proto::UnspentTransaction utxo2;
    utxo2.set_amount(200000000);
    utxo2.mutable_out_point()->set_hash(previousOutput2.hash.data(), previousOutput2.hash.size());
    utxo2.mutable_out_point()->set_index(previousOutput2.index);
    auto script2 = Bitcoin::Script(utxo2.script().begin(), utxo2.script().end());

    auto txIn2 = TransactionInput(previousOutput1, script2);
    tx.inputs.push_back(txIn2);

    auto txOutOrigin = Bitcoin::TransactionOutput();
    txOutOrigin.value = 250000000;
    txOutOrigin.script = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    tx.outputs.push_back(txOutOrigin);


    Data serialized;
    tx.serializeInput(0, script2, 1, TWSignatureHashTypeSingle, serialized); 
    ASSERT_EQ(hex(serialized), "0000000000000000000000000000000000000000000000000000000000000000ffffffff0000000000");
}
