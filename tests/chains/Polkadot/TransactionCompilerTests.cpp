// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Polkadot.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Polkadot::tests {

TEST(PolkadotCompiler, CompileWithSignatures) {
    /// Prepare transaction input (protobuf)
    const auto coin = TWCoinTypePolkadot;
    auto input = Polkadot::Proto::SigningInput();
    auto block_hash = parse_hex("40cee3c3b7f8422f4c512e9ebebdeeff1c28e81cc678ee4864d945d641e05f9b");
    auto genesis_hash = parse_hex("91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3");
    input.set_block_hash(block_hash.data(), block_hash.size());
    input.set_genesis_hash(genesis_hash.data(), genesis_hash.size());
    input.set_nonce(0);
    input.set_spec_version(25);
    input.set_transaction_version(5);
    input.set_network(ss58Prefix(TWCoinTypePolkadot));

    auto era = input.mutable_era();
    era->set_block_number(5898150);
    era->set_period(10000);

    auto call = input.mutable_balance_call();
    auto tx = call->mutable_transfer();
    auto value = parse_hex("210fdc0c00");
    tx->set_to_address("15JWiQUmczAFU3hrZrD2gDyuJdL2BbFaX9yngivb1UWiBJWA");
    tx->set_value(value.data(), value.size());

    auto txInputData = data(input.SerializeAsString());
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    auto preOutput = TxCompiler::Proto::PreSigningOutput();
    preOutput.ParseFromArray(preImageHashes.data(), (int)preImageHashes.size());
    auto preImageHash = preOutput.data_hash();

    {
        EXPECT_EQ(hex(preImageHash), "0500be4c21aa92dcba057e9b719ce1de970f774f064c09b13a3ea3009affb8cb5ec707000cdc0f219dfe0000190000000500000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c340cee3c3b7f8422f4c512e9ebebdeeff1c28e81cc678ee4864d945d641e05f9b");
    }

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = parse_hex("d84accbb64934815506288fafbfc7d275e64aa4e3cd9c5392db6e83b13256bf3");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature = parse_hex("fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a09");

    // Verify signature (pubkey & hash & signature)
    {
        EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    }

    const auto expectedTx = "390284d84accbb64934815506288fafbfc7d275e64aa4e3cd9c5392db6e83b13256bf300fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a099dfe00000500be4c21aa92dcba057e9b719ce1de970f774f064c09b13a3ea3009affb8cb5ec707000cdc0f21";
    {
        /// Compile transaction info
        const Data outputData = TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKeyData});
        auto output = Polkadot::Proto::SigningOutput();
        output.ParseFromArray(outputData.data(), (int)outputData.size());
        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Polkadot::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Polkadot::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

} // namespace TW::Polkadot::tests
