// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"
#include "uint256.h"

#include "proto/Filecoin.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(FilecoinCompiler, CompileWithSignatures) {
    auto coin = TWCoinTypeFilecoin;

    /// Step 1: Prepare transaction input (protobuf)
    Filecoin::Proto::SigningInput input;
    auto privateKey = parse_hex("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe");
    auto key = PrivateKey(privateKey);
    auto publicKey = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    auto toAddress =
        "f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq";
    uint64_t nonce = 2;
    // 600 FIL
    // auto value = parse_hex("2086ac351052600000");
    auto value = store(uint256_t(600) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));
    uint64_t gasLimit = 1000;
    // auto gasFeeCap = parse_hex("25f273933db5700000");
    auto gasFeeCap = store(uint256_t(700) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));
    // auto gasPremium = parse_hex("2b5e3af16b18800000");
    auto gasPremium = store(uint256_t(800) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));

    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_to(toAddress);
    input.set_nonce(nonce);
    input.set_value(value.data(), value.size());
    input.set_gas_limit(gasLimit);
    input.set_gas_fee_cap(gasFeeCap.data(), gasFeeCap.size());
    input.set_gas_premium(gasPremium.data(), gasPremium.size());
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto txInputData = data(input.SerializeAsString());
    auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = data(preSigningOutput.data_hash());
    EXPECT_EQ(hex(preImageHash), "8368c0f622b2c529c7fa147d75aa02aaa7fc13fc4847d4dc57e7a5c59048aafe");

    // Simulate signature, normally obtained from signature server
    const auto signature = key.sign(preImageHash, TWCurveSECP256k1);

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});

    const auto ExpectedTx = R"({"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"700000000000000000000","GasLimit":1000,"GasPremium":"800000000000000000000","Method":0,"Nonce":2,"To":"f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq","Value":"600000000000000000000"},"Signature":{"Data":"jMRu+OZ/lfppgmqSfGsntFrRLWZnUg3ZYmJTTRLsVt4V1310vR3VKGJpaE6S4sNvDOE6sEgmN9YmfTkPVK2qMgE=","Type":1}})";
    {
        Filecoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        ASSERT_EQ(output.json(), ExpectedTx);
    }

    // double check
    {
        Filecoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(output.json(), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Filecoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Filecoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}
