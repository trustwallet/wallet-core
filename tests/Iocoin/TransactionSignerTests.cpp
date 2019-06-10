// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Iocoin/Address.h"
#include "Iocoin/TransactionSigner.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Iocoin.pb.h"

#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Iocoin;

TEST(IocoinSigner, SignBase) {
    const auto privateKey = PrivateKey(parse_hex("9b681a7d3722ded5e93c33615b699fd670de7c558a5d196b7fb38f56744c340f"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto keyhash = Hash::ripemd(Hash::sha256(publicKey.bytes));

    const auto address = TW::Iocoin::Address(publicKey, 0x67);
    ASSERT_EQ(address.string(), "ifGCfC57Nk6SWZvKByfeFqxgDEYmgDjLpq"); 

    auto tx_0 = Transaction();

    Bitcoin::OutPoint previousOutput = Bitcoin::OutPoint(std::array<byte, 32>{}, UINT32_MAX);

    Proto::UnspentTransaction utxo;
    utxo.set_amount(100000000);
    utxo.mutable_out_point()->set_hash(previousOutput.hash.data(), previousOutput.hash.size());
    utxo.mutable_out_point()->set_index(previousOutput.index);
    auto script = Bitcoin::Script(utxo.script().begin(), utxo.script().end());

    auto txIn = TransactionInput(previousOutput, script);
    tx_0.inputs.push_back(txIn);

    auto txOutOrigin = Bitcoin::TransactionOutput();
    txOutOrigin.value = 100000000;
    txOutOrigin.script = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    tx_0.outputs.push_back(txOutOrigin);

    Data data; 
    tx_0.encode(data);
    ASSERT_EQ(hex(data), "0100000000000000010000000000000000000000000000000000000000000000000000000000000000ffffffff00ffffffff0100e1f505000000001976a914888fdd72857dbb44c4f18483f5aa129d46b493f188ac00000000");

    Bitcoin::Proto::SigningInput input;
    input.set_hash_type(TWSignatureHashTypeAll);
    input.set_amount(50000000);
    input.set_byte_fee(100);
    input.set_to_address("ifqSLgtsywgZuFfXYAAFxeNaM1N4ihBTDn");
    input.set_change_address("ioQtVAVgoa3vPeMNTqxyuey1Non9YFVUYo");

    auto utxoKey = parse_hex("9b681a7d3722ded5e93c33615b699fd670de7c558a5d196b7fb38f56744c340f");
    input.add_private_key(utxoKey.data(), utxoKey.size());
    auto utxo1 = input.add_utxo();
    auto utxoScript = Bitcoin::Script::buildPayToPublicKeyHash(keyhash);
    utxo1->set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
    utxo1->set_amount(100000000);
    //tx hash 052574051d4d1c3a06b3616e2055a8152e12608c8fadd993842962c47d8d014e
    auto txHash = parse_hex("4e018d7dc462298493d9ad8f8c60122e15a855206e61b3063a1c4d1d05742505");
    utxo1->mutable_out_point()->set_hash(txHash.data(), txHash.size());
    utxo1->mutable_out_point()->set_index(0);

    auto signer = TransactionSigner<Iocoin::Transaction>(std::move(input));
    const auto result = signer.sign();
    ASSERT_TRUE(result);
    EXPECT_EQ(hex(result.payload().inputs[0].script.bytes), "4730440220626796dca5b2ee4b8893be928af98ce128d506384a12a9d48f432956ada6e3ba02207e2898bd3e658967e7de994d39411dfc26cd7fa33e7396132e329926132387240121027ddb3c4cb6dde97cbdade59b9496f56cd7b60166e11ebbb80ab84e0c45de17ea");

    Data serialized;
    auto signedTx = result.payload();
    signedTx.encode(serialized);
    ASSERT_EQ(hex(serialized), "0100000000000000014e018d7dc462298493d9ad8f8c60122e15a855206e61b3063a1c4d1d05742505000000006a4730440220626796dca5b2ee4b8893be928af98ce128d506384a12a9d48f432956ada6e3ba02207e2898bd3e658967e7de994d39411dfc26cd7fa33e7396132e329926132387240121027ddb3c4cb6dde97cbdade59b9496f56cd7b60166e11ebbb80ab84e0c45de17eaffffffff0280f0fa02000000001976a9148ed90317711d91068dab826eab43e4a6f8489bc188ace069f902000000001976a914e1f571eb4f4397c64cba77f2db74f1a04e6aeea088ac00000000");

    auto prevOutHash = signedTx.getPrevoutHash();
    ASSERT_EQ(hex(prevOutHash), "854dfbd49febb33c61e7425f9e1a78ba3eb3e6390b431bfcf9f4da6c7be856a0");
    auto seq = signedTx.getSequenceHash();
    ASSERT_EQ(hex(seq), "5df6e0e2761359d30a8275058e299fcc0381534545f55cf43e41983f5d4c9456");
    auto oHash = signedTx.getOutputsHash();
    ASSERT_EQ(hex(oHash), "963b7388f1975f4760064f38a395dec60b64c59b4d6ece369303aafdfaa182bf");
}

