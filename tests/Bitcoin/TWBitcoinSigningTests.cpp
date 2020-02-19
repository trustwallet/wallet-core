// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/Transaction.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Bitcoin.pb.h"
#include "../interface/TWTestUtilities.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(BitcoinSigning, EncodeP2WPKH) {
    auto emptyScript = WRAP(TWBitcoinScript, TWBitcoinScriptCreate());
    auto unsignedTx = Transaction(1, 0x11);

    auto hash0 = parse_hex("fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f");
    auto outpoint0 = TW::Bitcoin::OutPoint(hash0, 0);
    unsignedTx.inputs.emplace_back(outpoint0, Script(), 0xffffffee);

    auto hash1 = parse_hex("ef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a");
    auto outpoint1 = TW::Bitcoin::OutPoint(hash1, 1);
    unsignedTx.inputs.emplace_back(outpoint1, Script(), UINT32_MAX);

    auto outScript0 = Script(parse_hex("76a9148280b37df378db99f66f85c95a783a76ac7a6d5988ac"));
    unsignedTx.outputs.emplace_back(112340000, outScript0);

    auto outScript1 = Script(parse_hex("76a9143bde42dbee7e4dbe6a21b2d50ce2f0167faa815988ac"));
    unsignedTx.outputs.emplace_back(223450000, outScript1);

    auto unsignedData = std::vector<uint8_t>();
    unsignedTx.encode(false, unsignedData);
    ASSERT_EQ(hex(unsignedData.begin(), unsignedData.end()), ""
        "01000000"
        "02"
            "fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000000eeffffff"
            "ef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a0100000000ffffffff"
        "02"
            "202cb206000000001976a9148280b37df378db99f66f85c95a783a76ac7a6d5988ac"
            "9093510d000000001976a9143bde42dbee7e4dbe6a21b2d50ce2f0167faa815988ac"
        "11000000");
}

