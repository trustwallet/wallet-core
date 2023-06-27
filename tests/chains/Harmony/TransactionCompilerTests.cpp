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
#include "uint256.h"

#include "proto/Harmony.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(HarmonyCompiler, CompileWithSignatures) {
    // txHash 0x238c0db5f139422d64d12b3d5208243b4b355bfb87024cec7795660291a628d0 on https://explorer.ps.hmny.io/
    /// Step 1: Prepare transaction input (protobuf)
    auto coin = TWCoinTypeHarmony;
    auto input = TW::Harmony::Proto::SigningInput();
    auto trasactionMsg = input.mutable_transaction_message();
    auto receiver = "one1y563nrrtcpu7874cry68ehxwrpteyhp0sztlym";
    trasactionMsg->set_to_address(receiver);
    auto payload = parse_hex("");
    trasactionMsg->set_payload(payload.data(), payload.size());

    uint256_t MAIN_NET = 0x4;
    auto value = store(MAIN_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t(0));
    trasactionMsg->set_nonce(value.data(), value.size());

    value = store(uint256_t(1000000000000000));
    trasactionMsg->set_gas_price(value.data(), value.size());

    value = store(uint256_t(1000000));
    trasactionMsg->set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x0"));
    trasactionMsg->set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    trasactionMsg->set_to_shard_id(value.data(), value.size());

    value = store(uint256_t(10));
    trasactionMsg->set_amount(value.data(), value.size());

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

    std::string expectedPreImage = "e98087038d7ea4c68000830f42408080942535198c6bc079e3fab819347cdcce1857925c2f0a80048080";
    std::string expectedPreImageHash = "fd1be8579542dc60f15a6218887cc1b42945bf04b50205d15ad7df8b5fac5714";
    ASSERT_EQ(hex(preImage), expectedPreImage);
    ASSERT_EQ(hex(preImageHash), expectedPreImageHash);

    const auto privateKey = PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    Data signature = parse_hex("43824f50bf4b16ebe1020114de16e3579bdb5f3dcaa26117de87a73b5414b72550506609fd60e3cb565b1f9bae0952d37f3a6c6be262380f7f18cbda5216f34300");
    const PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, preImageHash));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKey.bytes});
    const auto ExpectedTx =
        "f8698087038d7ea4c68000830f42408080942535198c6bc079e3fab819347cdcce1857925c2f0a802ba043824f50bf4b16ebe1020114de16e3579bdb5f3dcaa26117de87a73b5414b725a050506609fd60e3cb565b1f9bae0952d37f3a6c6be262380f7f18cbda5216f343";
    {
        TW::Harmony::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Harmony::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Harmony::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        Harmony::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        Harmony::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}
