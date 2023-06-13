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

#include "proto/Oasis.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Oasis::tests {

TEST(OasisCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeOasis;
    /// Step 1: Prepare transaction input (protobuf)
    auto privateKey = PrivateKey(parse_hex("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0"));
    auto publicKey = privateKey.getPublicKey(publicKeyType(coin));

    auto input = TW::Oasis::Proto::SigningInput();
    auto& transfer = *input.mutable_transfer();
    transfer.set_gas_price(0);
    transfer.set_gas_amount("0");
    transfer.set_nonce(0);
    transfer.set_to("oasis1qrrnesqpgc6rfy2m50eew5d7klqfqk69avhv4ak5");
    transfer.set_amount("10000000");
    transfer.set_context("oasis-core/consensus: tx for chain a245619497e580dd3bc1aa3256c07f68b8dcc13f92da115eadc3b231b083d3c4");

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash), "373976e01fa0634a40ce8898a869f1056d862e3a0f26d8ae22ebeb5fdbcde9b3");

    auto signature = parse_hex("6e51c18c9b2015c9b49414b3307336597f51ff331873d214ce2db81c9651a34d99529ccaa294a39ccd01c6b0bc2c2239d87c624e5ba4840cf99ac8f9283e240c");

    /// Step 3: Compile transaction info
    const auto expectedTx = "a2697369676e6174757265a2697369676e617475726558406e51c18c9b2015c9b49414b3307336597f51ff331873d214ce2db81c9651a34d99529ccaa294a39ccd01c6b0bc2c2239d87c624e5ba4840cf99ac8f9283e240c6a7075626c69635f6b6579582093d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b73756e747275737465645f7261775f76616c7565585ea463666565a2636761730066616d6f756e74410064626f6479a262746f5500c73cc001463434915ba3f39751beb7c0905b45eb66616d6f756e744400989680656e6f6e636500666d6574686f64707374616b696e672e5472616e73666572";
    auto outputData = TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKey.bytes});

    {
        TW::Oasis::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        TW::Oasis::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Oasis::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        Oasis::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        Oasis::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

} // namespace TW::Oasis::tests
