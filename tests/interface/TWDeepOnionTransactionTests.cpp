
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "DeepOnion/Transaction.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::DeepOnion;

/*
TEST(DeepOnionTransaction, SignTransaction) {
    /*
        m/44'/305'/0'/0/0 Address - DcUvw7yP8dhVPGHMj7hPWderRfeVQByqrA
        m/44'/305'/0'/0/0 Private key in Base58 encoding - 6LcK7pESURyZUaus5QU1bykTm2mngH2GgdChM8gK2xccYNLn6LY
        m/44'/305'/0'/0/0 Private key in bytes - 4646464646464646464646464646464646464646464646464646464646464646
        utxo - https://chainz.cryptoid.info/onion/tx.dws?2743237.htm 2488ff54e5b98528422202986aac51f6c2c47c39b6b5ebc6b9f1d4598ce2bd27
        tx - https://chainz.cryptoid.info/onion/tx.dws?2743254.htm 69a4b0b8641c16268033667371e71ed1b0c1a48bbb09831c61398c9c47978d0e
    

    const int64_t utxo_amount = 4910598;
    const int64_t amount = 4867035;
    const int64_t fee = 10000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWSignatureHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("DohbWhdFgC4KEr4SM7vBSuhQ3hNqzNsLhs");
    input.set_change_address("DqaUWMm9tfEvroAzA1L6G6PK9gRT7nJQ5u");

    auto hash0 = DATA("445560237d8093da3487eb90bc7ff826fab43cdbe213c034d671ec4eb4827e0c");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("76a9145d6e33f3a108bbcc586cbbe90994d5baf5a9cce488ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("4646464646464646464646464646464646464646464646464646464646464646");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    auto plan = Bitcoin::TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = utxo_amount - amount - fee;

    // Sign
    auto signer = TW::Bitcoin::TransactionSigner<TW::DeepOnion::Transaction>(std::move(input), plan);
    auto result = signer.sign();

    ASSERT_TRUE(result);
    ASSERT_EQ(fee, signer.plan.fee);

    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(false, serialized);
    ASSERT_EQ(
        hex(serialized),
        "010000000c9a035d0127bde28c59d4f1b9c6ebb5b6397cc4c2f651ac6a980222422885b9e554ff8824000000008a47304402200f6b2d7b25af127e460c89059137eb9125475e339cc1bf7f030fc792233f0cfd022072f51331685e3ecf96cf71bd8d4d08537d9ae1a60c65dd999952cc4d3f4079940141044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad7119ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232afeffffff021b830000000000001976a914e77245576e4b6701aaa762a885d1ba6511d566f488acdb434a00000000001976a914d2dac77deb6b0d6c86e5717f485c0605c296834e88acdeaf1300"
    ); 
}
*/

TEST(DeepOnionTransaction, LockScripts) {
    // P2PKH    
    // tx - https://chainz.cryptoid.info/onion/tx.dws?2743254.htm 69a4b0b8641c16268033667371e71ed1b0c1a48bbb09831c61398c9c47978d0e
    
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("DohbWhdFgC4KEr4SM7vBSuhQ3hNqzNsLhs").get(), TWCoinTypeDeepOnion));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914d2dac77deb6b0d6c86e5717f485c0605c296834e88ac");

    // P2SH
    // https://chainz.cryptoid.info/onion/tx.dws?61dd3d18819ac9718137cfaa05f0bdb9f3b75378080331f87fd17c1d4023e64e.htm

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("DbmhitYuDmTLcL7KTDZzVTURfim3VACuzD").get(), TWCoinTypeDeepOnion));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a91450001e1fe61c95973acdc8aa4807f10632d537b888ac");
}
