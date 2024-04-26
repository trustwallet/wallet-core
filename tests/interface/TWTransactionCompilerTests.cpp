// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "proto/Binance.pb.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/NULS.pb.h"
#include "proto/Ripple.pb.h"
#include "proto/Solana.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWTransactionCompiler.h>

#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"
#include "NULS/Address.h"
#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "uint256.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <map>
#include <thread>

using namespace TW;

TEST(TWTransactionCompiler, ExternalSignatureSignBinance) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeBinance;
    // bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2
    const auto fromAddressData = parse_hex("40c2979694bbc961023d1d27be6fc4d21a9febe6");
    // bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5
    const auto toAddressData = parse_hex("bffe47abfaede50419c577f1074fee6dd1535cd1");

    Binance::Proto::SigningInput txInput;

    txInput.set_chain_id("Binance-Chain-Nile");
    auto& sendOrder = *txInput.mutable_send_order();

    auto& input1 = *sendOrder.add_inputs();
    input1.set_address(fromAddressData.data(), fromAddressData.size());
    auto& input1Coin = *input1.add_coins();
    input1Coin.set_amount(1);
    input1Coin.set_denom("BNB");

    auto& output1 = *sendOrder.add_outputs();
    output1.set_address(toAddressData.data(), toAddressData.size());
    auto& output1Coin = *output1.add_coins();
    output1Coin.set_amount(1);
    output1Coin.set_denom("BNB");

    auto txInputData = data(txInput.SerializeAsString());

    {
        // Check, by parsing
        EXPECT_EQ((int)txInputData.size(), 88);
        Binance::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(txInputData.data(), (int)txInputData.size()));
        EXPECT_EQ(input.chain_id(), "Binance-Chain-Nile");
        EXPECT_TRUE(input.has_send_order());
        ASSERT_EQ(input.send_order().inputs_size(), 1);
        EXPECT_EQ(hex(data(input.send_order().inputs(0).address())),
                  "40c2979694bbc961023d1d27be6fc4d21a9febe6");
    }

    /// Step 2: Obtain preimage hash
    auto txInputDataPtr = WRAPD(TWDataCreateWithBytes(txInputData.data(), txInputData.size()));
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputDataPtr.get()));
    auto preImageHash = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHash.data(), int(preImageHash.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    const auto preImageHashData = data(preSigningOutput.data_hash());
    
    EXPECT_EQ(hex(preImageHashData), "3f3fece9059e714d303a9a1496ddade8f2c38fa78fc4cc2e505c5dbb0ea678d1");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData =
        parse_hex("026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const auto signature =
        parse_hex("1b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5"
                  "366bb8b041781a6771248550d04094c3d7a504f9e8310679");

    // Verify signature (pubkey & hash & signature)
    { EXPECT_TRUE(publicKey.verify(signature, preImageHashData)); }

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputDataPtr.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&publicKeyData)).get()));

    const auto ExpectedTx =
        "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001"
        "121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35"
        "920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c"
        "253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a5"
        "04f9e8310679";
    {
        Binance::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()),
                                          (int)TWDataSize(outputData.get())));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Binance::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputDataPtr.get()),
                                         (int)TWDataSize(txInputDataPtr.get())));
        auto key = parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832");
        input.set_private_key(key.data(), key.size());

        Binance::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

