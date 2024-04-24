// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Ontology.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Ontology::tests {

TEST(OntologyCompiler, CompileWithSignatures) {
    /// Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeOntology;
    auto input = Ontology::Proto::SigningInput();
    input.set_method("transfer");
    input.set_owner_address("AaCTzuhEr6essEEKnSTuxD2GJkmGc4nuJp");
    input.set_to_address("AWBzyqpXcSpgrWyzR6qzUGWc9ZoYT3Bsvk");
    input.set_payer_address("AaCTzuhEr6essEEKnSTuxD2GJkmGc4nuJp");
    input.set_amount(1);
    input.set_gas_price(3500);
    input.set_gas_limit(20000);
    input.set_nonce(1);

    /// Obtain preimage hash and check it
    input.set_contract("ONT");
    auto ontTxInputData = data(input.SerializeAsString());
    const auto ontPreImageHashes = TransactionCompiler::preImageHashes(coin, ontTxInputData);
    auto ontPreOutput = TxCompiler::Proto::PreSigningOutput();
    ontPreOutput.ParseFromArray(ontPreImageHashes.data(), (int)ontPreImageHashes.size());
    auto ontPreImageHash = ontPreOutput.data_hash();

    input.set_contract("ONG");
    auto ongTxInputData = data(input.SerializeAsString());
    const auto ongPreImageHashes = TransactionCompiler::preImageHashes(coin, ongTxInputData);
    auto ongPreOutput = TxCompiler::Proto::PreSigningOutput();
    ongPreOutput.ParseFromArray(ongPreImageHashes.data(), (int)ongPreImageHashes.size());
    auto ongPreImageHash = ongPreOutput.data_hash();

    {
        EXPECT_EQ(hex(ontPreImageHash),
                  "d3770eb50f1fcddc17ac9d59f1b7e69c4916dbbe4c484cc6ba27dd0792aeb943");
        EXPECT_EQ(hex(ongPreImageHash),
                  "788066583071cfd05a4a10e5b897b9b81d2363c16fd98128ddc81891535567af");
    }

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData =
        parse_hex("038ea73c590f48c7d5a8ba544a928a0c8fb206aab60688793a054db9823602765a");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeNIST256p1);
    const auto ontSignature =
        parse_hex("b1678dfcda9b9b468d9a97a5b3021a680814180ca08cd17d9e3a9cf512b05a3b286fed9b8f635718"
                  "c0aabddc9fc1acfbc48561577e35ef92ee97d7fa86e14f47");
    const auto ongSignature =
        parse_hex("d90c4d76e9d07d3e5c00e4a8768ce09ca66be05cfb7f48ad02632b4f08fcaa6f4e3f6f52eb4278c1"
                  "579065e54ea5e696b7711f071298576fa7050b21ae614bbe");

    // Verify signature (pubkey & hash & signature)
    {
        EXPECT_TRUE(publicKey.verify(ontSignature, TW::data(ontPreImageHash)));
        EXPECT_TRUE(publicKey.verify(ongSignature, TW::data(ongPreImageHash)));
    }

    /// Compile transaction info
    const Data ontOutputData = TransactionCompiler::compileWithSignatures(
        coin, ontTxInputData, {ontSignature}, {publicKeyData});
    const Data ongOutputData = TransactionCompiler::compileWithSignatures(
        coin, ongTxInputData, {ongSignature}, {publicKeyData});
    auto ontOutput = Ontology::Proto::SigningOutput();
    auto ongOutput = Ontology::Proto::SigningOutput();
    ontOutput.ParseFromArray(ontOutputData.data(), (int)ontOutputData.size());
    ongOutput.ParseFromArray(ongOutputData.data(), (int)ongOutputData.size());
    const auto ontExpectedTx =
        "00d101000000ac0d000000000000204e000000000000ca18ec37ac94f19588926a5302ded54cd909a76e7100c6"
        "6b14ca18ec37ac94f19588926a5302ded54cd909a76e6a7cc8149e21dda3257e18eb033d9451dda1d9ac8bcfa4"
        "776a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f"
        "6e746f6c6f67792e4e61746976652e496e766f6b6500014140b1678dfcda9b9b468d9a97a5b3021a680814180c"
        "a08cd17d9e3a9cf512b05a3b286fed9b8f635718c0aabddc9fc1acfbc48561577e35ef92ee97d7fa86e14f4723"
        "21038ea73c590f48c7d5a8ba544a928a0c8fb206aab60688793a054db9823602765aac";
    const auto ongExpectedTx =
        "00d101000000ac0d000000000000204e000000000000ca18ec37ac94f19588926a5302ded54cd909a76e7100c6"
        "6b14ca18ec37ac94f19588926a5302ded54cd909a76e6a7cc8149e21dda3257e18eb033d9451dda1d9ac8bcfa4"
        "776a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000020068164f"
        "6e746f6c6f67792e4e61746976652e496e766f6b6500014140d90c4d76e9d07d3e5c00e4a8768ce09ca66be05c"
        "fb7f48ad02632b4f08fcaa6f4e3f6f52eb4278c1579065e54ea5e696b7711f071298576fa7050b21ae614bbe23"
        "21038ea73c590f48c7d5a8ba544a928a0c8fb206aab60688793a054db9823602765aac";

    {
        EXPECT_EQ(hex(ontOutput.encoded()), ontExpectedTx);
        EXPECT_EQ(hex(ongOutput.encoded()), ongExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, ontTxInputData, {ongSignature, ongSignature}, {publicKey.bytes});
        Ontology::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
    }

    { // Negative: empty signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, ontTxInputData, {}, {});
        Ontology::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }

    // OEP4Token transfer test case
    input.set_method("transfer");
    input.set_contract("2f34b28eb98a1dd901d303f5294c87546fb37fe7");
    input.set_owner_address("Aa8QcHJ8tbRXyjpG6FHo7TysjKXxkd1Yf2");
    input.set_to_address("ARR6PsaBwRttzCmyxCMhL7NmFk1LqExD7L");
    input.set_payer_address("Aa8QcHJ8tbRXyjpG6FHo7TysjKXxkd1Yf2");
    input.set_amount(1000);
    input.set_gas_price(2500);
    input.set_gas_limit(20000);
    input.set_nonce(1);

    auto oepTxInputData = data(input.SerializeAsString());
    const auto oepPreImageHashes = TransactionCompiler::preImageHashes(coin, oepTxInputData);
    auto oepPreOutput = TxCompiler::Proto::PreSigningOutput();
    oepPreOutput.ParseFromArray(oepPreImageHashes.data(), (int)oepPreImageHashes.size());
    auto oepPreImageHash = oepPreOutput.data_hash();
    EXPECT_EQ(hex(oepPreImageHash),
              "5be4a3be92a49ce2af800c94c7c44eeb8cd345c25541f63e545edc06bd72c0ed");

    const auto oepPublicKeyData =
        parse_hex("03932a08085b4bd7adcf8915f805ab35ad51f58ebbd09783b01bb4c44e503444f9");
    const PublicKey opePublicKey = PublicKey(oepPublicKeyData, TWPublicKeyTypeNIST256p1);
    const auto oepSignature =
        parse_hex("55aff2726c5e17dd6a6bbdaf5200442f4c9890a0cc044fb13d4a09918893dce261bb14eec2f578b590ed5c925f66bcfeddf794bee6a014c65e049f544953cb09");
    EXPECT_TRUE(opePublicKey.verify(oepSignature, TW::data(oepPreImageHash)));

    const Data oepOutputData = TransactionCompiler::compileWithSignatures(
        coin, oepTxInputData, {oepSignature}, {oepPublicKeyData});
    auto oepOutput = Ontology::Proto::SigningOutput();
    oepOutput.ParseFromArray(oepOutputData.data(), (int)oepOutputData.size());
    const auto oepExpectedTx =
        "00d101000000c409000000000000204e000000000000c9546dcef4038ce3b64e79d079b3c97a8931c7174d02e8"
        "031469c329fbb30a490979ea1a6f0b6a3a91235f6bd714c9546dcef4038ce3b64e79d079b3c97a8931c71753c1"
        "087472616e7366657267e77fb36f54874c29f503d301d91d8ab98eb2342f0001414055aff2726c5e17dd6a6bbd"
        "af5200442f4c9890a0cc044fb13d4a09918893dce261bb14eec2f578b590ed5c925f66bcfeddf794bee6a014c6"
        "5e049f544953cb09232103932a08085b4bd7adcf8915f805ab35ad51f58ebbd09783b01bb4c44e503444f9ac";
    EXPECT_EQ(hex(oepOutput.encoded()), oepExpectedTx);
}

} // namespace TW::Ontology::tests
