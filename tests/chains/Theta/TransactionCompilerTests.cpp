// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"
#include "uint256.h"

#include "proto/Theta.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(ThetaCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeTheta;
    /// Step 1: Prepare transaction input (protobuf)
    const auto pkFrom =
        PrivateKey(parse_hex("0x93a90ea508331dfdf27fb79757d4250b4e84954927ba0073cd67454ac432c737"));
    const auto publicKey = pkFrom.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    TW::Theta::Proto::SigningInput input;
    input.set_chain_id("privatenet");
    input.set_to_address("0x9F1233798E905E173560071255140b4A8aBd3Ec6");
    auto amount = store(uint256_t(10));
    input.set_theta_amount(amount.data(), amount.size());
    auto tfuelAmount = store(uint256_t(20));
    input.set_tfuel_amount(tfuelAmount.data(), tfuelAmount.size());
    auto fee = store(uint256_t(1000000000000));
    input.set_fee(fee.data(), fee.size());
    input.set_sequence(1);
    std::string pubkeyStr(publicKey.bytes.begin(), publicKey.bytes.end());
    input.set_public_key(pubkeyStr);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    EXPECT_EQ(hex(preSigningOutput.data_hash()),
              "2dc419e9919e65f129453419dc72a6bee99b2281dfddf754807a5c212ae35678");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature =
        parse_hex("5190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134ccdef0"
                  "53ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preSigningOutput.data_hash())));

    /// Step 3: Compile transaction info
    auto expectedTx = "02f887c78085e8d4a51000f863f861942e833968e5bb786ae419c4d13189fb081cc43babc70a"
                      "85e8d4a5101401b8415190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb"
                      "7fff267cb8fe9134ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501d9d8"
                      "949f1233798e905e173560071255140b4a8abd3ec6c20a14";
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});

    {
        Theta::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        Theta::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(pkFrom.bytes.data(), pkFrom.bytes.size());

        Theta::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKeyData});
        Theta::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }
}
