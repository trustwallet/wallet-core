// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/TransactionCompiler.pb.h"
#include "proto/VeChain.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(VechainCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeVeChain;

    /// Step 1: Prepare transaction input (protobuf)
    TW::VeChain::Proto::SigningInput input;
    PrivateKey privateKey =
        PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto publicKey = privateKey.getPublicKey(publicKeyType(coin));

    input.set_chain_tag(1);
    input.set_block_ref(1);
    input.set_expiration(1);
    input.set_gas_price_coef(0);
    input.set_gas(21000);
    input.set_nonce(1);

    auto& clause = *input.add_clauses();
    auto amount = parse_hex("31303030"); // 1000
    clause.set_to("0x3535353535353535353535353535353535353535");
    clause.set_value(amount.data(), amount.size());

    auto stringInput = input.SerializeAsString();
    auto dataInput = TW::Data(stringInput.begin(), stringInput.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, dataInput);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImage = preSigningOutput.data();
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImage),
              "e7010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0");
    EXPECT_EQ(hex(preImageHash),
              "a1b8ef3af3d8c74e97ac6cd732916a8f4c38c0905c8b70d2fa598edf1f62ea04");

    /// Step 3: Sign
    TW::Data signature;
    {
        TW::VeChain::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        ASSERT_EQ(hex(output.encoded()),
                  "f86a010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0b8"
                  "41bf8edf9600e645b5abd677cb52f585e7f655d1361075d511b37f707a9f31da6702d28739933b264527"
                  "a1d05b046f5b74044b88c30c3f5a09d616bd7a4af4901601");

        signature = data(output.signature());
        /// Step 4: Verify signature
        ASSERT_TRUE(publicKey.verify(signature, data(preImageHash.data())));
    }
    {
        const Data outputData =
            TransactionCompiler::compileWithSignatures(coin, dataInput, {signature}, {publicKey.bytes});

        TW::VeChain::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        ASSERT_EQ(hex(output.encoded()),
                  "f86a010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0b8"
                  "41bf8edf9600e645b5abd677cb52f585e7f655d1361075d511b37f707a9f31da6702d28739933b264527"
                  "a1d05b046f5b74044b88c30c3f5a09d616bd7a4af4901601");
    }

    { // Negative: more than one signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, dataInput, {signature, signature}, {publicKey.bytes});
        VeChain::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }
}
