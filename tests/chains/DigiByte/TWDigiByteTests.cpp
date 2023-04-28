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

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(DigiByteTransaction, SignTransaction) {
    /*
        https://iancoleman.io/bip39/
        Mnemonic - shoot island position soft burden budget tooth cruel issue economy destroy above
        m/44'/20'/0'/0/0 Private key in Base58 encoding - L37s29UXmQowW7y5BeDhe8AwM7quvRS2FmkWxSuctw9PbFprhhqA
        m/44'/20'/0'/0/0 Private key in bytes - b00420bab8b663f0870ee8e46435743ba9588eb88d8d31410ed54afa67602e8d
        https://dgb2.trezor.io/tx/6eb7e134cafbe4030032aa9f44a14420826e8d290cddff1a53339c3916b19f30
    */

    const int64_t utxo_amount = 105000000;
    const int64_t amount = 101000000;
    const int64_t fee = 1000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_coin_type(TWCoinTypeDigiByte);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("D9Gv7jWSVsS9Y5q98C79WyfEj6P2iM5Nzs");
    input.set_change_address("DBfCffUdSbhqKZhjuvrJ6AgvJofT4E2kp4");
    input.set_coin_type(TWCoinTypeDigiByte);

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

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto result = Bitcoin::TransactionSigner<Bitcoin::Transaction, Bitcoin::TransactionBuilder>::sign(Bitcoin::SigningInput(input));
    auto signedTx = result.payload();
    ASSERT_TRUE(result);

    Data serialized;
    signedTx.encode(serialized, Bitcoin::Transaction::SegwitFormatMode::NonSegwit);
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
        "4023050600000000"
        "19"
        "76a9142d5b215a11029ee51a1dd9404d271c7e4a74f5f288ac"
        "18053d0000000000"
        "19"
        "76a91447825943ca6a936b177fdc7c9dc05251640169c288ac"
        "00000000");
}

TEST(DigiByteTransaction, SignP2WPKH) {
    /*
        https://iancoleman.io/bip39/
        Mnemonic - shoot island position soft burden budget tooth cruel issue economy destroy above
        m/84'/20'/0'/0/1 Private key in Base58 encoding - L3Fy66STjMEzBoQSHLpQbpnLjGywHcjPwZHJ1vKs51pYD25HH49S
        m/84'/20'/0'/0/1 Private key in bytes - b42f799279ef32df2d8e14314025a05b0c86622aaae66fc1d8ed77a20ec3d2c5
        https://dgb2.trezor.io/tx/4c9d48df9cc5a047b62a0ec5a19bbf1f88aa60faffab554dcc8fe833e4ff6080
    */

    const int64_t utxo_amount = 3980000;
    const int64_t amount = 2000000;

    Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeDigiByte);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("dgb1qtjgmerfqwdffyf8ghcrkgy52cghsqptynmyswu");
    input.set_change_address("dgb1qfd3xjn8a6776ckwta5s39zxv6hqd40gz4kgf6j");

    auto utxoKey0 = parse_hex("b42f799279ef32df2d8e14314025a05b0c86622aaae66fc1d8ed77a20ec3d2c5");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Bitcoin::Script(parse_hex("00144b62694cfdd7bdac59cbed211288ccd5c0dabd02"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(utxo_amount);
    auto hash0 = parse_hex("80a16412a880d13b0c88929397a50341018da2e78b70b313062b4a496fea5940");
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    auto result = Bitcoin::TransactionSigner<Bitcoin::Transaction, Bitcoin::TransactionBuilder>::sign(input);
    ASSERT_TRUE(result) << std::to_string(result.error());
    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(serialized);
    ASSERT_EQ(hex(serialized), "0100000000010180a16412a880d13b0c88929397a50341018da2e78b70b313062b4a496fea59400100000000ffffffff0280841e00000000001600145c91bc8d2073529224e8be0764128ac22f000564d3351e00000000001600144b62694cfdd7bdac59cbed211288ccd5c0dabd0202473044022057b876880b6c98511d9e5baab00428c50bf96868bdf4dc50bd61c2477ed8438b0220312ff89a078ab5a38b7b909ceb58310d93a5b4e2d637b37b77c4d7baf35a1815012102ac2e56f40d38530fcbf21f1eba0c3c668aa839cda8f2c615e99df44b6447772600000000");
}

TEST(DigiByteTransaction, LockScripts) {
    // https://dgb2.trezor.io/tx/966b80caa754148158339a0fa70363996f15819599adf72de0eb3590c3bd63ea

    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("DBfCffUdSbhqKZhjuvrJ6AgvJofT4E2kp4").get(), TWCoinTypeDigiByte));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a91447825943ca6a936b177fdc7c9dc05251640169c288ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("dgb1q3p2nf26ac6qtdrv4czh5nmp2eshfj9wyn9vv3d").get(), TWCoinTypeDigiByte));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "0014885534ab5dc680b68d95c0af49ec2acc2e9915c4");

    // https://dgb2.trezor.io/tx/965eb4afcd0aa6e3f4f8fc3513ca042f09e6e2235367fa006cbd1f546c293a2a

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("SUngTA1vaC2E62mbnc81Mdos3TcvZHwsVo").get(), TWCoinTypeDigiByte));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "a91452356ed3d2d31eb8b263ace5d164e3cf3b37096687");
}

} // namespace TW::Bitcoin