TEST(TWTransactionCompiler, ExternalSignatureSignEthereum) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeEthereum;
    Ethereum::Proto::SigningInput signingInput;

    const auto nonce = store(uint256_t(11));
    const auto chainId = store(uint256_t(1));
    const auto gasPrice = store(uint256_t(20000000000));
    const auto gasLimit = store(uint256_t(21000));
    const auto amount = store(uint256_t(1'000'000'000'000'000'000));

    signingInput.set_nonce(nonce.data(), nonce.size());
    signingInput.set_chain_id(chainId.data(), chainId.size());
    signingInput.set_gas_price(gasPrice.data(), gasPrice.size());
    signingInput.set_gas_limit(gasLimit.data(), gasLimit.size());
    signingInput.set_tx_mode(Ethereum::Proto::Legacy);
    signingInput.set_to_address("0x3535353535353535353535353535353535353535");

    signingInput.mutable_transaction()->mutable_transfer()->set_amount(amount.data(), amount.size());

    // Serialize back, this shows how to serialize SigningInput protobuf to byte array
    const auto txInputDataData = data(signingInput.SerializeAsString());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(txInputDataData.data(), txInputDataData.size()));
    EXPECT_EQ((int)TWDataSize(txInputData.get()), 75);

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputData.get()));
    auto preImageHash = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHash.data(), int(preImageHash.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    const auto preImageHashData = data(preSigningOutput.data_hash());
    EXPECT_EQ(hex(preImageHashData), "15e180a6274b2f6a572b9b51823fce25ef39576d10188ecdcd7de44526c47217");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData =
        parse_hex("044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad711"
                  "9ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232a");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1Extended);
    const auto signature =
        parse_hex("360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a19"
                  "1d844db458893b928f3efbfee90c9febf51ab84c9796677900");

    // Verify signature (pubkey & hash & signature)
    { EXPECT_TRUE(publicKey.verify(signature, preImageHashData)); }

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputData.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&publicKeyData)).get()));

    const auto ExpectedTx =
        "f86c0b8504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0"
        "360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07ba053bd9d1376e15a191d844db4"
        "58893b928f3efbfee90c9febf51ab84c97966779";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 217ul);
        Ethereum::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()),
                                          (int)TWDataSize(outputData.get())));

        EXPECT_EQ(output.encoded().size(), 110ul);
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Ethereum::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()),
                                         (int)TWDataSize(txInputData.get())));
        auto key = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
        input.set_private_key(key.data(), key.size());

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

// data conversion utility
Data dataFromTWData(std::shared_ptr<TWData> data) {
    return TW::data(TWDataBytes(data.get()), TWDataSize(data.get()));
}

