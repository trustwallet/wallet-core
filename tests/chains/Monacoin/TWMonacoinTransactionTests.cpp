// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(MonacoinTransaction, SignTransaction) {
    /*
        https://iancoleman.io/bip39/
        Mnemonic - perfect runway tissue hover click expire orbit subway insane joy husband circle
        m/44'/22'/0'/0/0 Address - MX7ZpcMMN4GVDeUvCjAYwfRyMgfBzYNr3E
        m/44'/22'/0'/0/0 Private key in Base58 encoding - T8XV834egE6ZsgsQFPnBcYbNdFKNiGKiNj21mRjGx2scGNQh3ypZ
        m/44'/22'/0'/0/0 Private key in bytes - a356a193a24c73c657e0c7bbf4e876964984a2dcba986ea1ea1fca7b025218f3
        utxo - https://blockbook.electrum-mona.org/tx/ed0a42ced5e5fc78a0568b4bdeb512dcfc47c1aace429cc060b6c3cc3d511a44
        tx - https://blockbook.electrum-mona.org/tx/0c2916dee64b1e60414199debfa22300c7ecd1f94f3a5602d2e5b9e120b63573
    */

    const int64_t utxo_amount = 100000000;
    const int64_t amount = 50000000;
    const int64_t fee = 20000;

    auto input = Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("M8aShwteMWyAbUw4SGS4EHLqfo1EfnKHcM");
    input.set_change_address("MX7ZpcMMN4GVDeUvCjAYwfRyMgfBzYNr3E");
    input.set_coin_type(TWCoinTypeMonacoin);

    auto hash0 = DATA("441a513dccc3b660c09c42ceaac147fcdc12b5de4b8b56a078fce5d5ce420aed");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("76a914fea39370769d4fed2d8ab98dd5daa482cc56113b88ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("a356a193a24c73c657e0c7bbf4e876964984a2dcba986ea1ea1fca7b025218f3");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeDigiByte);
    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(utxo_amount - amount - fee);

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan;

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeDigiByte);

    ASSERT_EQ(output.error(), Common::Proto::OK);
    ASSERT_EQ(hex(output.encoded()),
              "0100000001441a513dccc3b660c09c42ceaac147fcdc12b5de4b8b56a078fce5d5ce420aed000000006a473044022047789dc7483b178199439bbfce0ab0caf532fec51095ba099d0d9b0b2169033402201745a0160d8d327655a8ef0542367396ce86bbb13df6b183d58c922e422cfa10012102fc08693599fda741558613cd44a50fc65953b1be797637f8790a495b85554f3effffffff0280f0fa02000000001976a914076df984229a2731cbf465ec8fbd35b8da94380f88ac60a2fa02000000001976a914fea39370769d4fed2d8ab98dd5daa482cc56113b88ac00000000");
}

TEST(MonacoinTransaction, LockScripts) {
    // P2PKH
    // https://blockbook.electrum-mona.org/tx/79ebdce15e4ac933328e62dbe92302fc8b4833786e46df8a4f18295cb824fb67

    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("M8aShwteMWyAbUw4SGS4EHLqfo1EfnKHcM").get(), TWCoinTypeMonacoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914076df984229a2731cbf465ec8fbd35b8da94380f88ac");

    // P2SH
    // https://blockbook.electrum-mona.org/tx/726ae7d5179bfd8c7d51a5b956c3d6a262fe5190c36ed7bcb3799dc5759d5830

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("P91UYtoBS4XAD39fEzaeMaq7YmMa42FFNd").get(), TWCoinTypeMonacoin));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a914049880fc73bb6a5e0140404713cabe2592fb2c5587");

    // BECH32
    // https://blockbook.electrum-mona.org/tx/6d7ebe444cc12c14625fa526ed9d81058b04d2f0c3b5dad2fb0032eeec3ba511

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("mona1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asju3qmd").get(), TWCoinTypeMonacoin));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "001422e6014ad3631f1939281c3625bc98db808fbfb0");
}

} // namespace TW::Bitcoin
