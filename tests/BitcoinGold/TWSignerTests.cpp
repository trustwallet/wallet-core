// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <gtest/gtest.h>

#include "Bitcoin/SegwitAddress.h"
#include "proto/Bitcoin.pb.h"
#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/Transaction.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "../interface/TWTestUtilities.h"

using namespace TW;
using namespace TW::Bitcoin;

TEST(TWBitcoinGoldSigner, SignTransaction) {
    const int64_t amount = 10000;

    // Setup input
    Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeBitcoinGold);
    input.set_hash_type(TWBitcoinSigHashTypeAll | TWBitcoinSigHashTypeForkBTG);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("btg1qmd6x5awe4t5fjhgntv0pngzdwajjg250wxdcs0");
    input.set_change_address("btg1qawhpp9gv3g662phqufjmj2ps2ge7sq4thy5g07");

    auto utxoKey0 = parse_hex("cbe13a79b82ec7f8871b336a64fd8d531f598e7c9022e29c67e824cfd54af57f");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());


    auto scriptPub1 = Script(parse_hex("0014db746a75d9aae8995d135b1e19a04d7765242a8f"));
    auto scriptHash = std::vector<uint8_t>();
    scriptPub1.matchPayToWitnessPublicKeyHash(scriptHash);
    auto scriptHashHex = hex(scriptHash.begin(), scriptHash.end());

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

    // Sign
    auto txSinger = TransactionSigner<Transaction, TransactionBuilder>(std::move(input));
    txSinger.transaction.lockTime = 0x00098971;
    auto result = txSinger.sign();

    ASSERT_TRUE(result) << result.error();;
    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(true, serialized);
    // BitcoinGold Mainnet: https://btg2.trezor.io/tx/3e818ad25d73123b6c1c8099ed462aa5413a4ef57d66d9d260306c012753ba43
    ASSERT_EQ(hex(serialized),
        "01000000"
        "0001"
        "01"
            "1d4653041a1915b3a52d47aeaa119c8f79ed7634a93692a6e811173067464f03" "01000000" "00" "fdffffff"
        "02"
            "1027000000000000" "160014db746a75d9aae8995d135b1e19a04d7765242a8f"
            "c65a010000000000" "160014ebae10950c8a35a506e0e265b928305233e802ab"
        "02"
            "473044022029b81a6b8f57f76aaf510d8a222ca835bd806936e329aead433f120007d6847002203afa611ff7823ec2a6770359901b0cacf56527cbf947b226ed86b61811545e2b41"
            "2103e00b5dec8078d526fba090247bd92db6b67a4dd1953b788cea9b52de9471b8cf"
        "71890900"
    );
}
            
