// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Bitcoin.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>

#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/TransactionInput.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionSigner.h"

#include "TxComparisonHelper.h"
#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

const static uint64_t ONE_BTC = 100'000'000ll;

TEST(BitcoinCompiler, CompileWithSignatures) {
    // Test external signining with a Bircoin transaction with 3 input UTXOs, all used, but only
    // using 2 public keys. Three signatures are neeeded.  This illustrates that order of
    // UTXOs/hashes is not always the same.

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
        if (count == 0)
            EXPECT_EQ(hex(redeemScript.bytes),
                      "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac");
        if (count == 1)
            EXPECT_EQ(hex(redeemScript.bytes),
                      "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac");
        if (count == 2)
            EXPECT_EQ(hex(redeemScript.bytes),
                      "76a9146641abedacf9483b793afe1718689cc9420bbb1c88ac");
        (*signingInput.mutable_scripts())[hex(keyHash)] =
            std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());

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

    // Serialize input
    const auto txInputData = data(signingInput.SerializeAsString());
    EXPECT_GT(txInputData.size(), 0ul);

    /// Step 2: Obtain preimage hashes
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    TW::Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), (int)preImageHashes.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].data_hash()), "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6");
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[1].data_hash()), "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7");
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[2].data_hash()), "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101");
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].public_key_hash()), hex(inPubKeyHash1));
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[1].public_key_hash()), hex(inPubKeyHash0));
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[2].public_key_hash()), hex(inPubKeyHash0));

    // Simulate signatures, normally obtained from signature server.
    std::vector<Data> signatureVec;
    std::vector<Data> pubkeyVec;
    for (const auto& h : preSigningOutput.hash_public_keys()) {
        const auto& preImageHash = h.data_hash();
        const auto& pubkeyhash = h.public_key_hash();

        const std::string key = hex(pubkeyhash) + "+" + hex(preImageHash);
        const auto sigInfoFind = signatureInfos.find(key);
        ASSERT_TRUE(sigInfoFind != signatureInfos.end());
        const auto& sigInfo = std::get<1>(*sigInfoFind);
        const auto& publicKeyData = sigInfo.publicKey;
        const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
        const auto signature = sigInfo.signature;

        signatureVec.push_back(signature);
        pubkeyVec.push_back(publicKeyData);

        EXPECT_TRUE(publicKey.verifyAsDER(signature, TW::Data(preImageHash.begin(), preImageHash.end())));
    }

    /// Step 3: Compile transaction info
    auto compileWithSignatures =
        TransactionCompiler::compileWithSignatures(coin, txInputData, signatureVec, pubkeyVec);

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
        EXPECT_EQ(compileWithSignatures.size(), 786ul);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(compileWithSignatures.data(), (int)compileWithSignatures.size()));

        EXPECT_EQ(output.encoded().size(), 518ul);
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Bitcoin::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(txInputData.data(), (int)txInputData.size()));

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

    { // Negative: not enough signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(coin, txInputData, {signatureVec[0]}, pubkeyVec);
        EXPECT_GT(outputData.size(), 1ul);

        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
    { // Negative: invalid public key
        const auto publicKeyBlake =
            parse_hex("b689ab808542e13f3d2ec56fe1efe43a1660dcadc73ce489fde7df98dd8ce5d9");
        EXPECT_EXCEPTION(
            TransactionCompiler::compileWithSignatures(
                coin, txInputData, signatureVec, {pubkeyVec[0], pubkeyVec[1], publicKeyBlake}),
            "Invalid public key");
    }
    { // Negative: wrong signature (formally valid)
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData,
            {parse_hex("415502201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b349022"
                       "00a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f3b51"),
             signatureVec[1], signatureVec[2]},
            pubkeyVec);
        EXPECT_EQ(outputData.size(), 2ul);

        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signing);
    }
}