TEST(TWTransactionCompiler, ExternalSignatureSignBitcoin) {
    // Test external signining with a Bitcoin transaction with 3 input UTXOs, all used, but only using 2 public keys.
    // Three signatures are neeeded.  This illustrates that order of UTXOs/hashes is not always the same.

    const auto revUtxoHash0 =
        parse_hex("07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa8");
    const auto revUtxoHash1 =
        parse_hex("d6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e");
    const auto revUtxoHash2 =
        parse_hex("6021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d");
    const auto inPubKey0 =
        parse_hex("024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382");
    const auto inPubKey1 =
        parse_hex("0217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc");
    const auto inPubKeyHash0 = parse_hex("bd92088bb7e82d611a9b94fbb74a0908152b784f");
    const auto inPubKeyHash1 = parse_hex("6641abedacf9483b793afe1718689cc9420bbb1c");

    // Input UTXO infos
    struct UtxoInfo {
        Data revUtxoHash;
        Data publicKey;
        long amount;
        int index;
    };
    std::vector<UtxoInfo> utxoInfos = {
        // first
        UtxoInfo{revUtxoHash0, inPubKey0, 600'000, 0},
        // second UTXO, with same pubkey
        UtxoInfo{revUtxoHash1, inPubKey0, 500'000, 1},
        // third UTXO, with different pubkey
        UtxoInfo{revUtxoHash2, inPubKey1, 400'000, 0},
    };

    // Signature infos, indexed by pubkeyhash+hash
    struct SignatureInfo {
        Data signature;
        Data publicKey;
    };
    std::map<std::string, SignatureInfo> signatureInfos = {
        {hex(inPubKeyHash0) + "+" +
             "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7",
         {
             parse_hex("304402201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b349022"
                       "00a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a40"),
             inPubKey0,
         }},
        {hex(inPubKeyHash1) + "+" +
             "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6",
         {
             parse_hex("3044022041294880caa09bb1b653775310fcdd1458da6b8e7d7fae34e37966414fe11582022"
                       "0646397c9d2513edc5974ecc336e9b287de0cdf071c366f3b3dc3ff309213e4e4"),
             inPubKey1,
         }},
        {hex(inPubKeyHash0) + "+" +
             "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101",
         {
             parse_hex("30440220764e3d5b3971c4b3e70b23fb700a7462a6fe519d9830e863a1f8388c402ad0b1022"
                       "07e777f7972c636961f92375a2774af3b7a2a04190251bbcb31d19c70927952dc"),
             inPubKey0,
         }},
    };

    const auto coin = TWCoinTypeBitcoin;
    const auto ownAddress = "bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv";

    // Setup input for Plan
    Bitcoin::Proto::SigningInput signingInput;
    signingInput.set_coin_type(coin);
    signingInput.set_hash_type(TWBitcoinSigHashTypeAll);
    signingInput.set_amount(1'200'000);
    signingInput.set_use_max_amount(false);
    signingInput.set_byte_fee(1);
    signingInput.set_to_address("bc1q2dsdlq3343vk29runkgv4yc292hmq53jedfjmp");
    signingInput.set_change_address(ownAddress);

    // process UTXOs
    int count = 0;
    for (auto& u : utxoInfos) {
        const auto publicKey = PublicKey(u.publicKey, TWPublicKeyTypeSECP256k1);
        const auto address = Bitcoin::SegwitAddress(publicKey, "bc");
        if (count == 0)
            EXPECT_EQ(address.string(), ownAddress);
        if (count == 1)
            EXPECT_EQ(address.string(), ownAddress);
        if (count == 2)
            EXPECT_EQ(address.string(), "bc1qveq6hmdvl9yrk7f6lct3s6yue9pqhwcuxedggg");

        const auto utxoScript = Bitcoin::Script::lockScriptForAddress(address.string(), coin);
        if (count == 0)
            EXPECT_EQ(hex(utxoScript.bytes), "0014bd92088bb7e82d611a9b94fbb74a0908152b784f");
        if (count == 1)
            EXPECT_EQ(hex(utxoScript.bytes), "0014bd92088bb7e82d611a9b94fbb74a0908152b784f");
        if (count == 2)
            EXPECT_EQ(hex(utxoScript.bytes), "00146641abedacf9483b793afe1718689cc9420bbb1c");

        Data keyHash;
        EXPECT_TRUE(utxoScript.matchPayToWitnessPublicKeyHash(keyHash));
        if (count == 0)
            EXPECT_EQ(hex(keyHash), hex(inPubKeyHash0));
        if (count == 1)
            EXPECT_EQ(hex(keyHash), hex(inPubKeyHash0));
        if (count == 2)
            EXPECT_EQ(hex(keyHash), hex(inPubKeyHash1));

        const auto redeemScript = Bitcoin::Script::buildPayToPublicKeyHash(keyHash);
        if (count == 0) EXPECT_EQ(hex(redeemScript.bytes), "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac");
        if (count == 1) EXPECT_EQ(hex(redeemScript.bytes), "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac");
        if (count == 2) EXPECT_EQ(hex(redeemScript.bytes), "76a9146641abedacf9483b793afe1718689cc9420bbb1c88ac");
        (*signingInput.mutable_scripts())[hex(keyHash)] = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());

        auto utxo = signingInput.add_utxo();
        utxo->set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
        utxo->set_amount(u.amount);
        utxo->mutable_out_point()->set_hash(
            std::string(u.revUtxoHash.begin(), u.revUtxoHash.end()));
        utxo->mutable_out_point()->set_index(u.index);
        utxo->mutable_out_point()->set_sequence(UINT32_MAX);

        ++count;
    }
    EXPECT_EQ(count, 3);
    EXPECT_EQ(signingInput.utxo_size(), 3);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(signingInput, plan, coin);

    // Plan is checked, assume it is accepted
    EXPECT_EQ(plan.amount(), 1'200'000);
    EXPECT_EQ(plan.fee(), 277);
    EXPECT_EQ(plan.change(), 299'723);
    ASSERT_EQ(plan.utxos_size(), 3);
    // Note that UTXOs happen to be in reverse order compared to the input
    EXPECT_EQ(hex(plan.utxos(0).out_point().hash()), hex(revUtxoHash2));
    EXPECT_EQ(hex(plan.utxos(1).out_point().hash()), hex(revUtxoHash1));
    EXPECT_EQ(hex(plan.utxos(2).out_point().hash()), hex(revUtxoHash0));

    // Extend input with accepted plan
    *signingInput.mutable_plan() = plan;

    // Serialize signingInput
    const auto txInputDataData = data(signingInput.SerializeAsString());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(txInputDataData.data(), txInputDataData.size()));

    /// Step 2: Obtain preimage hashes
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputData.get()));
    auto preImageHash = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));
    Bitcoin::Proto::PreSigningOutput preOutput;
    ASSERT_TRUE(preOutput.ParseFromArray(preImageHash.data(), (int)preImageHash.size()));
    ASSERT_EQ(preOutput.hash_public_keys_size(), 3);
    auto hashes = preOutput.hash_public_keys();
    EXPECT_EQ(hex(hashes[0].data_hash()), "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6");
    EXPECT_EQ(hex(hashes[1].data_hash()), "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7");
    EXPECT_EQ(hex(hashes[2].data_hash()), "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101");
    EXPECT_EQ(hex(hashes[0].public_key_hash()), hex(inPubKeyHash1));
    EXPECT_EQ(hex(hashes[1].public_key_hash()), hex(inPubKeyHash0));
    EXPECT_EQ(hex(hashes[2].public_key_hash()), hex(inPubKeyHash0));

    // Simulate signatures, normally obtained from signature server.
    auto signatureVec = WRAP(TWDataVector, TWDataVectorCreate());
    auto pubkeyVec = WRAP(TWDataVector, TWDataVectorCreate());
    for (const auto& h: hashes) {
        const auto& preImageHash_ = TW::data(h.data_hash());
        const auto& pubkeyhash = h.public_key_hash();

        const std::string key = hex(pubkeyhash) + "+" + hex(preImageHash_);
        const auto sigInfoFind = signatureInfos.find(key);
        ASSERT_TRUE(sigInfoFind != signatureInfos.end());
        const auto& sigInfo = std::get<1>(*sigInfoFind);
        const auto& publicKeyData = sigInfo.publicKey;
        const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
        const auto signature = sigInfo.signature;

        TWDataVectorAdd(signatureVec.get(), WRAPD(TWDataCreateWithBytes(signature.data(), signature.size())).get());
        TWDataVectorAdd(pubkeyVec.get(), WRAPD(TWDataCreateWithBytes(publicKeyData.data(), publicKeyData.size())).get());
        // Verify signature (pubkey & hash & signature)
        EXPECT_TRUE(publicKey.verifyAsDER(signature, preImageHash_));
    }
    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputData.get(), signatureVec.get(), pubkeyVec.get()));

    const auto ExpectedTx =
        "010000000001036021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d0000000000ff"
        "ffffffd6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e0100000000ffffffff07"
        "c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa80000000000ffffffff02804f1200"
        "000000001600145360df8231ac5965147c9d90ca930a2aafb05232cb92040000000000160014bd92088bb7e82d"
        "611a9b94fbb74a0908152b784f02473044022041294880caa09bb1b653775310fcdd1458da6b8e7d7fae34e379"
        "66414fe115820220646397c9d2513edc5974ecc336e9b287de0cdf071c366f3b3dc3ff309213e4e40121021714"
        "2f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc0247304402201857bc6e6e48b46046"
        "a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efd"
        "bc9c6784aa2571ff4f6f2a400121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb49"
        "3382024730440220764e3d5b3971c4b3e70b23fb700a7462a6fe519d9830e863a1f8388c402ad0b102207e777f"
        "7972c636961f92375a2774af3b7a2a04190251bbcb31d19c70927952dc0121024bc2a31265153f07e70e0bab08"
        "724e6b85e217f8cd628ceb62974247bb49338200000000";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 786ul);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()),
                                          (int)TWDataSize(outputData.get())));

        EXPECT_EQ(output.encoded().size(), 518ul);
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private keys were not used anywhere up to this point.
        Bitcoin::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));

        // 2 private keys are needed (despite >2 UTXOs)
        auto key0 = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
        auto key1 = parse_hex("7878787878787878787878787878787878787878787878787878787878787878");
        EXPECT_EQ(hex(PrivateKey(key0).getPublicKey(TWPublicKeyTypeSECP256k1).bytes),
                  hex(inPubKey0));
        EXPECT_EQ(hex(PrivateKey(key1).getPublicKey(TWPublicKeyTypeSECP256k1).bytes),
                  hex(inPubKey1));
        *input.add_private_key() = std::string(key0.begin(), key0.end());
        *input.add_private_key() = std::string(key1.begin(), key1.end());

        Bitcoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

