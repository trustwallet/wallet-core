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

#include "EOS/Signer.h"

#include "proto/EOS.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWStellarPassphrase.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(EOSCompiler, CompileWithSignatures) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeEOS;
    EOS::Proto::SigningInput input;
    auto chainId = parse_hex("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");
    auto refBlock = parse_hex("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144");
    auto key = parse_hex("559aead08264d5795d3909718cdd05abd49572e84fe55590eef31a88a08fdffd");

    auto& asset = *input.mutable_asset();
    asset.set_amount(300000);
    asset.set_decimals(4);
    asset.set_symbol("TKN");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_reference_block_id(refBlock.data(), refBlock.size());
    input.set_reference_block_time(1554209118);
    input.set_currency("token");
    input.set_sender("token");
    input.set_recipient("eosio");
    input.set_memo("my second transfer");
    input.set_private_key(key.data(), key.size());
    input.set_private_key_type(EOS::Proto::KeyType::MODERNK1);

    auto txInputData = data(input.SerializeAsString());
    {
        EOS::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(output.json_encoded(), R"({"compression":"none","packed_context_free_data":"","packed_trx":"6e67a35cd6679a1f3d4800000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232330000000080a920cd0000000000ea3055e09304000000000004544b4e00000000126d79207365636f6e64207472616e7366657200","signatures":["SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7"]})");
    }

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0ul);

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = data(preSigningOutput.data_hash());
    EXPECT_EQ(hex(preImageHash), "14fc3299ee3e1113096bf1869dfa14c04a7ffdedd8ebdabf530683e4cfcd726c");

    // Simulate signature, normally obtained from signature server
    const PublicKey publicKey = PrivateKey(key).getPublicKey(TWPublicKeyTypeNIST256p1);
    const auto signature = parse_hex("1f6c4efceb5a6dadab271fd7e2153d97d22690938475b23f017cf9ec29e20d25725e90e541e130daa83c38fc4c933725f05837422c3f4a51f8c1d07208c8fd5e0b"); // data("SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7");

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});

    const auto ExpectedTx = R"({"compression":"none","packed_context_free_data":"","packed_trx":"6e67a35cd6679a1f3d4800000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232330000000080a920cd0000000000ea3055e09304000000000004544b4e00000000126d79207365636f6e64207472616e7366657200","signatures":["SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7"]})";
    {
        EOS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.json_encoded(), ExpectedTx);
    }

    input.set_private_key_type(EOS::Proto::KeyType::LEGACY);
    {
        EOS::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        EXPECT_EQ(output.error(), Common::Proto::Error_internal);
        EXPECT_TRUE(output.json_encoded().empty());
    }
}