TEST(BitcoinSigning, SignP2WPKH) {
    // Build transaction
    auto unsignedTx = Transaction(1, 0x11);

    auto hash0 = parse_hex("fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f");
    auto outpoint0 = OutPoint(hash0, 0);
    unsignedTx.inputs.emplace_back(outpoint0, Script(), 0xffffffee);

    auto hash1 = parse_hex("ef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a");
    auto outpoint1 = OutPoint(hash1, 1);
    unsignedTx.inputs.emplace_back(outpoint1, Script(), UINT32_MAX);

    auto outScript0 = Script(parse_hex("76a9148280b37df378db99f66f85c95a783a76ac7a6d5988ac"));
    unsignedTx.outputs.emplace_back(112'340'000, outScript0);

    auto outScript1 = Script(parse_hex("76a9143bde42dbee7e4dbe6a21b2d50ce2f0167faa815988ac"));
    unsignedTx.outputs.emplace_back(223'450'000, outScript1);

    // Setup input
    Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(335'790'000);
    input.set_byte_fee(1);
    input.set_to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    input.set_change_address("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU");

    auto utxoKey0 = parse_hex("bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxoKey1 = parse_hex("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9");
    input.add_private_key(utxoKey1.data(), utxoKey1.size());

    auto scriptPub1 = Script(parse_hex("00141d0f172a0ecb48aee1be1f2687d2963ae33f71a1"));
    auto scriptHash = std::vector<uint8_t>();
    scriptPub1.matchPayToWitnessPublicKeyHash(scriptHash);
    auto scriptHashHex = hex(scriptHash.begin(), scriptHash.end());
    ASSERT_EQ(scriptHashHex, "1d0f172a0ecb48aee1be1f2687d2963ae33f71a1");

    auto redeemScript = Script::buildPayToPublicKeyHash(scriptHash);
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[scriptHashHex] = scriptString;

    auto utxo0 = input.add_utxo();
    auto utxo0Script = parse_hex("2103c9f4836b9a4f77fc0d81f7bcb01b7f1b35916864b9476c241ce9fc198bd25432ac");
    utxo0->set_script(utxo0Script.data(), utxo0Script.size());
    utxo0->set_amount(625'000'000);
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
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
    auto result = TransactionSigner<Transaction, TransactionBuilder>(std::move(input)).sign();

    ASSERT_TRUE(result) << result.error();;
    auto signedTx = result.payload();

    // txid = "03b30d55430f08365d19a62d3bd32e459ab50984fbcf22921ecc85f1e09dc6ed"
    // witid = "20bc58d07d91a3bae9e6f4d617d8f6271723d1a7673e486cc0ecaf9e758e2c22"

    Data serialized;
    signedTx.encode(true, serialized);
    ASSERT_EQ(hex(serialized),
        "01000000"
        "0001"
        "01"
            "fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f" "00000000" "49483045022100b6006eb0fe2da8cbbd204f702b1ffdb1e29c49f3de51c4983d420bf9f9125635022032a195b153ccb2c4978333b4aad72aaa7e6a0b334a14621d5d817a42489cb0d301" "ffffffff"
        "02"
            "b0bf031400000000" "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac"
            "aefd3c1100000000" "1976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac"
        "0000000000"
    );
}

TEST(BitcoinSigning, EncodeP2WSH) {
    auto unsignedTx = Transaction(1, 0);

    auto outpoint0 = OutPoint(parse_hex("0001000000000000000000000000000000000000000000000000000000000000"), 0);
    unsignedTx.inputs.emplace_back(outpoint0, Script(), UINT32_MAX);

    auto outScript0 = Script(parse_hex("76a9144c9c3dfac4207d5d8cb89df5722cb3d712385e3f88ac"));
    unsignedTx.outputs.emplace_back(1000, outScript0);

    auto unsignedData = std::vector<uint8_t>();
    unsignedTx.encode(false, unsignedData);
    ASSERT_EQ(hex(unsignedData.begin(), unsignedData.end()), ""
        "01000000"
        "01"
            "00010000000000000000000000000000000000000000000000000000000000000000000000ffffffff"
        "01"
            "e8030000000000001976a9144c9c3dfac4207d5d8cb89df5722cb3d712385e3f88ac"
        "00000000");
}

TEST(BitcoinSigning, SignP2WSH) {
    // Setup input
    Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(1000);
    input.set_byte_fee(1);
    input.set_to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    input.set_change_address("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU");

    auto utxoKey0 = parse_hex("ed00a0841cd53aedf89b0c616742d1d2a930f8ae2b0fb514765a17bb62c7521a");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxoKey1 = parse_hex("619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9");
    input.add_private_key(utxoKey1.data(), utxoKey1.size());

    auto redeemScript = Script(parse_hex("2103596d3451025c19dbbdeb932d6bf8bfb4ad499b95b6f88db8899efac102e5fc71ac"));
    auto scriptHash = "593128f9f90e38b706c18623151e37d2da05c229";
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[scriptHash] = scriptString;

    auto utxo0 = input.add_utxo();
    auto p2wsh = Script::buildPayToWitnessScriptHash(parse_hex("ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db"));
    utxo0->set_script(p2wsh.bytes.data(), p2wsh.bytes.size());
    utxo0->set_amount(1226);
    auto hash0 = DATA("0001000000000000000000000000000000000000000000000000000000000000");
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    // Sign
    auto result = TransactionSigner<Transaction, TransactionBuilder>(std::move(input)).sign();

    ASSERT_TRUE(result) << result.error();;
    auto signedTx = result.payload();

    // txid = "b588f910d7ff03d5fbc3da91f62e48bab47153229c8d1b114b43cb31b9c4d0dd"
    // witid = "16a17dd8f6e507220010c56c07a8479e3f909f87791683577d4e6aad61ab113a"

    Data serialized;
    signedTx.encode(true, serialized);
    ASSERT_EQ(hex(serialized), "01000000"
        "0001"
        "01"
            "0001000000000000000000000000000000000000000000000000000000000000" "00000000" "00" "ffffffff"
        "01"
            "e803000000000000" "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac"
        "02"
            "4730440220252e92b8757f1e5577c54ce5deb8072914c1f03333128777dee96ebceeb6a99b02202b7298789316779d0aa7595abeedc03054405c42ab9859e67d9253d2c9a0cdfa01232103596d3451025c"
            "19dbbdeb932d6bf8bfb4ad499b95b6f88db8899efac102e5fc71ac"
        "00000000"
    );
}

TEST(BitcoinSigning, EncodeP2SH_P2WPKH) {
    auto unsignedTx = Transaction(1, 0x492);

    auto outpoint0 = OutPoint(parse_hex("db6b1b20aa0fd7b23880be2ecbd4a98130974cf4748fb66092ac4d3ceb1a5477"), 1);
    unsignedTx.inputs.emplace_back(outpoint0, Script(), 0xfffffffe);

    auto outScript0 = Script(parse_hex("76a914a457b684d7f0d539a46a45bbc043f35b59d0d96388ac"));
    unsignedTx.outputs.emplace_back(199'996'600, outScript0);

    auto outScript1 = Script(parse_hex("76a914fd270b1ee6abcaea97fea7ad0402e8bd8ad6d77c88ac"));
    unsignedTx.outputs.emplace_back(800'000'000, outScript1);

    auto unsignedData = std::vector<uint8_t>();
    unsignedTx.encode(false, unsignedData);
    ASSERT_EQ(hex(unsignedData.begin(), unsignedData.end()), ""
        "01000000"
        "01"
            "db6b1b20aa0fd7b23880be2ecbd4a98130974cf4748fb66092ac4d3ceb1a54770100000000feffffff"
        "02"
            "b8b4eb0b000000001976a914a457b684d7f0d539a46a45bbc043f35b59d0d96388ac"
            "0008af2f000000001976a914fd270b1ee6abcaea97fea7ad0402e8bd8ad6d77c88ac"
        "92040000");
}

TEST(BitcoinSigning, SignP2SH_P2WPKH) {
    // Setup input
    Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(200'000'000);
    input.set_byte_fee(1);
    input.set_to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    input.set_change_address("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU");

    auto utxoKey0 = PrivateKey(parse_hex("eb696a065ef48a2192da5b28b694f87544b30fae8327c4510137a922f32c6dcf"));
    auto pubKey0 = utxoKey0.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto utxoPubkeyHash = Hash::ripemd(Hash::sha256(pubKey0.bytes));
    ASSERT_EQ(hex(utxoPubkeyHash.begin(), utxoPubkeyHash.end()), "79091972186c449eb1ded22b78e40d009bdf0089");
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto redeemScript = Script::buildPayToWitnessPubkeyHash(utxoPubkeyHash);
    auto scriptHash = Hash::ripemd(Hash::sha256(redeemScript.bytes));
    ASSERT_EQ(hex(scriptHash.begin(), scriptHash.end()), "4733f37cf4db86fbc2efed2500b4f4e49f312023");
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[hex(scriptHash.begin(), scriptHash.end())] = scriptString;

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Script(parse_hex("a9144733f37cf4db86fbc2efed2500b4f4e49f31202387"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(1000'000'000);
    auto hash0 = DATA("db6b1b20aa0fd7b23880be2ecbd4a98130974cf4748fb66092ac4d3ceb1a5477");
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    // Sign
    auto result = TransactionSigner<Transaction, TransactionBuilder>(std::move(input)).sign();

    ASSERT_TRUE(result) << result.error();
    auto signedTx = result.payload();

    // txid = "060046204220fd00b81fd6426e391acb9670d1e61e8f0224f37276cc34f49e8c"
    // witid = "3911b16643972437d27a759b5647a552c7a2e433364b531374f3761967bf8fd7"

    Data serialized;
    signedTx.encode(true, serialized);
    ASSERT_EQ(hex(serialized), "01000000000101db6b1b20aa0fd7b23880be2ecbd4a98130974cf4748fb66092ac4d3ceb1a5477010000001716001479091972186c449eb1ded22b78e40d009bdf0089ffffffff0200c2eb0b000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac1e07af2f000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac02473044022009195d870ecc40f54130008e392904e77d32b738c1add19d1d8ebba4edf812e602204f49de6dc60d9a3c3703e1e642942f8834f3a2cd81a6562a34b293942ce42f40012103ad1d8e89212f0b92c74d23bb710c00662ad1470198ac48c43f7d6f93a2a2687300000000");
}

TEST(BitcoinSigning, EncodeP2SH_P2WSH) {
    auto unsignedTx = Transaction(1, 0);

    auto hash0 = parse_hex("36641869ca081e70f394c6948e8af409e18b619df2ed74aa106c1ca29787b96e");
    auto outpoint0 = OutPoint(hash0, 1);
    unsignedTx.inputs.emplace_back(outpoint0, Script(), 0xffffffff);

    auto outScript0 = Script(parse_hex("76a914389ffce9cd9ae88dcc0631e88a821ffdbe9bfe2688ac"));
    unsignedTx.outputs.emplace_back(0x0000000035a4e900, outScript0);

    auto outScript1 = Script(parse_hex("76a9147480a33f950689af511e6e84c138dbbd3c3ee41588ac"));
    unsignedTx.outputs.emplace_back(0x00000000052f83c0, outScript1);

    auto unsignedData = std::vector<uint8_t>();
    unsignedTx.encode(false, unsignedData);
    ASSERT_EQ(hex(unsignedData.begin(), unsignedData.end()), ""
        "01000000"
        "01"
            "36641869ca081e70f394c6948e8af409e18b619df2ed74aa106c1ca29787b96e0100000000ffffffff"
        "02"
            "00e9a435000000001976a914389ffce9cd9ae88dcc0631e88a821ffdbe9bfe2688ac"
            "c0832f05000000001976a9147480a33f950689af511e6e84c138dbbd3c3ee41588ac"
        "00000000");
}

TEST(BitcoinSigning, SignP2SH_P2WSH) {
    auto emptyScript = Script();
    auto unsignedTx = Transaction(1, 0);

    auto outpoint0 = OutPoint(parse_hex("36641869ca081e70f394c6948e8af409e18b619df2ed74aa106c1ca29787b96e"), 1);
    unsignedTx.inputs.emplace_back(outpoint0, emptyScript, 0xffffffff);

    auto outScript0 = Script(parse_hex("76a914389ffce9cd9ae88dcc0631e88a821ffdbe9bfe2688ac"));
    unsignedTx.outputs.emplace_back(0x0000000035a4e900, outScript0);

    auto outScript1 = Script(parse_hex("76a9147480a33f950689af511e6e84c138dbbd3c3ee41588ac"));
    unsignedTx.outputs.emplace_back(0x00000000052f83c0, outScript1);

    // Setup signing input
    auto input = Proto::SigningInput();

    auto key0 = parse_hex("730fff80e1413068a05b57d6a58261f07551163369787f349438ea38ca80fac6");
    input.add_private_key(key0.data(), key0.size());
    auto key1 = parse_hex("11fa3d25a17cbc22b29c44a484ba552b5a53149d106d3d853e22fdd05a2d8bb3");
    input.add_private_key(key1.data(), key1.size());
    auto key2 = parse_hex("77bf4141a87d55bdd7f3cd0bdccf6e9e642935fec45f2f30047be7b799120661");
    input.add_private_key(key2.data(), key2.size());
    auto key3 = parse_hex("14af36970f5025ea3e8b5542c0f8ebe7763e674838d08808896b63c3351ffe49");
    input.add_private_key(key3.data(), key3.size());
    auto key4 = parse_hex("fe9a95c19eef81dde2b95c1284ef39be497d128e2aa46916fb02d552485e0323");
    input.add_private_key(key4.data(), key4.size());
    auto key5 = parse_hex("428a7aee9f0c2af0cd19af3cf1c78149951ea528726989b2e83e4778d2c3f890");
    input.add_private_key(key5.data(), key5.size());

    auto redeemScript = Script::buildPayToWitnessScriptHash(parse_hex("a16b5755f7f6f96dbd65f5f0d6ab9418b89af4b1f14a1bb8a09062c35f0dcb54"));
    auto scriptHash = Hash::ripemd(Hash::sha256(redeemScript.bytes));
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[hex(scriptHash.begin(), scriptHash.end())] = scriptString;

    auto witnessScript = Script(parse_hex(""
        "56"
            "210307b8ae49ac90a048e9b53357a2354b3334e9c8bee813ecb98e99a7e07e8c3ba3"
            "2103b28f0c28bfab54554ae8c658ac5c3e0ce6e79ad336331f78c428dd43eea8449b"
            "21034b8113d703413d57761b8b9781957b8c0ac1dfe69f492580ca4195f50376ba4a"
            "21033400f6afecb833092a9a21cfdf1ed1376e58c5d1f47de74683123987e967a8f4"
            "2103a6d48b1131e94ba04d9737d61acdaa1322008af9602b3b14862c07a1789aac16"
            "2102d8b661b0b3302ee2f162b09e07a55ad5dfbe673a9f01d9f0c19617681024306b"
        "56ae"
    ));
    auto witnessScriptHash = Hash::ripemd(Hash::sha256(witnessScript.bytes));
    auto witnessScriptString = std::string(witnessScript.bytes.begin(), witnessScript.bytes.end());
    (*input.mutable_scripts())[hex(witnessScriptHash.begin(), witnessScriptHash.end())] = witnessScriptString;

    auto utxo0Script = Script(parse_hex("a9149993a429037b5d912407a71c252019287b8d27a587"));
    auto utxo = input.add_utxo();
    utxo->mutable_out_point()->set_hash(outpoint0.hash.data(), outpoint0.hash.size());
    utxo->mutable_out_point()->set_index(outpoint0.index);
    utxo->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo->set_amount(987654321);

    // Sign
    auto signer = TransactionSigner<Transaction, TransactionBuilder>(std::move(input));
    signer.transaction = unsignedTx;
    signer.plan.utxos = {*utxo};
    auto result = signer.sign();
    ASSERT_TRUE(result) << result.error();;
    auto signedTx = result.payload();

    auto expected = ""
            "01000000"
            "0001"
            "01"
                "36641869ca081e70f394c6948e8af409e18b619df2ed74aa106c1ca29787b96e0100000023220020a16b5755f7f6f96dbd65f5f0d6ab9418b89af4b1f14a1bb8a09062c35f0dcb54ffffffff"
            "02"
                "00e9a43500000000" "1976a914389ffce9cd9ae88dcc0631e88a821ffdbe9bfe2688ac"
                "c0832f0500000000" "1976a9147480a33f950689af511e6e84c138dbbd3c3ee41588ac"
            "08"
                "00"
                "47304402201992f5426ae0bab04cf206d7640b7e00410297bfe5487637f6c2427ee8496be002204ad4e64dc2d269f593cc4820db1fc1e8dc34774f602945115ce485940e05c64200"
                "47304402201e412363fa554b994528fd44149f3985b18bb901289ef6b71105b27c7d0e336c0220595e4a1e67154337757562ed5869127533e3e5084c3c2e128518f5f0b85b721800"
                "473044022003b0a20ccf545b3f12c5ade10db8717e97b44da2e800387adfd82c95caf529d902206aee3a2395530d52f476d0ddd9d20ba062820ae6f4e1be4921c3630395743ad900"
                "483045022100ed7a0eeaf72b84351bceac474b0c0510f67065b1b334f77e6843ed102f968afe022004d97d0cfc4bf5651e46487d6f87bd4af6aef894459f9778f2293b0b2c5b7bc700"
                "483045022100934a0c364820588154aed2d519cbcc61969d837b91960f4abbf0e374f03aa39d022036b5c58b754bd44cb5c7d34806c89d9778ea1a1c900618a841e9fbfbe805ff9b00"
                "473044022044e3b59b06931d46f857c82fa1d53d89b116a40a581527eac35c5eb5b7f0785302207d0f8b5d063ffc6749fb4e133db7916162b540c70dee40ec0b21e142d8843b3a00"
            "cf56"
                "210307b8ae49ac90a048e9b53357a2354b3334e9c8bee813ecb98e99a7e07e8c3ba3"
                "2103b28f0c28bfab54554ae8c658ac5c3e0ce6e79ad336331f78c428dd43eea8449b"
                "21034b8113d703413d57761b8b9781957b8c0ac1dfe69f492580ca4195f50376ba4a"
                "21033400f6afecb833092a9a21cfdf1ed1376e58c5d1f47de74683123987e967a8f4"
                "2103a6d48b1131e94ba04d9737d61acdaa1322008af9602b3b14862c07a1789aac16"
                "2102d8b661b0b3302ee2f162b09e07a55ad5dfbe673a9f01d9f0c19617681024306b"
            "56ae"
            "00000000";

    auto serialized = std::vector<uint8_t>();
    signedTx.encode(true, serialized);
    ASSERT_EQ(hex(serialized.begin(), serialized.end()), expected);
}