TEST(TWTransactionCompiler, ExternalSignatureSignSolana) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_transfer_transaction();
    auto recipient = std::string("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    auto sender = std::string("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("TPJFTN4CjBn12HiBfAbGUhpD9zGvRSm2RcheFRA4Fyv"));
    message.set_recipient(recipient);
    message.set_value((uint64_t)1000);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(inputStrData.data(), inputStrData.size()));

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputData.get()));
    auto preImageHashData = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    ASSERT_EQ(preSigningOutput.signers_size(), 1);
    auto signer = preSigningOutput.signers(0);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(hex(preImageHash),
              "010001030d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c024c255a8bc3e"
              "8496217a2cd2a1894b9b9dcace04fcd9c0d599acdaaea40a1b6100000000000000000000000000000000"
              "0000000000000000000000000000000006c25012cc11a599a45b3b2f7f8a7c65b0547fa0bb67170d7a0c"
              "d1eda4e2c9e501020200010c02000000e803000000000000");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData =
        parse_hex("0d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature =
        parse_hex("a8c610697087eaf8a34b3facbe06f8e9bb9603bb03270dad021ffcd2fc37b6e9efcdcb78b227401f"
                  "000eb9231c67685240890962e44a17fd27fc2ff7b971df03");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputData.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&publicKeyData)).get()));

    const auto ExpectedTx =
        "5bWxVCP5fuzkKSGby9hnsLranszQJR2evJGTfBrpDQ4rJceW1WxKNrWqVPBsN2QCAGmE6W7VaYkyWjv39HhGrr1Ne2"
        "QSUuHZdyyn7hK4pxzLPMgPG8fY1XvXdppWMaKMLmhriLkckzGKJMaE3pWBRFBKzigXY28714uUNndb7S9hVakxa59h"
        "rLph39CMgAkcj6b8KYvJEkb1YdYytHSZNGi4kVVTNqiicNgPdf1gmG6qz9zVtnqj9JtaD2efdS8qxsKnvNWSgb8Xxb"
        "T6dwyp7msUUi7d27cYaPTpK";
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()),
                                          (int)TWDataSize(outputData.get())));
        EXPECT_EQ(output.encoded(), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(TWDataBytes(txInputData.get()),
                                         (int)TWDataSize(txInputData.get())));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TWTransactionCompiler, ExternalSignatureSignNULS) {
    const auto coin = TWCoinTypeNULS;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::NULS::Proto::SigningInput();
    auto from = std::string("NULSd6HgWabfcG6H7NDK2TJvtoU3wxY1YLKwJ");
    auto to = std::string("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe");
    uint32_t chainId = 1;
    uint32_t idassetsId = 1;
    auto amount = TW::store((TW::uint256_t)10000000);
    auto amountStr = std::string(amount.begin(), amount.end());
    auto balance = TW::store((TW::uint256_t)100000000);
    auto balanceStr = std::string(balance.begin(), balance.end());
    auto nonce = std::string("0000000000000000");
    input.set_from(from);
    input.set_to(to);
    input.set_amount(amountStr);
    input.set_chain_id(chainId);
    input.set_idassets_id(idassetsId);
    input.set_nonce(nonce);
    input.set_balance(balanceStr);
    input.set_timestamp((uint32_t)1569228280);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(inputStrData.data(), inputStrData.size()));

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputData.get()));
    auto preImageHashData = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImage = preSigningOutput.data();
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImage),
              "0200f885885d00008c01170100012c177a01a7afbe98e094007b99476534fb7926b701000100201d9a00"
              "000000000000000000000000000000000000000000000000000000000800000000000000000001170100"
              "01f05e7878971f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000"
              "0000000000000000000000000000000000000000000000");
    EXPECT_EQ(hex(preImageHash),
              "8746b37cb4b443424d3093e8107c5dfd6c5318010bbffcc8e8ba7c1da60877fd");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData =
        parse_hex("033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0aff0ee045");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const auto signature =
        parse_hex("a5234269eab6fe8a1510dd0cb36070a03464b48856e1ef2681dbb79a5ec656f92961ac01d401a6f8"
                  "49acc958c6c9653f49282f5a0916df036ea8766918bac19500");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputData.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&publicKeyData)).get()));

    const auto ExpectedEncoded = parse_hex(
        "0200f885885d00008c01170100012c177a01a7afbe98e094007b99476534fb7926b701000100201d9a00000000"
        "00000000000000000000000000000000000000000000000000080000000000000000000117010001f05e787897"
        "1f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000000000000000000000"
        "00000000000000000000000000006a21033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0a"
        "ff0ee045473045022100a5234269eab6fe8a1510dd0cb36070a03464b48856e1ef2681dbb79a5ec656f9022029"
        "61ac01d401a6f849acc958c6c9653f49282f5a0916df036ea8766918bac195");
    const auto ExpectedTx = std::string(ExpectedEncoded.begin(), ExpectedEncoded.end());
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 259ul);
        NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()),
                                          (int)TWDataSize(outputData.get())));
        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 256ul);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        NULS::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(TWDataBytes(txInputData.get()),
                                         (int)TWDataSize(txInputData.get())));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        NULS::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

