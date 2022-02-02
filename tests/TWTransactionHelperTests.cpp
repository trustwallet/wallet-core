// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWTransactionHelper.h>
#include <TrustWalletCore/TWData.h>
#include "proto/Binance.pb.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Ethereum.pb.h"

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "Bitcoin/Script.h"

#include "HexCoding.h"
#include "PublicKey.h"
#include "uint256.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "interface/TWTestUtilities.h"
#include <gtest/gtest.h>

#include <thread>
#include <algorithm>

using namespace TW;

TEST(TWTransactionHelper, ExternalSignatureSignBinance) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeBinance;
    const auto txInputData = WRAPD(TWTransactionHelperBuildInput(
        coin,
        STRING("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2").get(),  // from
        STRING("bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5").get(),  // to
        STRING("1").get(),  // amount
        STRING("BNB").get(),  // asset
        STRING("").get(),  // memo
        STRING("").get()  // chainId
    ));

    {
        // Check, by parsing
        EXPECT_EQ((int)TWDataSize(txInputData.get()), 88);
        Binance::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));
        EXPECT_EQ(input.chain_id(), "Binance-Chain-Nile");
        EXPECT_TRUE(input.has_send_order());
        ASSERT_EQ(input.send_order().inputs_size(), 1);
        EXPECT_EQ(hex(data(input.send_order().inputs(0).address())), "40c2979694bbc961023d1d27be6fc4d21a9febe6");
    }

    /// Step 2: Obtain preimage hash
    const auto preImageHash = WRAPD(TWTransactionHelperPreImageHash(coin, txInputData.get()));

    const auto preImageHashData = data(TWDataBytes(preImageHash.get()), TWDataSize(preImageHash.get()));
    EXPECT_EQ(hex(preImageHashData), "3f3fece9059e714d303a9a1496ddade8f2c38fa78fc4cc2e505c5dbb0ea678d1");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = parse_hex("026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const auto signature = parse_hex("1b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679");

    // Verify signature (pubkey & hash & signature)
    {
        EXPECT_TRUE(publicKey.verify(signature, preImageHashData));
    }

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionHelperCompileWithSignature(
        coin,
        txInputData.get(),
        WRAPD(TWDataCreateWithData((TWData*)&signature)).get(),
        WRAPD(TWDataCreateWithData((TWData*)&publicKeyData)).get())
    );

    const auto ExpectedTx = "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 189);
        Binance::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()), (int)TWDataSize(outputData.get())));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result
        Binance::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));
        auto key = parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832");
        input.set_private_key(key.data(), key.size());

        Binance::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

TEST(TWTransactionHelper, ExternalSignatureSignEthereum) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeEthereum;
    const auto txInputData0 = WRAPD(TWTransactionHelperBuildInput(
        coin,
        STRING("0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F").get(),  // from
        STRING("0x3535353535353535353535353535353535353535").get(),  // to
        STRING("1000000000000000000").get(),  // amount
        STRING("ETH").get(),  // asset
        STRING("").get(),  // memo
        STRING("").get()  // chainId
    ));

    // Check, by parsing
    EXPECT_EQ((int)TWDataSize(txInputData0.get()), 61);
    Ethereum::Proto::SigningInput input;
    ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData0.get()), (int)TWDataSize(txInputData0.get())));
    EXPECT_EQ(hex(input.chain_id()), "01");
    EXPECT_EQ(input.to_address(), "0x3535353535353535353535353535353535353535");
    ASSERT_TRUE(input.transaction().has_transfer());
    EXPECT_EQ(hex(input.transaction().transfer().amount()), "0de0b6b3a7640000");

    // Set a few other values
    const auto nonce = store(uint256_t(11));
    const auto gasPrice = store(uint256_t(20000000000));
    const auto gasLimit = store(uint256_t(21000));
    input.set_nonce(nonce.data(), nonce.size());
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_tx_mode(Ethereum::Proto::Legacy);

    // Serialize back, this shows how to serialize SigningInput protobuf to byte array
    const auto txInputDataData = data(input.SerializeAsString());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(txInputDataData.data(), txInputDataData.size()));
    EXPECT_EQ((int)TWDataSize(txInputData.get()), 75);

    /// Step 2: Obtain preimage hash
    const auto preImageHash = WRAPD(TWTransactionHelperPreImageHash(coin, txInputData.get()));

    const auto preImageHashData = data(TWDataBytes(preImageHash.get()), TWDataSize(preImageHash.get()));
    EXPECT_EQ(hex(preImageHashData), "15e180a6274b2f6a572b9b51823fce25ef39576d10188ecdcd7de44526c47217");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = parse_hex("044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad7119ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232a");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1Extended);
    const auto signature = parse_hex("360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c9796677900");

    // Verify signature (pubkey & hash & signature)
    {
        EXPECT_TRUE(publicKey.verify(signature, preImageHashData));
    }

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionHelperCompileWithSignature(
        coin,
        txInputData.get(),
        WRAPD(TWDataCreateWithData((TWData*)&signature)).get(),
        WRAPD(TWDataCreateWithData((TWData*)&publicKeyData)).get())
    );

    const auto ExpectedTx = "f86c0b8504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07ba053bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c97966779";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 183);
        Ethereum::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()), (int)TWDataSize(outputData.get())));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result
        Ethereum::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));
        auto key = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
        input.set_private_key(key.data(), key.size());

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

