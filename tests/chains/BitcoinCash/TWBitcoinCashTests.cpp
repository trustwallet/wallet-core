// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Bitcoin/Address.h"
#include "Bitcoin/SigHashType.h"
#include "HexCoding.h"
#include "proto/Bitcoin.pb.h"
#include "proto/BitcoinV2.pb.h"
#include "TestUtilities.h"

#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Bitcoin::tests {

// clang-format off
TEST(BitcoinCash, Address) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("pqx578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc").get(), TWCoinTypeBitcoinCash));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("bitcoincash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc").get(), TWCoinTypeBitcoinCash));
}

TEST(BitcoinCash, ValidAddress) {
    auto string = STRING("bitcoincash:qqa2qx0d8tegw32xk8u75ws055en4x3h2u0e6k46y4");
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeBitcoinCash));
    ASSERT_NE(address.get(), nullptr);

    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(string.get(), TWCoinTypeBitcoinCash));
    ASSERT_FALSE(TWBitcoinScriptSize(script.get()) == 0);
}

TEST(BitcoinCash, InvalidAddress) {
    // Wrong checksum
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("pqx578nanz2h2estzmkr53zqdg6qt8xyqvffffffff").get(), TWCoinTypeBitcoinCash));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("bitcoincash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvffffffff").get(), TWCoinTypeBitcoinCash));

    // Valid eCash addresses are invalid for BCH
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0").get(), TWCoinTypeECash));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0").get(), TWCoinTypeBitcoinCash));

    EXPECT_TRUE(TWAnyAddressIsValid(STRING("ecash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0").get(), TWCoinTypeECash));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("ecash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvh683mrz0").get(), TWCoinTypeBitcoinCash));

    // Wrong prefix
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("bcash:pqx578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc").get(), TWCoinTypeBitcoinCash));

    // Wrong base 32 (characters o, i)
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("poi578nanz2h2estzmkr53zqdg6qt8xyqvwhn6qeyc").get(), TWCoinTypeBitcoinCash));
}

TEST(BitcoinCash, LegacyToCashAddr) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("28071bf4e2b0340db41b807ed8a5514139e5d6427ff9d58dbd22b7ed187103a4").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(publicKey.get(), 0));
    auto addressString = WRAPS(TWBitcoinAddressDescription(address.get()));
    assertStringsEqual(addressString, "1PeUvjuxyf31aJKX6kCXuaqxhmG78ZUdL1");

    auto cashAddress = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeBitcoinCash));
    auto cashAddressString = WRAPS(TWAnyAddressDescription(cashAddress.get()));
    assertStringsEqual(cashAddressString, "bitcoincash:qruxj7zq6yzpdx8dld0e9hfvt7u47zrw9gfr5hy0vh");
}

TEST(BitcoinCash, LockScript) {
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("bitcoincash:qpk05r5kcd8uuzwqunn8rlx5xvuvzjqju5rch3tc0u").get(), TWCoinTypeBitcoinCash));
    auto data = WRAPD(TWAnyAddressData(address.get()));
    auto rawData = WRAPD(TWDataCreateWithSize(0));
    TWDataAppendByte(rawData.get(), 0x00);
    TWDataAppendData(rawData.get(), data.get());

    auto legacyAddress = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithData(rawData.get()));
    auto legacyString = WRAPS(TWBitcoinAddressDescription(legacyAddress.get()));
    assertStringsEqual(legacyString, "1AwDXywmyhASpCCFWkqhySgZf8KiswFoGh");

    auto keyHash = WRAPD(TWDataCreateWithBytes(legacyAddress.get()->impl.bytes.data() + 1, 20));
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildPayToPublicKeyHash(keyHash.get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a9146cfa0e96c34fce09c0e4e671fcd43338c14812e588ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("pzukqjmcyzrkh3gsqzdcy3e3d39cqxhl3g0f405k5l").get(), TWCoinTypeBitcoinCash));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a914b9604b7820876bc510009b8247316c4b801aff8a87");
}

TEST(BitcoinCash, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeBitcoinCash, TWHDVersionXPRV));
    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeBitcoinCash, TWHDVersionXPUB));

    assertStringsEqual(xprv, "xprv9yEvwSfPanK5gLYVnYvNyF2CEWJx1RsktQtKDeT6jnCnqASBiPCvFYHFSApXv39bZbF6hRaha1kWQBVhN1xjo7NHuhAn5uUfzy79TBuGiHh");
    assertStringsEqual(xpub, "xpub6CEHLxCHR9sNtpcxtaTPLNxvnY9SQtbcFdov22riJ7jmhxmLFvXAoLbjHSzwXwNNuxC1jUP6tsHzFV9rhW9YKELfmR9pJaKFaM8C3zMPgjw");
}