// TEST(TWTransactionCompiler, ExternalSignatureSignRipple) {
//     const auto coin = TWCoinTypeXRP;
//     /// Step 1: Prepare transaction input (protobuf)
//     auto key = parse_hex("ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764");
//     auto input = TW::Ripple::Proto::SigningInput();
//     auto privateKey = TW::PrivateKey(key);
//     auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
//     input.set_amount(29000000);
//     input.set_fee(200000);
//     input.set_sequence(1);
//     input.set_account("rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF");
//     input.set_destination("rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF");
//     input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());
//     auto inputString = input.SerializeAsString();
//     auto inputStrData = TW::Data(inputString.begin(), inputString.end());
//     const auto txInputData = WRAPD(TWDataCreateWithBytes(inputStrData.data(), inputStrData.size()));
    
//     /// Step 2: Obtain preimage hash
//     const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputData.get()));
//     auto preImageHashData = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

//     auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
//     ASSERT_TRUE(
//         preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
//     ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
//     // preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size());
//     auto preImage = preSigningOutput.data();
//     EXPECT_EQ(hex(preImage),
//               "5354580012000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92"
//               "cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b681148400b6b6d08d5d495653d73e"
//               "da6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d");
//     auto preImageHash = preSigningOutput.data_hash();
//     EXPECT_EQ(hex(preImageHash),
//               "8624dbbd5da9ccc8f7a50faf8af8709837db72f51a50cac15a6cd28ce6107b3d");
//     // Simulate signature, normally obtained from signature server
//     const auto signature = privateKey.sign(parse_hex("8624dbbd5da9ccc8f7a50faf8af8709837db72f51a50cac15a6cd28ce6107b3d"), TWCurveSECP256k1);
//     // Verify signature (pubkey & hash & signature)
//     EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
//     /// Step 3: Compile transaction info
//     const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
//         coin, txInputData.get(),
//         WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
//         WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&publicKey.bytes)).get()));

