// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Tezos.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TezosCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeTezos;

    /// Step 1: Prepare transaction input (protobuf)
    auto privateKey =
        PrivateKey(parse_hex("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));
    auto publicKey = privateKey.getPublicKey(::publicKeyType(coin));
    auto revealKey = parse_hex("311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff95");

    TW::Tezos::Proto::SigningInput input;
    auto& operations = *input.mutable_operation_list();
    operations.set_branch("BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp");

    auto& reveal = *operations.add_operations();
    auto& revealData = *reveal.mutable_reveal_operation_data();
    revealData.set_public_key(revealKey.data(), revealKey.size());
    reveal.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    reveal.set_fee(1272);
    reveal.set_counter(30738);
    reveal.set_gas_limit(10100);
    reveal.set_storage_limit(257);
    reveal.set_kind(Tezos::Proto::Operation::REVEAL);

    auto& transaction = *operations.add_operations();
    auto& txData = *transaction.mutable_transaction_operation_data();
    txData.set_amount(1);
    txData.set_destination("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transaction.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transaction.set_fee(1272);
    transaction.set_counter(30739);
    transaction.set_gas_limit(10100);
    transaction.set_storage_limit(257);
    transaction.set_kind(Tezos::Proto::Operation::TRANSACTION);

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "12e4f8b17ad3b316a5a56960db76c7d6505dbf2fff66106be75c8d6753daac0e");

    auto signature = parse_hex("0217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c987"
                               "74cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05");

    /// Step 3: Compile transaction info
    const auto tx =
        "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35f"
        "cc8c90dfa3b0b95721f80992f001f44e810200311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10"
        "906f511cff956c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f74"
        "1ef614b0e35fcc8c90dfa3b0b95721000217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a1"
        "0db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05";
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {});

    {
        TW::Tezos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), tx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Tezos::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Tezos::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), tx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {});
        Tezos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
    }
}