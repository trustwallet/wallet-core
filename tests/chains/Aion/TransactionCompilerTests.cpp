// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Aion.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Aion::tests {

TEST(AionCompiler, CompileWithSignatures) {
    /// Step 1: Prepare transaction input (protobuf)
    auto privateKey = parse_hex("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9");
    auto key = PrivateKey(privateKey);
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);

    Proto::SigningInput input;
    input.set_to_address("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e");
    auto amount = store(uint256_t(10000));
    input.set_amount(amount.data(), amount.size());
    auto gasPrice = store(uint256_t(20000000000));
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    auto gasLimit = store(uint256_t(21000));
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    auto nonce = store(uint256_t(9));
    input.set_nonce(nonce.data(), nonce.size());
    input.set_timestamp(155157377101);
    input.set_private_key(privateKey.data(), privateKey.size());

    auto txInputData = data(input.SerializeAsString());
    auto preImageHashes = TransactionCompiler::preImageHashes(TWCoinTypeAion, txInputData);

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = data(preSigningOutput.data_hash());
    EXPECT_EQ(hex(preImageHash), "d4423fe7d233b85c1bf5b1120ec03842e572fb25f3755f7a20bc83addc8c4d85");

    // Simulate signature, normally obtained from signature server
    const auto signature = key.sign(preImageHash, TWCurveED25519);

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(TWCoinTypeAion, txInputData, {signature}, {publicKey.bytes});

    const auto ExpectedTx =
        "f89b09a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8227108085242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07";
    {
        Aion::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    // double check
    {
        Aion::Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeAion);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            TWCoinTypeAion, txInputData, {signature, signature}, {publicKey.bytes});
        Aion::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            TWCoinTypeAion, txInputData, {}, {});
        Aion::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

} // namespace TW::Aion::tests