//     const auto ExpectedTx = std::string(
//         "12000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92cefb3a45"
//         "37b3edb0b6044c04af27c01583c577823ecc69a9a21119b6744630440220067f20b3eebfc7107dd0bcc7"
//         "2337a236ac3be042c0469f2341d76694a17d4bb9022048393d7ee7dcb729783b33f5038939ddce1bb833"
//         "7e66d752974626854556bbb681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aec"
//         "f29090ac428a9c43f230a829220d");
//     EXPECT_EQ(TWDataSize(outputData.get()), 185ul);

//     {
//         TW::Ripple::Proto::SigningOutput output;
//         ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()),
//                                             (int)TWDataSize(outputData.get())));

//         EXPECT_EQ(hex(output.encoded()), ExpectedTx);
//         EXPECT_EQ(output.encoded().size(), 182ul);
//         ASSERT_EQ(output.error(), TW::Common::Proto::SigningError::OK);
//     }

//     { // Double check: check if simple signature process gives the same result. Note that private
//       // keys were not used anywhere up to this point.
//         Ripple::Proto::SigningInput signingInput;
//         ASSERT_TRUE(signingInput.ParseFromArray(TWDataBytes(txInputData.get()),
//                                          (int)TWDataSize(txInputData.get())));
//         signingInput.set_private_key(key.data(), key.size());