TEST(BitcoinCash, DeriveFromXPub) {
    auto xpub = STRING("xpub6CEHLxCHR9sNtpcxtaTPLNxvnY9SQtbcFdov22riJ7jmhxmLFvXAoLbjHSzwXwNNuxC1jUP6tsHzFV9rhW9YKELfmR9pJaKFaM8C3zMPgjw");
    auto pubKey2 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeBitcoinCash, STRING("m/44'/145'/0'/0/2").get()));
    auto pubKey9 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeBitcoinCash, STRING("m/44'/145'/0'/0/9").get()));

    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey2.get(), TWCoinTypeBitcoinCash));
    auto address2String = WRAPS(TWAnyAddressDescription(address2.get()));

    auto address9 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey9.get(), TWCoinTypeBitcoinCash));
    auto address9String = WRAPS(TWAnyAddressDescription(address9.get()));

    assertStringsEqual(address2String, "bitcoincash:qq4cm0hcc4trsj98v425f4ackdq7h92rsy6zzstrgy");
    assertStringsEqual(address9String, "bitcoincash:qqyqupaugd7mycyr87j899u02exc6t2tcg9frrqnve");
}

TEST(BitcoinCash, SignTransaction) {
    const int64_t amount = 600;

    // Transaction on Bitcoin Cash Mainnet
    // https://blockchair.com/bitcoin-cash/transaction/96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4

    auto input = Proto::SigningInput();
    input.set_hash_type(hashTypeForCoin(TWCoinTypeBitcoinCash));
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    input.set_change_address("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU");

    auto hash0 = DATA("e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(2);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(5151);
    auto script0 = parse_hex("76a914aff1e0789e5fe316b729577665aa0a04d5b0f8c788ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    // Sign
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeBitcoinCash);

    EXPECT_EQ(output.transaction().outputs_size(), 2);
    EXPECT_EQ(output.transaction().outputs(0).value(), amount);
    EXPECT_EQ(output.transaction().outputs(1).value(), 4325);
    EXPECT_EQ(output.encoded().length(), 226ul);
    ASSERT_EQ(hex(output.encoded()),
        "01000000"
        "01"
            "e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05" "02000000" "6b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5b" "ffffffff"
        "02"
            "5802000000000000" "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac"
            "e510000000000000" "1976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac"
        "00000000");
}

TEST(BitcoinCash, SignTransactionV2) {
    auto privateKey = parse_hex("7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384");
    auto txId = parse_hex("050d00e2e18ef13969606f1ceee290d3f49bd940684ce39898159352952b8ce2");
    std::reverse(txId.begin(), txId.end());

    BitcoinV2::Proto::SigningInput signing;
    signing.add_private_keys(privateKey.data(), privateKey.size());
    signing.mutable_chain_info()->set_p2pkh_prefix(0);
    signing.mutable_chain_info()->set_p2sh_prefix(5);
    signing.mutable_chain_info()->set_hrp("bitcoincash");

    auto& builder = *signing.mutable_builder();
    builder.set_version(BitcoinV2::Proto::TransactionVersion::V1);
    builder.set_input_selector(BitcoinV2::Proto::InputSelector::UseAll);
    builder.set_fixed_dust_threshold(546);

    auto& in = *builder.add_inputs();
    auto& inOutPoint = *in.mutable_out_point();
    inOutPoint.set_hash(txId.data(), txId.size());
    inOutPoint.set_vout(2);
    in.set_value(5151);
    // Cash address without prefix.
    in.set_receiver_address("qzhlrcrcne07x94h99thved2pgzdtv8ccujjy73xya");
    in.set_sighash_type(TWBitcoinSigHashTypeAll | TWBitcoinSigHashTypeFork);

    auto& out0 = *builder.add_outputs();
    out0.set_value(600);
    // Legacy address.
    out0.set_to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");

    auto& explicitChangeOutput = *builder.add_outputs();
    explicitChangeOutput.set_value(4325);
    // Cash address with an explicit prefix.
    explicitChangeOutput.set_to_address("bitcoincash:qz0q3xmg38sr94rw8wg45vujah7kzma3cskxymnw06");

    Proto::SigningInput legacy;
    *legacy.mutable_signing_v2() = signing;
    legacy.set_coin_type(TWCoinTypeBitcoinCash);

    Proto::TransactionPlan plan;
    ANY_PLAN(legacy, plan, TWCoinTypeBitcoin);

    ASSERT_EQ(plan.error(), Common::Proto::SigningError::OK);
    const auto planV2 = plan.planning_result_v2();
    EXPECT_EQ(planV2.error(), Common::Proto::SigningError::OK) << planV2.error_message();

    EXPECT_EQ(planV2.inputs_size(), 1);
    EXPECT_EQ(planV2.outputs_size(), 2);
    EXPECT_EQ(planV2.vsize_estimate(), 227ul);
    EXPECT_EQ(planV2.fee_estimate(), 226);
    EXPECT_EQ(planV2.change(), 0);

    Proto::SigningOutput output;
    ANY_SIGN(legacy, TWCoinTypeBitcoin);

    EXPECT_EQ(output.error(), Common::Proto::OK);
    ASSERT_TRUE(output.has_signing_result_v2());
    const auto outputV2 = output.signing_result_v2();
    EXPECT_EQ(outputV2.error(), Common::Proto::SigningError::OK) << outputV2.error_message();
    ASSERT_EQ(hex(outputV2.encoded()),
              "01000000"
              "01"
              "e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05" "02000000" "6b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5b" "ffffffff"
              "02"
              "5802000000000000" "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac"
              "e510000000000000" "1976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac"
              "00000000");
}

// clang-format on

} // namespace TW::Bitcoin::tests
