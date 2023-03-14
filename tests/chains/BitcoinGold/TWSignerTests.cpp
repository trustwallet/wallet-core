// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <gtest/gtest.h>

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/SigHashType.h"
#include "Bitcoin/Transaction.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "proto/Bitcoin.pb.h"
#include "../Bitcoin/TxComparisonHelper.h"
#include "TestUtilities.h"

namespace TW::Bitcoin {

TEST(TWBitcoinGoldSigner, SignTransaction) {
    const int64_t amount = 10000;

    // Setup input
    Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeBitcoinGold);
    input.set_hash_type(hashTypeForCoin(TWCoinTypeBitcoinGold));
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("btg1qmd6x5awe4t5fjhgntv0pngzdwajjg250wxdcs0");
    input.set_change_address("btg1qawhpp9gv3g662phqufjmj2ps2ge7sq4thy5g07");

    auto utxoKey0 = parse_hex("cbe13a79b82ec7f8871b336a64fd8d531f598e7c9022e29c67e824cfd54af57f");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto scriptPub1 = Script(parse_hex("0014db746a75d9aae8995d135b1e19a04d7765242a8f"));
    auto scriptHash = std::vector<uint8_t>();
    scriptPub1.matchPayToWitnessPublicKeyHash(scriptHash);
    auto scriptHashHex = hex(scriptHash);

    auto redeemScript = Script::buildPayToPublicKeyHash(scriptHash);
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[scriptHashHex] = scriptString;

    auto utxo0 = input.add_utxo();
    auto utxo0Script = parse_hex("0014d53cae7c6fb6c8efe4fd8bfecea36534105b1674");
    utxo0->set_script(utxo0Script.data(), utxo0Script.size());
    utxo0->set_amount(99000);

    auto hash0 = parse_hex("1d4653041a1915b3a52d47aeaa119c8f79ed7634a93692a6e811173067464f03");
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(0xfffffffd);
    input.set_lock_time(0x00098971);

    Proto::TransactionPlan plan;
    {
        // try plan first
        ANY_PLAN(input, plan, TWCoinTypeBitcoinGold);
        EXPECT_TRUE(verifyPlan(plan, {99'000}, amount, 141));
    }

    // Supply plan for signing, to match fee of previously-created real TX
    *input.mutable_plan() = plan;
    input.mutable_plan()->set_fee(149);
    input.mutable_plan()->set_change(88851);

    // Sign
    auto signingInput = SigningInput(input);
    auto result = TransactionSigner<Transaction, TransactionBuilder>::sign(signingInput);

    ASSERT_TRUE(result) << std::to_string(result.error());
    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(serialized);
    // BitcoinGold Mainnet: https://btg2.trezor.io/tx/db26faec66d070045df0da56140349beb5a12bd14bca12b162fded8f84d18afa
    EXPECT_EQ(serialized.size(), 222ul);
    // clang-format off
    ASSERT_EQ(hex(serialized),
        "01000000"
        "0001"
        "01"
            "1d4653041a1915b3a52d47aeaa119c8f79ed7634a93692a6e811173067464f03" "01000000" "00" "fdffffff"
        "02"
            "1027000000000000" "160014db746a75d9aae8995d135b1e19a04d7765242a8f"
            "135b010000000000" "160014ebae10950c8a35a506e0e265b928305233e802ab"
        "02"
            "4730440220325c56363b17e1b1329efeb400c0933a3d9adfb304f29889b3ef01084aef19e302202a69d9be9ef668b5a5517fbfa42e1fc26b3f8b582c721bd1eabd721322bc2b6c41"
            "2103e00b5dec8078d526fba090247bd92db6b67a4dd1953b788cea9b52de9471b8cf"
        "71890900"
    );
    // clang-format on
}

} // namespace TW::Bitcoin
