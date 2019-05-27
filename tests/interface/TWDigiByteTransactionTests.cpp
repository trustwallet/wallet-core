
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
    Mnemonic - shoot island position soft burden budget tooth cruel issue economy destroy above
    m/44'/20'/0'/0/0 Private key in Base58 encoding - L37s29UXmQowW7y5BeDhe8AwM7quvRS2FmkWxSuctw9PbFprhhqA
    m/44'/20'/0'/0/0 Private key in bytes - b00420bab8b663f0870ee8e46435743ba9588eb88d8d31410ed54afa67602e8d

    https://dgb2.trezor.io/tx/6eb7e134cafbe4030032aa9f44a14420826e8d290cddff1a53339c3916b19f30
*/
TEST(DigiByteTransaction, SignTransaction) {
    const int64_t utxo_amount = 105000000;
    const int64_t amount = 101000000;
    const int64_t fee = 1000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWSignatureHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("D9Gv7jWSVsS9Y5q98C79WyfEj6P2iM5Nzs");
    input.set_change_address("DBfCffUdSbhqKZhjuvrJ6AgvJofT4E2kp4");

    auto hash0 = DATA("ea63bdc39035ebe02df7ad999581156f996303a70f9a3358811454a7ca806b96");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("76a91447825943ca6a936b177fdc7c9dc05251640169c288ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("b00420bab8b663f0870ee8e46435743ba9588eb88d8d31410ed54afa67602e8d");
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
            "ea63bdc39035ebe02df7ad999581156f996303a70f9a3358811454a7ca806b96"
            "00000000"
            "6a"
            "473044022003e9756b12ecbe5788fdb6eb4b6d7b58f9f9410df32f3047edb0dd0ebffb0d630220499d00d17e50c48b4bac6c0ce148f13bb3109a8845fa3400a2d6a57dabf2c4010121024e525e582452cece7b869532d9e354cfec58b71cbed76f7238c91274a64b2116"
            "ffffffff"
        "02"
            "4023050600000000""19"
            "76a9142d5b215a11029ee51a1dd9404d271c7e4a74f5f288ac"
            "18053d0000000000""19"
            "76a91447825943ca6a936b177fdc7c9dc05251640169c288ac"
        "00000000"
    ); 
}