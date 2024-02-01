// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/NEO.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "NEO/Address.h"
#include "NEO/Script.h"
#include "NEO/TransactionAttribute.h"
#include "NEO/TransactionAttributeUsage.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::NEO::tests {

TEST(NEOCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeNEO;
    /// Step 1: Prepare transaction input (protobuf)
    const std::string NEO_ASSET_ID =
        "9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5";
    const std::string GAS_ASSET_ID =
        "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60";

    TW::NEO::Proto::SigningInput input;
    auto privateKey =
        PrivateKey(parse_hex("F18B2F726000E86B4950EBEA7BFF151F69635951BC4A31C44F28EE6AF7AEC128"));
    auto publicKey = privateKey.getPublicKey(publicKeyType(coin));
    input.set_gas_asset_id(GAS_ASSET_ID);
    input.set_gas_change_address("AdtSLMBqACP4jv8tRWwyweXGpyGG46eMXV");

    auto* utxo = input.add_inputs();
    auto hash = parse_hex("9c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de");
    std::reverse(hash.begin(), hash.end());
    utxo->set_prev_hash(hash.data(), hash.size());
    utxo->set_prev_index(1);
    utxo->set_asset_id(NEO_ASSET_ID);
    utxo->set_value(89300000000);

    auto txOutput = input.add_outputs();
    txOutput->set_asset_id(NEO_ASSET_ID);
    txOutput->set_to_address("Ad9A1xPbuA5YBFr1XPznDwBwQzdckAjCev");
    txOutput->set_change_address("AdtSLMBqACP4jv8tRWwyweXGpyGG46eMXV");
    txOutput->set_amount(100000000);

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "7fd5629cfc7cb0f8f01f15fc6d8b37ed1240c4f818d0b397bac65266aa6466da");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature =
        parse_hex("5046619c8e20e1fdeec92ce95f3019f6e7cc057294eb16b2d5e55c105bf32eb27e1fc01c18585762"
                  "28f1fef8c0945a8ad69688e52a4ed19f5b85f5eff7e961d7");
    // TODO uncomment when nist256p1 Rust implementation is enabled.
    // const auto signature =
    //     parse_hex("5046619c8e20e1fdeec92ce95f3019f6e7cc057294eb16b2d5e55c105bf32eb281e03fe2e7a7a89e"
    //               "d70e01073f6ba574e65071c87cc8cce59833d4d30479c37a");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    auto expectedTx =
        "800000019c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de0100029b7cffdaa674"
        "beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000ea610aa6db39bd8c8556c9"
        "569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500fcbbc4"
        "14000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac0141405046619c8e20e1fdeec92ce95f3019f6e7cc"
        "057294eb16b2d5e55c105bf32eb27e1fc01c1858576228f1fef8c0945a8ad69688e52a4ed19f5b85f5eff7e961"
        "d7232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac";
    // TODO uncomment when nist256p1 Rust implementation is enabled.
    // auto expectedTx =
    //     "800000019c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de0100029b7cffdaa674"
    //     "beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000ea610aa6db39bd8c8556c9"
    //     "569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500fcbbc4"
    //     "14000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac0141405046619c8e20e1fdeec92ce95f3019f6e7cc"
    //     "057294eb16b2d5e55c105bf32eb281e03fe2e7a7a89ed70e01073f6ba574e65071c87cc8cce59833d4d30479c3"
    //     "7a232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac";

    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    {
        NEO::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        NEO::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        NEO::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        NEO::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(NEOCompiler, Nep5TokenCompileWithSignatures) {
    // tx on mainnet
    // https://neoscan.io/transaction/6368FC6127DD7FAA3B7548CA97162D5BE18D4B2FA0046A79853E5864318E19B8

    const auto coin = TWCoinTypeNEO;
    /// Step 1: Prepare transaction input (protobuf)
    const std::string ASSET_ID =
        "f56c89be8bfcdec617e2402b5c3fd5b6d71b820d";
    const std::string GAS_ASSET_ID =
        "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60";

    TW::NEO::Proto::SigningInput input;
    auto publicKey = PublicKey(parse_hex("030ab39b99d8675cd9bd90aaec37cba964297cc817078d33e508ab11f1d245c068"), publicKeyType(coin));
    input.set_gas_asset_id(GAS_ASSET_ID);
    input.set_gas_change_address("AJzoeKrj7RHMwSrPQDPdv61ciVEYpmhkjk");

    auto* utxo = input.add_inputs();
    auto hash = parse_hex("f231ee7b5097d912a22fe7cb6b7417490d2ddde200e3c57042be5abcdf6f974c");
    utxo->set_prev_hash(hash.data(), hash.size());
    utxo->set_prev_index(0);
    utxo->set_asset_id(GAS_ASSET_ID);
    utxo->set_value(7011673);

    auto txOutput = input.add_outputs();
    txOutput->set_asset_id(GAS_ASSET_ID);
    txOutput->set_to_address("AJzoeKrj7RHMwSrPQDPdv61ciVEYpmhkjk");
    txOutput->set_change_address("AJzoeKrj7RHMwSrPQDPdv61ciVEYpmhkjk");
    txOutput->set_amount(6911673);

    // set nep5 transfer
    auto nep5Tx = input.mutable_transaction()->mutable_nep5_transfer();
    nep5Tx->set_asset_id(ASSET_ID);
    nep5Tx->set_from("AJzoeKrj7RHMwSrPQDPdv61ciVEYpmhkjk");
    nep5Tx->set_to("AeRsDBqPiGKZhzNtL2vWhXbXGccJLCGrbJ");
    auto amount = store(110000000);
    nep5Tx->set_amount(amount.data(), (int)amount.size());
    nep5Tx->set_script_with_ret(true);

    input.set_fee(100000);

    // Plan
    NEO::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    ASSERT_EQ(plan.error(), Common::Proto::OK);

    auto attr = plan.add_attributes();
    auto remark = parse_hex("f15508a6ea4e15e8");
    attr->set_usage((int32_t)NEO::TransactionAttributeUsage::TAU_Remark);
    attr->set_data(remark.data(), (int)remark.size());

    attr = plan.add_attributes();
    auto script = parse_hex("235a717ed7ed18a43de47499c3d05b8d4a4bcf3a");
    attr->set_usage((int32_t)NEO::TransactionAttributeUsage::TAU_Script);
    attr->set_data(script.data(), (int)script.size());

    *input.mutable_plan() = plan;

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "d301bc00e59a1c92741a31955714c76689f627dcb9d7ec176435f269a981159c");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature =
        parse_hex("8b707d23f84d39ddaad7da100e2d8b657ef6c0858c6c09edc029f441f28e49ff6af994ba7ad180f90e12dd9d7828f8f28785ae5079ed9a52bb5ddd3bcce1b189");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    auto expectedTx =
        "d101510480778e0614f88235a26e55cce0747ee827f39fd8167849672b14235a717ed7ed18a43de47499c3d05b8d4a4bcf3a53c1087472616e7366657267f56c89be8bfcdec617e2402b5c3fd5b6d71b820df166000000000000000002f008f15508a6ea4e15e820235a717ed7ed18a43de47499c3d05b8d4a4bcf3a014c976fdfbc5abe4270c5e300e2dd2d0d4917746bcbe72fa212d997507bee31f2000001e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60b976690000000000235a717ed7ed18a43de47499c3d05b8d4a4bcf3a0141408b707d23f84d39ddaad7da100e2d8b657ef6c0858c6c09edc029f441f28e49ff6af994ba7ad180f90e12dd9d7828f8f28785ae5079ed9a52bb5ddd3bcce1b1892321030ab39b99d8675cd9bd90aaec37cba964297cc817078d33e508ab11f1d245c068ac";
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    {
        NEO::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }
}

