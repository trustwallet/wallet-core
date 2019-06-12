
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace Bitcoin;

/*
 https://iancoleman.io/bip39/ and https://www.mobilefish.com/download/ethereum/hd_wallet.html
 Mnemonic - salute unfair glide naive deputy march trial phrase blouse control home staff injury draft learn achieve issue album
 m/44'/31'/0'/0/0 Private key in Base58 encoding - KnU39LFZH2uAm6Uocmo2SrTi95yXpw84TRm3mGays5R8e1XPQJ7u
 m/44'/31'/0'/0/0 Private key in bytes - fad56f32be3d4d1f3672716a0695c7999acda5330d7b95bcd25f46bbf3a11b20
 https://blockbook.monetaryunit.org/tx/a3681a20f19ded012b11486af269cd585eea9c092dd75550c51c991acb860728
 */
TEST(MonetaryUnitTransaction, SignTransaction) {
    const int64_t utxo_amount = 1500000000;
    const int64_t amount = 499997740;
    const int64_t fee = 2260;
    
    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWSignatureHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("7bH5T6e99x2xssC8thzqJcxvpQMVFeaAht");
    input.set_change_address("7XUdVpjrhY31uChwLEBUirUkKQNj8nvcrT");
    input.set_coin_type(TWCoinTypeMonetaryUnit);
    
    auto hash0 = DATA("170f786b94e0e287c1cce0214c78006b8c0080382304be67bc6f248ef7f29af1");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("76a91437993e4c8dea4eac2697d2ecb03748e0a9bb78d788ac");
    utxo0->set_script(script0.data(), script0.size());
    
    auto utxoKey0 = DATA("fad56f32be3d4d1f3672716a0695c7999acda5330d7b95bcd25f46bbf3a11b20");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));
    
    auto plan = Bitcoin::TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = utxo_amount - amount - fee;
    
    // Sign
    auto signer = TW::Bitcoin::TransactionSigner<TW::Bitcoin::Transaction>(std::move(input), plan);
    auto result = signer.sign();
    auto signedTx = result.payload();
    
    ASSERT_TRUE(result);
    ASSERT_EQ(fee, signer.plan.fee);
    
    Data serialized;
    signedTx.encode(false, serialized);
    ASSERT_EQ(
              hex(serialized),
              "01000000"
              "01"
              "170f786b94e0e287c1cce0214c78006b8c0080382304be67bc6f248ef7f29af1"
              "01000000"
              "6a"
              "473044022013d04c47ce9d530944d6e40e5d2c2e6cc03e0a776caf90b7f32557f41a74eb3c02203bc78aef91eb3f54abeee041a47290ca99e0052c4c57ee77a50095ecb4bb5571012103adbb6d0a2f3934be0b76148e5efb7eecfab0b1ee03279a2bdb2b2c9009f9c8bd"
              "ffffffff"
              "02"
              "2c5ccd1d00000000""19"
              "76a914614a6b5f6fb8d3690d3f31331a5476b3a597851288ac"
              "00ca9a3b00000000""19"
              "76a91437993e4c8dea4eac2697d2ecb03748e0a9bb78d788ac"
              "00000000"
              );
}


TEST(MonetaryUnitTransaction, LockScripts) {
    // https://blockbook.monetaryunit.org/tx/18f148b7d6098c59f7841333e156e210b0369ace9d7040a3c8de9eda6e85b6ec
    
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("7nnXDJCkajemktQYTZcz4gP46RLZiqUhXR").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914df855efb3fb8e58e733d24b98c3b892da978fb1588ac");
    
    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("7YQbBvhpy1u9Xf5MJSUNnnpQQMiape9NW3").get()));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a91441cdc737b3a0862479ffc56deffefe099ca0cfbc88ac");
}
