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
#include "Bitcoin/SegwitAddress.h"

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "uint256.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "../interface/TWTestUtilities.h"
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
    const auto preImageHashes = WRAP(TWDataVector, TWTransactionHelperPreImageHashes(coin, txInputData.get()));

    ASSERT_EQ(TWDataVectorSize(preImageHashes.get()), 1);
    auto preImageHash = WRAPD(TWDataVectorGet(preImageHashes.get(), 0));
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
    const auto outputData = WRAPD(TWTransactionHelperCompileWithSignatures(
        coin,
        txInputData.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&publicKeyData)).get())
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
    const auto preImageHashes = WRAP(TWDataVector, TWTransactionHelperPreImageHashes(coin, txInputData.get()));

    ASSERT_EQ(TWDataVectorSize(preImageHashes.get()), 1);
    auto preImageHash = WRAPD(TWDataVectorGet(preImageHashes.get(), 0));
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
    const auto outputData = WRAPD(TWTransactionHelperCompileWithSignatures(
        coin,
        txInputData.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&publicKeyData)).get())
    );

    const auto ExpectedTx = "f86c0b8504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07ba053bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c97966779";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 183);
        Ethereum::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()), (int)TWDataSize(outputData.get())));

        EXPECT_EQ(output.encoded().size(), 110);
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
    struct UtxoInfo {
        Data publicKey;
        long amount;
        int index;
        Data signature;
    };

    // UtxoInfos, mapped by hash (reverse hash)
    const auto utxoHash0 = parse_hex("07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa8");
    const auto utxoHash1 = parse_hex("d6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e");
    std::map<Data, UtxoInfo> utxoInfos = {
        {utxoHash0, UtxoInfo {
            parse_hex("024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382"),
            1'000'000,
            0,
            parse_hex("3045022100b407ce439a7e09b0d64a926e4987f2ceecd9307025c16bec63e44792eef8e79e0220739af2622399bf302e1e2f2a8dd461250d94c76e17bf42154687f825ae9effee")
        }},
        {utxoHash1, UtxoInfo {
            parse_hex("0217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc"),
            800'000,
            0,
            parse_hex("3045022100b2f3f682fe4b4c17c5bc075afd758a048ae42b7ee5ce525d2a80af4d8c08e66a02204d11207c15610086159870ef27cb0d3472d08e5b43c99ad4b4e681445f9bed73")
        }},
    };

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

    // process UTXOs
    int count = 0;
    for (auto iter = utxoInfos.begin(); iter != utxoInfos.end(); ++iter, ++count) {
        const auto key = iter->first;

        Data nonReverseHash = key;
        std::reverse(nonReverseHash.begin(), nonReverseHash.end());
        if (count == 0) EXPECT_EQ(hex(nonReverseHash), "a85fd6a9a7f2f54cacb57e83dfd408e51c0a5fc82885e3fa06be8692962bc407");
        if (count == 1) EXPECT_EQ(hex(nonReverseHash), "5e90d18df4799198d915c9d7a9aa330795a8493fd2ef30e48f3b4ea55a2a89d6");

        const auto publicKey = PublicKey(iter->second.publicKey, TWPublicKeyTypeSECP256k1);
        const auto address = Bitcoin::SegwitAddress(publicKey, "bc");
        if (count == 0) EXPECT_EQ(address.string(), ownAddress);
        if (count == 1) EXPECT_EQ(address.string(), "bc1qveq6hmdvl9yrk7f6lct3s6yue9pqhwcuxedggg");

        const auto utxoScript = Bitcoin::Script::lockScriptForAddress(address.string(), coin);
        if (count == 0) EXPECT_EQ(hex(utxoScript.bytes), "0014bd92088bb7e82d611a9b94fbb74a0908152b784f");
        if (count == 1) EXPECT_EQ(hex(utxoScript.bytes), "00146641abedacf9483b793afe1718689cc9420bbb1c");

        Data keyHash;
        EXPECT_TRUE(utxoScript.matchPayToWitnessPublicKeyHash(keyHash));
        if (count == 0) EXPECT_EQ(hex(keyHash), "bd92088bb7e82d611a9b94fbb74a0908152b784f");
        if (count == 1) EXPECT_EQ(hex(keyHash), "6641abedacf9483b793afe1718689cc9420bbb1c");

        const auto redeemScript = Bitcoin::Script::buildPayToPublicKeyHash(keyHash);
        if (count == 0) EXPECT_EQ(hex(redeemScript.bytes), "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac");
        if (count == 1) EXPECT_EQ(hex(redeemScript.bytes), "76a9146641abedacf9483b793afe1718689cc9420bbb1c88ac");
        (*input.mutable_scripts())[hex(keyHash)] = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());

        auto utxo = input.add_utxo();
        utxo->set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
        utxo->set_amount(iter->second.amount);
        utxo->mutable_out_point()->set_hash(std::string(key.begin(), key.end()));
        utxo->mutable_out_point()->set_index(iter->second.index);
        utxo->mutable_out_point()->set_sequence(UINT32_MAX);
    }

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    // Plan is checked, assume it is accepted
    EXPECT_EQ(plan.amount(), 1'200'000);
    EXPECT_EQ(plan.fee(), 209);
    EXPECT_EQ(plan.change(), 599'791);
    ASSERT_EQ(plan.utxos_size(), 2);
    // Note that UTXOs happen to be in reverse order compared to the input
    EXPECT_EQ(hex(plan.utxos(0).out_point().hash()), hex(utxoHash1));
    EXPECT_EQ(hex(plan.utxos(1).out_point().hash()), hex(utxoHash0));

    // Extend input with accepted plan
    *input.mutable_plan() = plan;

    // Serialize input
    const auto txInputDataData = data(input.SerializeAsString());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(txInputDataData.data(), txInputDataData.size()));
    EXPECT_EQ((int)TWDataSize(txInputData.get()), 544);

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = WRAP(TWDataVector, TWTransactionHelperPreImageHashes(coin, txInputData.get()));

    ASSERT_EQ(TWDataVectorSize(preImageHashes.get()), 2);
    auto preImageHash0 = WRAPD(TWDataVectorGet(preImageHashes.get(), 0));
    auto preImageHash1 = WRAPD(TWDataVectorGet(preImageHashes.get(), 1));
    const std::vector<Data> preImageHashDatas = {
        data(TWDataBytes(preImageHash0.get()), TWDataSize(preImageHash0.get())),
        data(TWDataBytes(preImageHash1.get()), TWDataSize(preImageHash1.get()))
    };
    EXPECT_EQ(hex(preImageHashDatas[0]), "cf3083599f7a75e0ef2da8c96627dae8b0b0c2393988ce0b65d356ece3c1c66c");
    EXPECT_EQ(hex(preImageHashDatas[1]), "1ea2205440222a7b0270955b1122a7b3db869c7094296b6b72661e6df9e3c8fa");

    // Simulate signatures, normally obtained from signature server.
    // Note that they are in the order of the hashes, which are in the order of the UTXOs in the plan (accidentally reverse of the input)
    auto signatureVec = WRAP(TWDataVector, TWDataVectorCreate());
    auto pubkeyVec = WRAP(TWDataVector, TWDataVectorCreate());
    for (auto i = 0; i < plan.utxos_size(); ++i) {
        const auto publicKeyData = utxoInfos[data(plan.utxos(i).out_point().hash())].publicKey;
        const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
        const auto signature = utxoInfos[data(plan.utxos(i).out_point().hash())].signature;

        TWDataVectorAdd(signatureVec.get(), WRAPD(TWDataCreateWithBytes(signature.data(), signature.size())).get());
        TWDataVectorAdd(pubkeyVec.get(), WRAPD(TWDataCreateWithBytes(publicKeyData.data(), publicKeyData.size())).get());

        // Verify signature (pubkey & hash & signature)
        EXPECT_TRUE(publicKey.verifyAsDER(signature, preImageHashDatas[i]));
    }

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionHelperCompileWithSignatures(
        coin, txInputData.get(), signatureVec.get(), pubkeyVec.get()
    ));

    const auto ExpectedTx = "01000000000102d6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e0000000000ffffffff07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa80000000000ffffffff02804f1200000000001600145360df8231ac5965147c9d90ca930a2aafb05232ef26090000000000160014bd92088bb7e82d611a9b94fbb74a0908152b784f02483045022100b2f3f682fe4b4c17c5bc075afd758a048ae42b7ee5ce525d2a80af4d8c08e66a02204d11207c15610086159870ef27cb0d3472d08e5b43c99ad4b4e681445f9bed7301210217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc02483045022100b407ce439a7e09b0d64a926e4987f2ceecd9307025c16bec63e44792eef8e79e0220739af2622399bf302e1e2f2a8dd461250d94c76e17bf42154687f825ae9effee0121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb49338200000000";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 594);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()), (int)TWDataSize(outputData.get())));

        EXPECT_EQ(output.encoded().size(), 372);
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result
        Bitcoin::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));

        auto key0 = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
        auto key1 = parse_hex("7878787878787878787878787878787878787878787878787878787878787878");
        EXPECT_EQ(hex(PrivateKey(key0).getPublicKey(TWPublicKeyTypeSECP256k1).bytes), "024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382");
        EXPECT_EQ(hex(PrivateKey(key1).getPublicKey(TWPublicKeyTypeSECP256k1).bytes), "0217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc");
        *input.add_private_key() = std::string(key0.begin(), key0.end());
        *input.add_private_key() = std::string(key1.begin(), key1.end());

        Bitcoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}