//         Ripple::Proto::SigningOutput output;
//         ANY_SIGN(signingInput, coin);

//         ASSERT_EQ(hex(output.encoded()), ExpectedTx);
//     }
// }

TEST(TWTransactionCompiler, ExternalSignatureSignNULSToken) {
    const auto coin = TWCoinTypeNULS;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::NULS::Proto::SigningInput();
    auto from = std::string("NULSd6HgWabfcG6H7NDK2TJvtoU3wxY1YLKwJ");
    auto to = std::string("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe");
    uint32_t chainId = 9;
    uint32_t idassetsId = 1;
    auto amount = TW::store((TW::uint256_t)10000000);
    auto amountStr = std::string(amount.begin(), amount.end());
    auto balance = TW::store((TW::uint256_t)100000000);
    auto balanceStr = std::string(balance.begin(), balance.end());
    auto feePayerBalance = TW::store((TW::uint256_t)100000000);
    auto feePayerBalanceStr = std::string(feePayerBalance.begin(), feePayerBalance.end());
    auto nonce = std::string("0000000000000000");
    auto asset_nonce = std::string("0000000000000000");
    input.set_from(from);
    input.set_to(to);
    input.set_amount(amountStr);
    input.set_chain_id(chainId);
    input.set_idassets_id(idassetsId);
    input.set_nonce(asset_nonce.data(), asset_nonce.size());
    input.set_balance(balanceStr);
    input.set_fee_payer(from);
    input.set_fee_payer_nonce(nonce.data(), nonce.size());
    input.set_fee_payer_balance(feePayerBalanceStr);
    input.set_timestamp((uint32_t)1569228280);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(inputStrData.data(), inputStrData.size()));

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputData.get()));
    auto preImageHashData =
        data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImage = preSigningOutput.data();
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(
        hex(preImage),
        "0200f885885d0000d202170100012c177a01a7afbe98e094007b99476534fb7926b70900010080969800000000"
        "0000000000000000000000000000000000000000000000000008000000000000000000170100012c177a01a7af"
        "be98e094007b99476534fb7926b701000100a08601000000000000000000000000000000000000000000000000"
        "0000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120900010080"
        "969800000000000000000000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(hex(preImageHash),
              "9040642ce845b320453b2ccd6f80efc38fdf61ec8f0c12e0c16f6244ec2e0496");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData =
        parse_hex("033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0aff0ee045");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const auto signature =
        parse_hex("5ddea604c6cdfcf6cbe32f5873937641676ee5f9aee3c40aa9857c59aefedff25b77429cf62307d4"
                  "3a6a79b4c106123e6232e3981032573770fe2726bf9fc07c00");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputData.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&publicKeyData)).get()));

    const auto ExpectedEncoded = parse_hex(
        "0200f885885d0000d202170100012c177a01a7afbe98e094007b99476534fb7926b70900010080969800000000"
        "0000000000000000000000000000000000000000000000000008000000000000000000170100012c177a01a7af"
        "be98e094007b99476534fb7926b701000100a08601000000000000000000000000000000000000000000000000"
        "0000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120900010080"
        "9698000000000000000000000000000000000000000000000000000000000000000000000000006921033c87a3"
        "d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0aff0ee04546304402205ddea604c6cdfcf6cbe32f"
        "5873937641676ee5f9aee3c40aa9857c59aefedff202205b77429cf62307d43a6a79b4c106123e6232e3981032"
        "573770fe2726bf9fc07c");
    const auto ExpectedTx = std::string(ExpectedEncoded.begin(), ExpectedEncoded.end());
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 328ul);
        NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()),
                                          (int)TWDataSize(outputData.get())));
        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 325ul);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        NULS::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(TWDataBytes(txInputData.get()),
                                         (int)TWDataSize(txInputData.get())));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        NULS::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}
