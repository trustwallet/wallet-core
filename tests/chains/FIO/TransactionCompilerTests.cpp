// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"
#include "uint256.h"

#include "FIO/Address.h"
#include "FIO/Action.h"
#include "FIO/NewFundsRequest.h"
#include "FIO/Transaction.h"
#include "FIO/TransactionBuilder.h"
#include "FIO/Signer.h"

#include "proto/FIO.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::FIO;
using namespace std;

TEST(FIOCompiler, CompileWithSignatures) {
    Data chainId = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77");
    // 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf
    PrivateKey privateKey = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035"));
    PublicKey pubKeyA = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    Address addrA(pubKeyA);
    const auto coin = TWCoinTypeFIO;

    /// Step 1: Prepare transaction input (protobuf)    
    Proto::SigningInput input;
    input.set_expiry(1579790000);
    input.set_tpid("rewards@wallet");
    input.set_owner_public_key(addrA.string());

    input.mutable_chain_params()->set_chain_id(string(chainId.begin(), chainId.end()));
    input.mutable_chain_params()->set_head_block_number(50000);
    input.mutable_chain_params()->set_ref_block_prefix(4000123456);

    input.mutable_action()->mutable_transfer_message()->set_amount(1000000000);
    input.mutable_action()->mutable_transfer_message()->set_fee(250000000);
    input.mutable_action()->mutable_transfer_message()->set_payee_public_key("FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE");

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

    std::string expectedPreImage = "4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c6574000000000000000000000000000000000000000000000000000000000000000000";
    std::string expectedPreImageHash = "6a82a57fb9bfc43918aa757d6094ba71fa2c7ece1691c4b8551a0607273771d7";
    ASSERT_EQ(hex(preImage), expectedPreImage);
    ASSERT_EQ(hex(preImageHash), expectedPreImageHash);

    // create signature
    Data signature = Signer::signData(privateKey, preImage);
    EXPECT_EQ("1f6ccee1f4cd188cc8aefa63f8fda8c90c0493ca1504806d3a26a7300a9687bb701f188337bc9a32f01ee0c2ecf030aee197b050460d72f7272cc6ce36ef14c95b", hex(signature));
    std::string sigStr = Signer::signatureToBase58(signature);
    EXPECT_EQ("SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9", sigStr);
    
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(Signer::verify(pubKeyA, preImageHash, signature));

    const auto ExpectedTx = R"({"compression":"none","packed_context_free_data":"","packed_trx":"b0ae295e50c3400a6dee00000000010000980ad20ca85be0e1d195ba85e7cd01102b2f46fca756b200000000a8ed32325d3546494f37754d5a6f6565693548745841443234433479436b70575762663234626a597472524e6a57646d474358485a63637775694500ca9a3b0000000080b2e60e00000000102b2f46fca756b20e726577617264734077616c6c657400","signatures":["SIG_K1_K9VRCnvaTYN7vgcoVKVXgyJTdKUGV8hLXgFLoEbvqAcFxy7DXQ1rSnAfEuabi4ATkgmvnpaSBdVFN7TBtM1wrbZYqeJQw9"]})";
    /// Step 3: Compile transaction info
    {
        const Data outputData = TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {pubKeyA.bytes});

        TW::FIO::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(ExpectedTx, output.json());
        EXPECT_EQ(output.action_name(), "trnsfiopubky");
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::FIO::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
        TW::FIO::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.json(), ExpectedTx);
    }
    { // Negative: not enough signatures
        const Data outputData =
            TransactionCompiler::compileWithSignatures(coin, inputStrData, {}, {pubKeyA.bytes});

        TW::FIO::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
        EXPECT_EQ(output.error_message(), "empty signatures or publickeys");
    }

    { // Negative: not enough publicKey
        const Data outputData =
            TransactionCompiler::compileWithSignatures(coin, inputStrData, {TW::data(sigStr)}, {});

        TW::FIO::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
        EXPECT_EQ(output.error_message(), "empty signatures or publickeys");
    }

    { // Negative: not one to one
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {TW::data(sigStr)}, {pubKeyA.bytes, pubKeyA.bytes});

        TW::FIO::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
        EXPECT_EQ(output.error_message(), "signatures and publickeys size can only be one");
    }
}