TEST(BitcoinCompiler, CompileWithSignaturesV2) {
    Bitcoin::Proto::SigningInput inputLegacy;
    auto& input = *inputLegacy.mutable_signing_v2();

    const PrivateKey alicePrivateKey(parse_hex("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657"));
    const auto alicePublicKey = alicePrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto bobPublicKey = parse_hex("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf");

    input.mutable_chain_info()->set_p2pkh_prefix(0);
    input.mutable_chain_info()->set_p2sh_prefix(5);
    input.add_public_keys(alicePublicKey.bytes.data(), alicePublicKey.bytes.size());

    auto txid0 = parse_hex("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b");
    std::reverse(txid0.begin(), txid0.end());

    // Step 1: Prepare transaction input (protobuf)

    auto& builder = *input.mutable_builder();

    auto& utxo0 = *builder.add_inputs();
    utxo0.mutable_out_point()->set_hash(txid0.data(), txid0.size());
    utxo0.mutable_out_point()->set_vout(0);
    utxo0.set_value(ONE_BTC * 50);
    utxo0.set_sighash_type(TWBitcoinSigHashTypeAll);
    // Set the Alice public key as the owner of the P2PKH input.
    utxo0.mutable_script_builder()->mutable_p2pkh()->set_pubkey(alicePublicKey.bytes.data(), alicePublicKey.bytes.size());

    auto& out0 = *builder.add_outputs();
    out0.set_value(ONE_BTC * 50 - 1'000'000);
    // Set the Bob public key as the receiver of the P2PKH output.
    out0.mutable_builder()->mutable_p2pkh()->set_pubkey(bobPublicKey.data(), bobPublicKey.size());

    builder.set_version(BitcoinV2::Proto::TransactionVersion::V2);
    builder.set_input_selector(BitcoinV2::Proto::InputSelector::UseAll);
    builder.set_fixed_dust_threshold(546);

    const auto inputLegacyData = data(inputLegacy.SerializeAsString());

    // Step 2: Obtain preimage hashes

    const auto preOutputData = TransactionCompiler::preImageHashes(TWCoinTypeBitcoin, inputLegacyData);
    Bitcoin::Proto::PreSigningOutput preOutput;
    preOutput.ParseFromArray(preOutputData.data(), static_cast<int>(preOutputData.size()));

    EXPECT_EQ(preOutput.error(), Common::Proto::SigningError::OK);
    EXPECT_TRUE(preOutput.has_pre_signing_result_v2());
    EXPECT_EQ(preOutput.pre_signing_result_v2().error(), Common::Proto::SigningError::OK);

    const auto& sighashes = preOutput.pre_signing_result_v2().sighashes();
    EXPECT_EQ(sighashes.size(), 1);
    const auto& sighash0 = sighashes.Get(0);
    EXPECT_EQ(data(sighash0.public_key()), alicePublicKey.bytes);
    EXPECT_EQ(hex(sighash0.sighash()), "6a0e072da66b141fdb448323d54765cafcaf084a06d2fa13c8aed0c694e50d18");

    // Step 3: Simulate signature, normally obtained from signature server

    const auto sig0 = alicePrivateKey.sign(data(sighash0.sighash()), TWCurveSECP256k1);
    EXPECT_EQ(hex(sig0), "78eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b11a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd45900");

    // Step 4: Compile transaction info

    const std::vector<Data> signatures { sig0 };
    const std::vector<Data> publicKeys { alicePublicKey.bytes };
    const auto outputData = TransactionCompiler::compileWithSignatures(TWCoinTypeBitcoin, inputLegacyData, signatures, publicKeys);
    Bitcoin::Proto::SigningOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_TRUE(output.has_signing_result_v2());
    const auto& outputV2 = output.signing_result_v2();
    EXPECT_EQ(outputV2.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(
        hex(outputV2.encoded()),
        "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000"
    );
    EXPECT_EQ(hex(outputV2.txid()), "c19f410bf1d70864220e93bca20f836aaaf8cdde84a46692616e9f4480d54885");
}
