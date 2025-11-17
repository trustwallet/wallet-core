// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "TransactionCompiler.h"

#include "proto/Tezos.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Tezos;

TEST(TezosCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeTezos;

    // Test key
    auto privateKey =
        PrivateKey(parse_hex("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));

    // This is the forged (non-base58) reveal public key bytes used in older golden vectors.
    // It corresponds to edpk "edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A"
    auto revealKey = parse_hex(
        "311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff95");

    // -------- Step 1: Prepare transaction input (protobuf)
    TW::Tezos::Proto::SigningInput input;
    auto& operations = *input.mutable_operation_list();
    operations.set_branch("BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp");

    // REVEAL (must come before any other manager ops for the same source)
    auto& reveal = *operations.add_operations();
    auto& revealData = *reveal.mutable_reveal_operation_data();
    revealData.set_public_key(revealKey.data(), revealKey.size());
    reveal.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    reveal.set_fee(1272);
    reveal.set_counter(30738);
    reveal.set_gas_limit(10100);
    reveal.set_storage_limit(257);
    reveal.set_kind(Tezos::Proto::Operation::REVEAL);

    // TRANSACTION
    auto& tx = *operations.add_operations();
    auto& txData = *tx.mutable_transaction_operation_data();
    txData.set_amount(1);
    txData.set_destination("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    tx.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    tx.set_fee(1272);
    tx.set_counter(30739);
    tx.set_gas_limit(10100);
    tx.set_storage_limit(257);
    tx.set_kind(Tezos::Proto::Operation::TRANSACTION);

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    // -------- Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    TW::TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    // This expected hash should remain stable given the fixed inputs above.
    EXPECT_EQ(hex(preImageHash),
              "2268aec5a2becbcd784a03060a4816e20396e6d96e7839ddaf2c3b980632e591");

    // Produce a proper 64-byte Ed25519 signature (R||S) over the preimage hash.
    Data digest(preImageHash.begin(), preImageHash.end());
    auto signature = privateKey.sign(digest, TWCurveED25519);
    ASSERT_EQ(signature.size(), 64ul);

    // -------- Step 3: Compile transaction using that signature
    auto compiledData = TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {});
    Proto::SigningOutput compiledOutput;
    ASSERT_TRUE(compiledOutput.ParseFromArray(compiledData.data(), (int)compiledData.size()));
    const auto compiledHex = hex(compiledOutput.encoded());

    // As a cross-check, sign the same input with AnySigner and compare full bytes.
    TW::Tezos::Proto::SigningInput signingInput;
    ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
    signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    {
        Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);
        const auto anySignHex = hex(output.encoded());

        ASSERT_EQ(compiledHex, anySignHex);
    }

    // -------- Structural Seoul check:
    // Ensure the reveal has the new presence_of_proof byte (0x00) after the forged public key,
    // followed by the transaction op tag (0x6c).
    {
        // Reveal tag = 0x6b, Transaction tag = 0x6c
        ASSERT_NE(compiledHex.find("6b"), std::string::npos) << "Reveal tag not found";

        const std::string forgedPubKeyHex = hex(revealKey); // already raw forged key bytes
        auto pkPos = compiledHex.find(forgedPubKeyHex);
        ASSERT_NE(pkPos, std::string::npos) << "Forged public key not found in reveal";

        // presence_of_proof = 0x00 must be immediately after the public key bytes
        ASSERT_EQ(compiledHex.substr(pkPos + forgedPubKeyHex.size(), 2), "00");

        // and the next op tag should be 0x6c (transaction)
        ASSERT_EQ(compiledHex.substr(pkPos + forgedPubKeyHex.size() + 2, 2), "6c");
    }

    // -------- Negative: inconsistent signatures & public keys count
    {
        auto compiledBad = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {});
        Proto::SigningOutput badOutput;
        ASSERT_TRUE(badOutput.ParseFromArray(compiledBad.data(), (int)compiledBad.size()));
        EXPECT_EQ(badOutput.encoded().size(), 0ul);
        EXPECT_EQ(badOutput.error(), Common::Proto::Error_signatures_count);
    }
}
