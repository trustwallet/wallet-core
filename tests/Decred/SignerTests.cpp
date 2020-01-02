// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Decred/Address.h"
#include "Decred/Signer.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Decred.pb.h"

#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Decred;

TEST(DecredSigner, Sign) {
    const auto privateKey = PrivateKey(parse_hex("22a47fa09a223f2aa079edf85a7c2d4f8720ee63e502ee2869afab7de234b80c"));
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
    signer.plan.utxos.push_back(*utxo0);
    signer.plan.amount = 100'000'000;
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
