// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestUtilities.h"
#include "TransactionCompiler.h"

#include "proto/Greenfield.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Greenfield {

TEST(GreenfieldCompiler, PreHashCompile) {
    // Successfully broadcasted https://greenfieldscan.com/tx/0x9f895cf2dd64fb1f428cefcf2a6585a813c3540fc9fe1ef42db1da2cb1df55ab

    auto privateKeyData = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");
    PrivateKey privateKey(privateKeyData);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15560);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(2);
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto& msg = *input.add_messages();
    auto& msgSend = *msg.mutable_send_coins_message();
    msgSend.set_from_address("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
    msgSend.set_to_address("0x280b27f3676db1C4475EE10F75D510Eb527fd155");
    auto amountOfTx = msgSend.add_amounts();
    amountOfTx->set_denom("BNB");
    amountOfTx->set_amount("1000000000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("2000000000000000");

    // Step 1: PreHash

    auto inputData = data(input.SerializeAsString());
    auto preOutputData = TransactionCompiler::preImageHashes(TWCoinTypeGreenfield, inputData);
    TW::TxCompiler::Proto::PreSigningOutput preOutput;
    preOutput.ParseFromArray(preOutputData.data(), static_cast<int>(preOutputData.size()));

    EXPECT_EQ(preOutput.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(hex(preOutput.data_hash()), "b8c62654582ca96b37ca94966199682bf70ed934e740d2f874ff54675a0ac344");

    // Step 2: Sign "remotely"

    auto signature = privateKey.sign(data(preOutput.data_hash()), TWCurveSECP256k1);

    EXPECT_EQ(hex(signature), "cb3a4684a991014a387a04a85b59227ebb79567c2025addcb296b4ca856e9f810d3b526f2a0d0fad6ad1b126b3b9516f8b3be020a7cca9c03ce3cf47f4199b6d00");

    // Step 3: Compile

    auto outputData = TransactionCompiler::compileWithSignatures(TWCoinTypeGreenfield, inputData, {signature}, {publicKey.bytes});
    Proto::SigningOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(hex(output.signature()), "cb3a4684a991014a387a04a85b59227ebb79567c2025addcb296b4ca856e9f810d3b526f2a0d0fad6ad1b126b3b9516f8b3be020a7cca9c03ce3cf47f4199b6d1b");
    EXPECT_EQ(output.serialized(), R"({"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CpQBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4OWQxZDk3YURGY2QzMjRCYmQ2MDNEMzg3MkJENzhlMDQwOTg1MTBiMRIqMHgyODBiMjdmMzY3NmRiMUM0NDc1RUUxMEY3NUQ1MTBFYjUyN2ZkMTU1GhcKA0JOQhIQMTAwMDAwMDAwMDAwMDAwMBJ5ClgKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAnnvNAZNoQ2wRjxwSAYWugIHA+w6RQJt73vr0ggvXW/IEgUKAwjIBRgCEh0KFwoDQk5CEhAyMDAwMDAwMDAwMDAwMDAwEMCaDBpByzpGhKmRAUo4egSoW1kifrt5VnwgJa3cspa0yoVun4ENO1JvKg0PrWrRsSazuVFvizvgIKfMqcA8489H9BmbbRs="})");
}

} // namespace TW::Greenfield
