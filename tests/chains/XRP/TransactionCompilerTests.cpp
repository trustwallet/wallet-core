// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Ripple.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Ripple::tests {

TEST(RippleCompiler, CompileRippleWithSignatures) {
    const auto coin = TWCoinTypeXRP;
    /// Step 1: Prepare transaction input (protobuf)
    auto key = parse_hex("acf1bbf6264e699da0cc65d17ac03fcca6ded1522d19529df7762db46097ff9f");
    auto input = TW::Ripple::Proto::SigningInput();
    auto privateKey = TW::PrivateKey(key);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    input.mutable_op_payment()->set_amount(1000000);
    input.set_fee(10);
    input.set_sequence(75674534);
    input.set_last_ledger_sequence(75674797);
    input.set_account("rGV1v1xw23PHcRn4Km4tF8R2mfh6yTZkcP");
    input.mutable_op_payment()->set_destination("rNLpgsBTCwiaZAnHe2ZViAN1GcXZtYW6rg");
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(hex(preImage), "535458001200002200000000240482b3a6201b0482b4ad6140000000000f424068400000000000000a7321027efc5f15071d2ae5e73ee09a0c17456c5d9170a41d67e3297c554829199be80b8114aa000c09c692ef1f82787e51e22833149941ea2083149232ef60695add51f0f84534cc4084e4fdfc698e");
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash), "86ef78df7a4aad29e6b3730f7965c1bd5ccd2439426cb738d7c494a64cfaf4af");
    // Simulate signature, normally obtained from signature server
    const auto signature = privateKey.signAsDER(TW::data(preImageHash));
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verifyAsDER(signature, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKey.bytes});
    const auto ExpectedTx = std::string("1200002200000000240482b3a6201b0482b4ad6140000000000f424068400000000000000a7321027efc5f15071d2ae5e73ee09a0c17456c5d9170a41d67e3297c554829199be80b74473045022100e1c746c3aeebc8278c627ee4c2ce5cae97e3856292c7fe5388f803920230a37b02207d2eccb76cd35dd379d6b24c2cabd786e62d34a564cf083e863176109c5b6bb48114aa000c09c692ef1f82787e51e22833149941ea2083149232ef60695add51f0f84534cc4084e4fdfc698e");

    {
        TW::Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
        ASSERT_EQ(output.error(), TW::Common::Proto::SigningError::OK);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Ripple::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(key.data(), key.size());

        TW::Ripple::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: not enough signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

} // namespace TW::Ripple::tests