TEST(NEOCompiler, InvocationTransactionCompileWithSignatures) {
    // tx on mainnet
    // https://neoscan.io/transaction/73f540f5ce6fd4f363262e4b168d411f5443c694f3c53beee36fc03861c00356

    const auto coin = TWCoinTypeNEO;
    /// Step 1: Prepare transaction input (protobuf)
    auto ASSET_ID = parse_hex("f46719e2d16bf50cddcef9d4bbfece901f73cbb6");
    std::reverse(ASSET_ID.begin(), ASSET_ID.end());

    const std::string GAS_ASSET_ID =
        "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60";

    TW::NEO::Proto::SigningInput input;
    auto publicKey = PublicKey(parse_hex("02337b2d3982d71aa234a112cd8507260f480994d20129921f5a95c77f8bbe1bb3"), publicKeyType(coin));

    auto amount = store(15000000000);

    auto script = NEO::Script::CreateNep5TransferScript(
        ASSET_ID,
        NEO::Address("APqYfjvV2cCwcvFjceVcSrcouyq74qNFKS").toScriptHash(),
        NEO::Address("ANeo2toNeo3MigrationAddressxwPB2Hz").toScriptHash(),
        load(amount));

    // set invocation transaction script
    input.mutable_transaction()->mutable_invocation_generic()->set_script(script.data(), (int)script.size());

    auto plan = input.mutable_plan();
    auto attr = plan->add_attributes();
    auto attrScript = parse_hex("5872d3dd8741af4c8d5a94f8a1bfff5c617be01b");
    attr->set_usage((int32_t)NEO::TransactionAttributeUsage::TAU_Script);
    attr->set_data(attrScript.data(), (int)attrScript.size());

    attr = plan->add_attributes();
    auto remark1 = parse_hex("46726f6d204e656f4c696e652061742031363539303030373533343031");
    attr->set_usage((int32_t)NEO::TransactionAttributeUsage::TAU_Remark1);
    attr->set_data(remark1.data(), (int)remark1.size());

    attr = plan->add_attributes();
    auto remark14 = parse_hex("4e55577138626836486363746e5357346167384a4a4b453734333841637374554d54");
    attr->set_usage((int32_t)NEO::TransactionAttributeUsage::TAU_Remark14);
    attr->set_data(remark14.data(), (int)remark14.size());

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "a59a59f840dfc9426f070355bbbbe024b673095d86ba1b2810f61d5291f127f3");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature =
        parse_hex("0c22129691f4438adf0ff178acc4811dee1c625df0f65909791e2c0f563cd88f7967f0ccbb6b60609e5225fb7b2873d510fe42c43c2741d90ca002afb4861d5c");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    const auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});

    NEO::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    auto expectedTx =
        "d101500500d6117e03144b721e06b50cc74e68b417716e3b099fb99757a8145872d3dd8741af4c8d5a94f8a1bfff5c617be01b53c1087472616e7366657267b6cb731f90cefebbd4f9cedd0cf56bd1e21967f4000000000000000003205872d3dd8741af4c8d5a94f8a1bfff5c617be01bf11d46726f6d204e656f4c696e652061742031363539303030373533343031fe224e55577138626836486363746e5357346167384a4a4b453734333841637374554d5400000141400c22129691f4438adf0ff178acc4811dee1c625df0f65909791e2c0f563cd88f7967f0ccbb6b60609e5225fb7b2873d510fe42c43c2741d90ca002afb4861d5c232102337b2d3982d71aa234a112cd8507260f480994d20129921f5a95c77f8bbe1bb3ac";
    EXPECT_EQ(hex(output.encoded()), expectedTx);
}

} // namespace TW::NEO::tests
