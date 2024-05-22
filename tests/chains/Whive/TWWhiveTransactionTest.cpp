// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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

TEST(WhiveTransaction, SignTransaction) {
    /*
        https://iancoleman.io/bip39/
        Mnemonic - shoot island position soft burden budget tooth cruel issue economy destroy above
        m/44'/175'/0'/0/0 Address - RHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS
        m/44'/175'/0'/0/0 Private key in Base58 encoding - L1At2vQpaHCmbiu333N3kD4nbDzJgvb8hxNp5S8bQApocFYuW1rx
        m/44'/175'/0'/0/0 Private key in bytes - 75e4c520c92b3836e77dfe2715da469b71f7df86fc11ef328870735a700551fa
        utxo - https://blockbook.Whive.org/tx/0c7e82b44eec71d634c013e2db3cb4fa26f87fbc90eb8734da93807d23605544
        tx - https://blockbook.Whive.org/tx/3717b528eb4925461d9de5a596d2eefe175985740b4fda153255e10135f236a6
    */

    const int64_t utxo_amount = 100000000;
    const int64_t amount = 50000000;
    const int64_t fee = 2000000;

    auto input = Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("WQhUVG2QP8B8j8aStTwhw6f6RfWzsVabY4");
    input.set_change_address("wv1qdslrnma3muhtwym7d08ul73zuqu9vh34aanjtk");
    input.set_coin_type(TWCoinTypeWhive);

    auto hash0 = DATA("047c7b7ca4d84ec16ad137647ac95a361a279266af583dbe87f05b486a8aa5ad");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("79c6ddf1aec99f000092b259ecd903822e972af7a3a95cc09f3e99f73d64e08d");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("0c3216190dead1c4e314e20215f80a6d7d1e83290296a67cc00d0a06617981c2");
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
        "47304402203fcc78dedcd4011eb1f0a7b02fbe2efa9355fb7e0fa1dfd565b1594fa6842f02022004c2091fb75ae148c2cf24cd3fafa92edaca33cd89f41c121752e8597d6ba2b6012103fb32f6eb7a2092e3246c296bcf05d404e40eafecde4ec051cc17b07c30a9fbaa");
}

TEST(WhiveTransaction, LockScripts) {
    // P2PKH
    // https://whiveexplorer.cointest.com/tx/79c6ddf1aec99f000092b259ecd903822e972af7a3a95cc09f3e99f73d64e08d

    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("wv1qdslrnma3muhtwym7d08ul73zuqu9vh34aanjtk").get(), TWCoinTypeWhive));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "79c6ddf1aec99f000092b259ecd903822e972af7a3a95cc09f3e99f73d64e08d");

    // P2SH
    // https://whiveexplorer.cointest.com/tx/64dacb4a0f0bc0ec1b9a0766fcce378c75f6cb97cb3dbb9374164ca8af4acdf0

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("WeS3GruDo24c7eHQrrULQSMC79y1ZNvnYs").get(), TWCoinTypeWhive));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "ea1c7a4b99a477c41f9eda53e1e12e605d9640a57425e71899a82e10c0f36e12");
}

} // namespace TW::Bitcoin
