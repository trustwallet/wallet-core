
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
    https://digiexplorer.info/tx/2e725346df28cff3275c4087f2f365e041c6fb75d10bbd6c095abdd9716db53d
    https://digiexplorer.info/api/tx/2e725346df28cff3275c4087f2f365e041c6fb75d10bbd6c095abdd9716db53d
    https://dgb2.trezor.io/tx/2e725346df28cff3275c4087f2f365e041c6fb75d10bbd6c095abdd9716db53d
*/

TEST(DigiByteTransaction, Signing) {
    const int64_t amount = 100000000;
    const int64_t fee = 1000;
    const int64_t change = 840899000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWSignatureHashTypeFork);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("D7pvLVQyJSqKpYPtpkfydjeFizNrafHcrT");
    input.set_change_address("dgb1qhw8w4g6ny3cjq06vsas9w366yknjzcpr6pawt6");

    auto hash0 = DATA("c9f3b980ce441f3fb517a16e02a05fc4246ee6f8e54651e40414056d2771a522");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(110000000);
    auto script0 = parse_hex("76a91447825943ca6a936b177fdc7c9dc05251640169c288ac");
    utxo0->set_script(script0.data(), script0.size());

    // https://iancoleman.io/bip39/ and https://www.mobilefish.com/download/ethereum/hd_wallet.html
    // Mnemonic - shoot island position soft burden budget tooth cruel issue economy destroy above
    // m/44'/20'/0'/0/0 Private key in Base58 encoding - L37s29UXmQowW7y5BeDhe8AwM7quvRS2FmkWxSuctw9PbFprhhqA
    // m/44'/20'/0'/0/0 Private key in bytes - b00420bab8b663f0870ee8e46435743ba9588eb88d8d31410ed54afa67602e8d
    auto utxoKey0 = DATA("b00420bab8b663f0870ee8e46435743ba9588eb88d8d31410ed54afa67602e8d");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    auto plan = Bitcoin::TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = change;

    // Sign
    auto result = TW::Bitcoin::TransactionSigner<Transaction>(std::move(input), plan).sign();
    ASSERT_TRUE(result) << result.error();
    auto signedTx = result.payload();

    // txid = "2e725346df28cff3275c4087f2f365e041c6fb75d10bbd6c095abdd9716db53d"

    Data serialized;
    signedTx.encode(true, serialized);
    ASSERT_EQ(hex(serialized),
        "0100000001c9f3b980ce441f3fb517a16e02a05fc4246ee6f8e54651e40414056d2771a522000000006a473044022009a77c264da0758b47d8401dadd2803228e557bb83f95e429596ffaa63fe464102207552823b3faff1c7ec45434fd409aa3642b13fb5c47814593dac73f316aece330121024e525e582452cece7b869532d9e354cfec58b71cbed76f7238c91274a64b2116ffffffff02b8191f3200000000160014bb8eeaa3532471203f4c876057475a25a721602300e1f505000000001976a9141d785f831292ad918302633fa98c089fc647324b88ac00000000"
    /*
        "01000000"
        "01"
            "53685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a""00000000""6b483045022100ca0be9f37a4975432a52bb65b25e483f6f93d577955290bb7fb0060a93bfc92002203e0627dff004d3c72a957dc9f8e4e0e696e69d125e4d8e275d119001924d3b48012103b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6""ffffffff"
        "01"
            "4072070000000000""1976a91449964a736f3713d64283fd0018626ba50091c7e988ac"
        "00000000"
        "00000000"
        "0000000000000000"
        "00"
        "00"
        "00"
    */    
    );
}
