// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWTransactionCompiler.h>
#include <TrustWalletCore/TWData.h>
#include "proto/Binance.pb.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/TransactionCompiler.pb.h"

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
#include <map>

using namespace TW;

TEST(TWTransactionCompiler, ExternalSignatureSignBinance) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeBinance;
    const auto txInputData = WRAPD(TWTransactionCompilerBuildInput(
        coin,
        STRING("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2").get(),  // from
        STRING("bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5").get(),  // to
        STRING("1").get(),  // amount
        STRING("BNB").get(),  // asset
        STRING("").get(),  // memo
        STRING("Binance-Chain-Nile").get()  // testnet chainId
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
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputData.get()));
    auto preImageHash = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    TxCompiler::Proto::PreSigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(preImageHash.data(), int(preImageHash.size())));
    ASSERT_EQ(output.error(), 0);
    const auto preImageHashData = data(output.data_hash());
    
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
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
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

    { // Double check: check if simple signature process gives the same result. Note that private keys were not used anywhere up to this point.
        Binance::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));
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
    const auto txInputData0 = WRAPD(TWTransactionCompilerBuildInput(
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
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputData.get()));
    auto preImageHash = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    TxCompiler::Proto::PreSigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(preImageHash.data(), int(preImageHash.size())));
    ASSERT_EQ(output.error(), 0);
    const auto preImageHashData = data(output.data_hash());
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
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
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

    { // Double check: check if simple signature process gives the same result. Note that private keys were not used anywhere up to this point.
        Ethereum::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));
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
    // Test external signining with a Bircoin transaction with 3 input UTXOs, all used, but only using 2 public keys.
    // Three signatures are neeeded.  This illustrates that order of UTXOs/hashes is not always the same.

    const auto revUtxoHash0 = parse_hex("07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa8");
    const auto revUtxoHash1 = parse_hex("d6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e");
    const auto revUtxoHash2 = parse_hex("6021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d");
    const auto inPubKey0 = parse_hex("024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382");
    const auto inPubKey1 = parse_hex("0217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc");
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
        UtxoInfo {revUtxoHash0, inPubKey0, 600'000, 0},
        // second UTXO, with same pubkey
        UtxoInfo {revUtxoHash1, inPubKey0, 500'000, 1},
        // third UTXO, with different pubkey
        UtxoInfo {revUtxoHash2, inPubKey1, 400'000, 0},
    };

    // Signature infos, indexed by pubkeyhash+hash
    struct SignatureInfo {
        Data signature;
        Data publicKey;
    };
    std::map<std::string, SignatureInfo> signatureInfos = {
        {
            hex(inPubKeyHash0) + "+" + "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7",
            {
                parse_hex("304402201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a40"),
                inPubKey0,
            }
        },
        {
            hex(inPubKeyHash1) + "+" + "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6",
            {
                parse_hex("3044022041294880caa09bb1b653775310fcdd1458da6b8e7d7fae34e37966414fe115820220646397c9d2513edc5974ecc336e9b287de0cdf071c366f3b3dc3ff309213e4e4"),
                inPubKey1,
            }
        },
        {
            hex(inPubKeyHash0) + "+" + "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101",
            {
                parse_hex("30440220764e3d5b3971c4b3e70b23fb700a7462a6fe519d9830e863a1f8388c402ad0b102207e777f7972c636961f92375a2774af3b7a2a04190251bbcb31d19c70927952dc"),
                inPubKey0,
            }
        },
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
    for (auto& u: utxoInfos) {
        const auto publicKey = PublicKey(u.publicKey, TWPublicKeyTypeSECP256k1);
        const auto address = Bitcoin::SegwitAddress(publicKey, "bc");
        if (count == 0) EXPECT_EQ(address.string(), ownAddress);
        if (count == 1) EXPECT_EQ(address.string(), ownAddress);
        if (count == 2) EXPECT_EQ(address.string(), "bc1qveq6hmdvl9yrk7f6lct3s6yue9pqhwcuxedggg");

        const auto utxoScript = Bitcoin::Script::lockScriptForAddress(address.string(), coin);
        if (count == 0) EXPECT_EQ(hex(utxoScript.bytes), "0014bd92088bb7e82d611a9b94fbb74a0908152b784f");
        if (count == 1) EXPECT_EQ(hex(utxoScript.bytes), "0014bd92088bb7e82d611a9b94fbb74a0908152b784f");
        if (count == 2) EXPECT_EQ(hex(utxoScript.bytes), "00146641abedacf9483b793afe1718689cc9420bbb1c");

        Data keyHash;
        EXPECT_TRUE(utxoScript.matchPayToWitnessPublicKeyHash(keyHash));
        if (count == 0) EXPECT_EQ(hex(keyHash), hex(inPubKeyHash0));
        if (count == 1) EXPECT_EQ(hex(keyHash), hex(inPubKeyHash0));
        if (count == 2) EXPECT_EQ(hex(keyHash), hex(inPubKeyHash1));

        const auto redeemScript = Bitcoin::Script::buildPayToPublicKeyHash(keyHash);
        if (count == 0) EXPECT_EQ(hex(redeemScript.bytes), "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac");
        if (count == 1) EXPECT_EQ(hex(redeemScript.bytes), "76a914bd92088bb7e82d611a9b94fbb74a0908152b784f88ac");
        if (count == 2) EXPECT_EQ(hex(redeemScript.bytes), "76a9146641abedacf9483b793afe1718689cc9420bbb1c88ac");
        (*input.mutable_scripts())[hex(keyHash)] = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());

        auto utxo = input.add_utxo();
        utxo->set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
        utxo->set_amount(u.amount);
        utxo->mutable_out_point()->set_hash(std::string(u.revUtxoHash.begin(), u.revUtxoHash.end()));
        utxo->mutable_out_point()->set_index(u.index);
        utxo->mutable_out_point()->set_sequence(UINT32_MAX);

        ++count;
    }
    EXPECT_EQ(count, 3);
    EXPECT_EQ(input.utxo_size(), 3);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

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
    *input.mutable_plan() = plan;

    // Serialize input
    const auto txInputDataData = data(input.SerializeAsString());
    const auto txInputData = WRAPD(TWDataCreateWithBytes(txInputDataData.data(), txInputDataData.size()));
    EXPECT_EQ((int)TWDataSize(txInputData.get()), 692);

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
        const auto& preImageHash = TW::data(h.data_hash());
        const auto& pubkeyhash = h.public_key_hash();

        const std::string key = hex(pubkeyhash) + "+" + hex(preImageHash);
        const auto sigInfoFind = signatureInfos.find(key);
        ASSERT_TRUE(sigInfoFind != signatureInfos.end());
        const auto& sigInfo = std::get<1>(*sigInfoFind);
        const auto& publicKeyData = sigInfo.publicKey;
        const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
        const auto signature = sigInfo.signature;

        TWDataVectorAdd(signatureVec.get(), WRAPD(TWDataCreateWithBytes(signature.data(), signature.size())).get());
        TWDataVectorAdd(pubkeyVec.get(), WRAPD(TWDataCreateWithBytes(publicKeyData.data(), publicKeyData.size())).get());
        // Verify signature (pubkey & hash & signature)
        EXPECT_TRUE(publicKey.verifyAsDER(signature, preImageHash));
    }
    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputData.get(), signatureVec.get(), pubkeyVec.get()
    ));

    const auto ExpectedTx = "010000000001036021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d0000000000ffffffffd6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e0100000000ffffffff07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa80000000000ffffffff02804f1200000000001600145360df8231ac5965147c9d90ca930a2aafb05232cb92040000000000160014bd92088bb7e82d611a9b94fbb74a0908152b784f02473044022041294880caa09bb1b653775310fcdd1458da6b8e7d7fae34e37966414fe115820220646397c9d2513edc5974ecc336e9b287de0cdf071c366f3b3dc3ff309213e4e401210217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc0247304402201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a400121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382024730440220764e3d5b3971c4b3e70b23fb700a7462a6fe519d9830e863a1f8388c402ad0b102207e777f7972c636961f92375a2774af3b7a2a04190251bbcb31d19c70927952dc0121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb49338200000000";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 786);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()), (int)TWDataSize(outputData.get())));

        EXPECT_EQ(output.encoded().size(), 518);
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private keys were not used anywhere up to this point.
        Bitcoin::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));

        // 2 private keys are needed (despite >2 UTXOs)
        auto key0 = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
        auto key1 = parse_hex("7878787878787878787878787878787878787878787878787878787878787878");
        EXPECT_EQ(hex(PrivateKey(key0).getPublicKey(TWPublicKeyTypeSECP256k1).bytes), hex(inPubKey0));
        EXPECT_EQ(hex(PrivateKey(key1).getPublicKey(TWPublicKeyTypeSECP256k1).bytes), hex(inPubKey1));
        *input.add_private_key() = std::string(key0.begin(), key0.end());
        *input.add_private_key() = std::string(key1.begin(), key1.end());

        Bitcoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}
