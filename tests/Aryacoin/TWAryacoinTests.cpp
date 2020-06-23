// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>
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

TEST(TWAryacoinScript, LockScriptTest) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("AWWMxHsQUi58cSePL4EbBcodbUMZ1H1KNf").get(), TWCoinTypeAryacoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914a19adc557be2e08464cb18a27ddeb7453fdb8b5a88ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("MAaCCYVj2yUoW1MtvLWi3xA1cdJhESpVJh").get(), TWCoinTypeAryacoin));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a9141d521dcf4983772b3c1e6ef937103ebdfaa1ad7787");
}

TEST(AryacoinKey, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("salute result glow ride require lonely dragon sauce prevent crack fit buyer").get(),
        STRING("").get()
    ));

    // .bip44
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeAryacoin, TWHDVersionXPRV));
    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeAryacoin, TWHDVersionXPUB));

    assertStringsEqual(xprv, "xprv9zBXceFtcPTzZYmET4CBaSa3qrg6nLKEa3dmPYkevRwej3JHkszV3zhL7RkH3zj9N1fnvZuCrtuz1AnawyHx7riEUEumqghwT9hcfziYJQ9");
    assertStringsEqual(xpub, "xpub6DAt29nnSm2Hn2qhZ5jBwaWnPtWbBo35wGZNBwAGUmUdbqdSJRJjbo1oxiRjqamXJHJHjLuSut9Ke9f9vToXzUS8g7Rd9J72PKFptEe6HDe");
}

TEST(AryacoinKey, DeriveFromXPub) {
    auto xpub = STRING("xpub6DAt29nnSm2Hn2qhZ5jBwaWnPtWbBo35wGZNBwAGUmUdbqdSJRJjbo1oxiRjqamXJHJHjLuSut9Ke9f9vToXzUS8g7Rd9J72PKFptEe6HDe");
    auto pubKey2 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/357'/0'/0/2").get());
    auto pubKey9 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/357'/0'/0/9").get());

    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey2, TWCoinTypeAryacoin));
    auto address2String = WRAPS(TWAnyAddressDescription(address2.get()));

    auto address9 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey9, TWCoinTypeAryacoin));
    auto address9String = WRAPS(TWAnyAddressDescription(address9.get()));

    assertStringsEqual(address2String, "ANGMJxMPAgyGw5b9JroLA3vH4CH5nWAwKD");
    assertStringsEqual(address9String, "APg8kZBdeJhHRdUTLFfn7kgRnHSu9KKq3L");
}

TEST(AryacoinTransaction, SignTransaction) {
    /*
        https://iancoleman.io/bip39/
        Mnemonic - shoot island position soft burden budget tooth cruel issue economy destroy above
        m/44'/357'/0'/0/0 Address - AWWMxHsQUi58cSePL4EbBcodbUMZ1H1KNf
        m/44'/357'/0'/0/0 Private key in Base58 encoding - PNi3P7ap1UDARV9bKqcmdVBcsxd7SACfPRN935AdnWnMERnhhDVU
         m/44'/357'/0'/0/0 Private key in bytes 403769C2CC212A04B13830EA108DCABF8EC87DDD1DCBB1E9BB8F6E539649DE8E
        utxo - https://explorer2.aryacoin.io/tx/fc381233e1d7eb42fe675f112fef8ff36b7f7796dc9a9a04e9dac5fa41499e0d
        tx - https://explorer2.aryacoin.io/tx/c851e5692e28af0176cef24aec61b30f76073e52d2c41849c69cf8ad3dc8213f
    */

    const int64_t utxo_amount = 999774000;
    const int64_t amount = 100000000;
    const int64_t fee = 226000;

    auto input = Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("AVsR9tT1XjrgrU1S2MaisATakwdkJvdkcg");
    input.set_change_address("AWWMxHsQUi58cSePL4EbBcodbUMZ1H1KNf");
    input.set_coin_type(TWCoinTypeAryacoin);

    auto hash0 = DATA("0d9e4941fac5dae9049a9adc96777f6bf38fef2f115f67fe42ebd7e1331238fc");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("76a914a19adc557be2e08464cb18a27ddeb7453fdb8b5a88ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("403769C2CC212A04B13830EA108DCABF8EC87DDD1DCBB1E9BB8F6E539649DE8E");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    auto plan = TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = utxo_amount - amount - fee;

    auto &protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto signer = TransactionSigner<Transaction, TransactionBuilder>(std::move(input));
    auto result = signer.sign();
    auto signedTx = result.payload();

    ASSERT_TRUE(result);
    ASSERT_EQ(fee, signer.plan.fee);

    Data serialized;
    signedTx.encode(serialized, Transaction::SegwitFormatMode::NonSegwit);
    ASSERT_EQ(
        hex(serialized),
        "01000000010d9e4941fac5dae9049a9adc96777f6bf38fef2f115f67fe42ebd7e1331238fc000000006a47304402205f469bb9d003a13eb197e798f9dca482033541c8dc248633ff39515114606d9702205a6f1502553f544f1e49d1ff50fc15038f67b630afd5f51b3bfc69a50649995f01210266c009798a9b5d9f2ad03c4646cef09e8697de074900e4a38382fb83324c45f6ffffffff0200e1f505000000001976a9149a9e2429df2a3e11c978e64be20cf5c6465dbf8488ac60039e35000000001976a914a19adc557be2e08464cb18a27ddeb7453fdb8b5a88ac00000000"
    ); 
}