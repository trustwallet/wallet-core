// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(RavencoinTransaction, SignTransaction) {
    /*
        https://iancoleman.io/bip39/
        Mnemonic - shoot island position soft burden budget tooth cruel issue economy destroy above
        m/44'/175'/0'/0/0 Address - RHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS
        m/44'/175'/0'/0/0 Private key in Base58 encoding - L1At2vQpaHCmbiu333N3kD4nbDzJgvb8hxNp5S8bQApocFYuW1rx
        m/44'/175'/0'/0/0 Private key in bytes - 75e4c520c92b3836e77dfe2715da469b71f7df86fc11ef328870735a700551fa
        utxo - https://blockbook.ravencoin.org/tx/0c7e82b44eec71d634c013e2db3cb4fa26f87fbc90eb8734da93807d23605544
        tx - https://blockbook.ravencoin.org/tx/3717b528eb4925461d9de5a596d2eefe175985740b4fda153255e10135f236a6
    */

    const int64_t utxo_amount = 100000000;
    const int64_t amount = 50000000;
    const int64_t fee = 2000000;

    auto input = Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("RNoSGCX8SPFscj8epDaJjqEpuZa2B5in88");
    input.set_change_address("RHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS");
    input.set_coin_type(TWCoinTypeRavencoin);

    auto hash0 = DATA("445560237d8093da3487eb90bc7ff826fab43cdbe213c034d671ec4eb4827e0c");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("76a9145d6e33f3a108bbcc586cbbe90994d5baf5a9cce488ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("75e4c520c92b3836e77dfe2715da469b71f7df86fc11ef328870735a700551fa");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    auto plan = TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = utxo_amount - amount - fee;

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto result = TransactionSigner<Transaction, TransactionBuilder>::sign(input);
    auto signedTx = result.payload();
    ASSERT_TRUE(result);

    Data serialized;
    signedTx.encode(serialized, Transaction::SegwitFormatMode::NonSegwit);
    ASSERT_EQ(
        hex(serialized),
        "0100000001445560237d8093da3487eb90bc7ff826fab43cdbe213c034d671ec4eb4827e0c000000006b483045022100d790bdaa3c44eb5e3a422365ca5fc009c4512625222e3378f2f16e7e6ef1732a0220688c1bb995b7ff2f12729e101d7c24b6314430317e7717911fdc35c0d84f2f0d012102138724e702d25b0fdce73372ccea9734f9349442d5a9681a5f4d831036cd9429ffffffff0280f0fa02000000001976a9149451f4546e09fc2e49ef9b5303924712ec2b038e88ac006cdc02000000001976a9145d6e33f3a108bbcc586cbbe90994d5baf5a9cce488ac00000000");
}

TEST(RavencoinTransaction, LockScripts) {
    // P2PKH
    // https://blockbook.ravencoin.org/tx/3717b528eb4925461d9de5a596d2eefe175985740b4fda153255e10135f236a6

    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("RNoSGCX8SPFscj8epDaJjqEpuZa2B5in88").get(), TWCoinTypeRavencoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a9149451f4546e09fc2e49ef9b5303924712ec2b038e88ac");

    // P2SH
    // https://ravencoin.network/api/tx/f600d07814677d1f60545c8f7f71260238595c4928d6fb87caa0f9dd732e9bb5

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("rPWwn5h4QFZNaz1XmY39rc73sdYGGDdmq1").get(), TWCoinTypeRavencoin));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a914bd92088bb7e82d611a9b94fbb74a0908152b784f87");
}

} // namespace TW::Bitcoin
