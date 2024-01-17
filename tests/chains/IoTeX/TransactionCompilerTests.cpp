// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/IoTeX.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TransactionCompiler, IoTeXCompileWithSignatures) {
    const auto coin = TWCoinTypeIoTeX;

    const auto privateKey0 =
        parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f");
    const auto privateKey1 =
        parse_hex("6021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d");
    const auto pubKey0 =
        parse_hex("034e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c");
    const auto pubKey1 =
        parse_hex("0253ad2f3b734a197f64911242aabc9b5b10bf5744949f5396e56427f35448eafa");
    const auto ExpectedTx0 =
        "0a4c0801107b18f8062203393939523e0a033435361229696f313837777a703038766e686a6a706b79646e7239"
        "37716c68386b683064706b6b797466616d386a1a0c68656c6c6f20776f726c64211241044e18306ae9ef4ec9d0"
        "7bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34f"
        "beb71270d4bad3d648d91a41555cc8af4181bf85c044c3201462eeeb95374f78aa48c67b87510ee63d5e502372"
        "e53082f03e9a11c1e351de539cedf85d8dff87de9d003cb9f92243541541a000";
    const auto ExpectedTx1 =
        "0a4c0801107b18f8062203393939523e0a033435361229696f313837777a703038766e686a6a706b79646e7239"
        "37716c68386b683064706b6b797466616d386a1a0c68656c6c6f20776f726c642112410453ad2f3b734a197f64"
        "911242aabc9b5b10bf5744949f5396e56427f35448eafa84a5d74b49ecb56e011b18c3d5a300e8cff7c6b39d33"
        "0d1d3799c4700a0b1be21a41de4be56ce74dce8e526590f5b5f947385b00947c4c2ead014429aa706a2470055c"
        "56c7e57d1b119b487765d59b21bcdeafac25108f6929a14f9edf4b2309534501";

    const auto prkey0 = PrivateKey(privateKey0);
    const PublicKey pbkey0 = prkey0.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    const auto prkey1 = PrivateKey(privateKey1);
    const PublicKey pbkey1 = prkey1.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::IoTeX::Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(123);
    input.set_gaslimit(888);
    input.set_gasprice("999");
    auto tsf = input.mutable_transfer();
    tsf->set_amount("456");
    tsf->set_recipient("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j");
    auto text = parse_hex("68656c6c6f20776f726c6421"); // "hello world!"
    tsf->set_payload(text.data(), text.size());

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImage = data(preSigningOutput.data());
    auto preImageHash = data(preSigningOutput.data_hash());

    std::string expectedPreImage =
        "0801107b18f8062203393939523e0a033435361229696f313837777a703038766e686a6a706b79646e72393771"
        "6c68386b683064706b6b797466616d386a1a0c68656c6c6f20776f726c6421";
    std::string expectedPreImageHash =
        "0f17cd7f43bdbeff73dfe8f5cb0c0045f2990884e5050841de887cf22ca35b50";
    ASSERT_EQ(hex(preImage), expectedPreImage);
    ASSERT_EQ(hex(preImageHash), expectedPreImageHash);

    Data signature = parse_hex("555cc8af4181bf85c044c3201462eeeb95374f78aa48c67b87510ee63d5e502372e"
                               "53082f03e9a11c1e351de539cedf85d8dff87de9d003cb9f92243541541a000");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(pbkey0.verify(signature, preImageHash));
    /// Step 3: Compile transaction info
    {
        const Data outputData =
            TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {pbkey0.bytes});

        TW::IoTeX::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), ExpectedTx0);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::IoTeX::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        EXPECT_EQ(hex(PrivateKey(privateKey0).getPublicKey(TWPublicKeyTypeSECP256k1).bytes),
                  hex(pubKey0));
        signingInput.set_privatekey(prkey0.bytes.data(), prkey0.bytes.size());
        TW::IoTeX::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx0);
    }

    { // more signatures
        TW::IoTeX::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        EXPECT_EQ(hex(PrivateKey(privateKey1).getPublicKey(TWPublicKeyTypeSECP256k1).bytes),
                  hex(pubKey1));
        signingInput.set_privatekey(prkey1.bytes.data(), prkey1.bytes.size());
        TW::IoTeX::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx1);
    }

    { // Negative: invalid public key
        const auto publicKeyBlake =
            parse_hex("6641abedacf9483b793afe1718689cc9420bbb1c");
        EXPECT_EXCEPTION(
            TransactionCompiler::compileWithSignatures(
                coin, inputStrData, {signature}, {publicKeyBlake}),
            "Invalid public key");
    }

    { // Negative: not enough signatures
        const Data outputData =
            TransactionCompiler::compileWithSignatures(coin, inputStrData, {}, {pbkey0.bytes});

        TW::IoTeX::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
        EXPECT_EQ(output.error_message(), "empty signatures or publickeys");
    }

    { // Negative: not enough publicKey
        const Data outputData =
            TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {});

        TW::IoTeX::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
        EXPECT_EQ(output.error_message(), "empty signatures or publickeys");
    }

    { // Negative: not one to on
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature}, {pbkey0.bytes, pbkey1.bytes});

        TW::IoTeX::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
        EXPECT_EQ(output.error_message(), "signatures and publickeys size can only be one");
    }
}