TEST(TWTransactionHelper, ExternalSignatureSignBitcoin) {
    const auto coin = TWCoinTypeBitcoin;
    const auto ownAddress = "bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv";

    // Setup input for Plan
    Bitcoin::Proto::SigningInput input;
    input.set_coin_type(coin);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(1'200'000);
    input.set_use_max_amount(false);
    input.set_byte_fee(1);
    input.set_to_address("bc1q2dsdlq3343vk29runkgv4yc292hmq53jedfjmp");
    input.set_change_address(ownAddress);

    const auto utxo0Script = Bitcoin::Script::lockScriptForAddress(ownAddress, coin);
    EXPECT_EQ(hex(utxo0Script.bytes), "0014bd92088bb7e82d611a9b94fbb74a0908152b784f");
    Data keyHash0;
    EXPECT_TRUE(utxo0Script.matchPayToWitnessPublicKeyHash(keyHash0));
    EXPECT_EQ(hex(keyHash0), "bd92088bb7e82d611a9b94fbb74a0908152b784f");

    const auto redeemScript = Bitcoin::Script::buildPayToPublicKeyHash(keyHash0);
    (*input.mutable_scripts())[hex(keyHash0)] = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());

    auto utxo0 = input.add_utxo();
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(3'899'774);
    auto hash0 = parse_hex("a85fd6a9a7f2f54cacb57e83dfd408e51c0a5fc82885e3fa06be8692962bc407");
    std::reverse(hash0.begin(), hash0.end());
    utxo0->mutable_out_point()->set_hash(std::string(hash0.begin(), hash0.end()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    // Plan is checked, assume it is accepted
    EXPECT_EQ(plan.amount(), 1'200'000);
    EXPECT_EQ(plan.fee(), 141);
    EXPECT_EQ(plan.change(), 2'699'633);

    // Extend input with accepted plan
    *input.mutable_plan() = plan;

    // Serialize input
    const auto txInputDataData = data(input.SerializeAsString());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(txInputDataData.data(), txInputDataData.size()));
    EXPECT_EQ((int)TWDataSize(txInputData.get()), 332);

    /// Step 2: Obtain preimage hash
    const auto preImageHash = WRAPD(TWTransactionHelperPreImageHash(coin, txInputData.get()));

    const auto preImageHashData = data(TWDataBytes(preImageHash.get()), TWDataSize(preImageHash.get()));
    EXPECT_EQ(hex(preImageHashData), "61a6dc2f342f09c2c34d13c90bee0dda48e0515d185a8cd1758d70d31ca0b8ec");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = parse_hex("024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const auto signature = parse_hex("30450221009f61304eb182a4862825f27f1d93406d1e8b25d2cb889ffb7d191d8c8f6de1ea0220205e43a91abb8b7e4e4f40f54cbb3c7ebfcc725c3a97c0c42c77acebf17a7d00");

    // Verify signature (pubkey & hash & signature)
    {
        EXPECT_TRUE(publicKey.verifyDer(signature, preImageHashData));
    }

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionHelperCompileWithSignature(
        coin,
        txInputData.get(),
        WRAPD(TWDataCreateWithData((TWData*)&signature)).get(),
        WRAPD(TWDataCreateWithData((TWData*)&publicKeyData)).get())
    );

    const auto ExpectedTx = "0100000000010107c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa80000000000ffffffff02804f1200000000001600145360df8231ac5965147c9d90ca930a2aafb052327131290000000000160014bd92088bb7e82d611a9b94fbb74a0908152b784f024830450221009f61304eb182a4862825f27f1d93406d1e8b25d2cb889ffb7d191d8c8f6de1ea0220205e43a91abb8b7e4e4f40f54cbb3c7ebfcc725c3a97c0c42c77acebf17a7d000121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb49338200000000";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 401);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()), (int)TWDataSize(outputData.get())));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result
        Bitcoin::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));
        auto key = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
        *input.add_private_key() = std::string(key.begin(), key.end());

        Bitcoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}
