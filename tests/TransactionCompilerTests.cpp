// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "TransactionCompiler.h"
#include "proto/Binance.pb.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Common.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/NEO.pb.h"
#include "proto/Solana.pb.h"
#include "proto/Stellar.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWStellarPassphrase.h>

#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "uint256.h"

#include "interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TransactionCompiler, BinanceCompileWithSignatures) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeBinance;
    const auto txInputData =
        TransactionCompiler::buildInput(coin,
                                        "bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", // from
                                        "bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5", // to
                                        "1",                                          // amount
                                        "BNB",                                        // asset
                                        "",                                           // memo
                                        ""                                            // chainId
        );

    {
        // Check, by parsing
        EXPECT_EQ(txInputData.size(), 88);
        Binance::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(txInputData.data(), (int)txInputData.size()));
        EXPECT_EQ(input.chain_id(), "Binance-Chain-Nile");
        EXPECT_TRUE(input.has_send_order());
        ASSERT_EQ(input.send_order().inputs_size(), 1);
        EXPECT_EQ(hex(data(input.send_order().inputs(0).address())),
                  "40c2979694bbc961023d1d27be6fc4d21a9febe6");
    }

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0);

    TxCompiler::Proto::PreSigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(output.errorcode(), 0);

    auto preImageHash = data(output.datahash());
    EXPECT_EQ(hex(preImageHash), "3f3fece9059e714d303a9a1496ddade8f2c38fa78fc4cc2e505c5dbb0ea678d1");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData =
        parse_hex("026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const auto signature =
        parse_hex("1b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5"
                  "366bb8b041781a6771248550d04094c3d7a504f9e8310679");

    // Verify signature (pubkey & hash & signature)
    { EXPECT_TRUE(publicKey.verify(signature, preImageHash)); }

    /// Step 3: Compile transaction info
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKeyData});

    const auto ExpectedTx =
        "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001"
        "121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35"
        "920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c"
        "253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a5"
        "04f9e8310679";
    {
        EXPECT_EQ(outputData.size(), 189);
        Binance::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Binance::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(txInputData.data(), (int)txInputData.size()));
        auto key = parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832");
        input.set_private_key(key.data(), key.size());

        Binance::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

TEST(TransactionCompiler, BitcoinCompileWithSignatures) {
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
        (*input.mutable_scripts())[hex(keyHash)] =
            std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());

        auto utxo = input.add_utxo();
        utxo->set_script(utxoScript.bytes.data(), utxoScript.bytes.size());
        utxo->set_amount(u.amount);
        utxo->mutable_out_point()->set_hash(
            std::string(u.revUtxoHash.begin(), u.revUtxoHash.end()));
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
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_EQ((int)txInputData.size(), 692);

    /// Step 2: Obtain preimage hashes
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    TW::Bitcoin::Proto::PreSigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(preImageHashes.data(), (int)preImageHashes.size()));

    ASSERT_EQ(output.errorcode(), 0);
    EXPECT_EQ(hex(output.hashpublickeys()[0].datahash()),
              "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6");
    EXPECT_EQ(hex(output.hashpublickeys()[1].datahash()),
              "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7");
    EXPECT_EQ(hex(output.hashpublickeys()[2].datahash()),
              "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101");
    EXPECT_EQ(hex(output.hashpublickeys()[0].publickeyhash()), hex(inPubKeyHash1));
    EXPECT_EQ(hex(output.hashpublickeys()[1].publickeyhash()), hex(inPubKeyHash0));
    EXPECT_EQ(hex(output.hashpublickeys()[2].publickeyhash()), hex(inPubKeyHash0));

    // Simulate signatures, normally obtained from signature server.
    std::vector<Data> signatureVec;
    std::vector<Data> pubkeyVec;
    for (const auto& h : output.hashpublickeys()) {
        const auto& preImageHash = h.datahash();
        const auto& pubkeyhash = h.publickeyhash();

        const std::string key = hex(pubkeyhash) + "+" + hex(preImageHash);
        const auto sigInfoFind = signatureInfos.find(key);
        ASSERT_TRUE(sigInfoFind != signatureInfos.end());
        const auto& sigInfo = std::get<1>(*sigInfoFind);
        const auto& publicKeyData = sigInfo.publicKey;
        const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
        const auto signature = sigInfo.signature;

        signatureVec.push_back(signature);
        pubkeyVec.push_back(publicKeyData);

        // Verify signature (pubkey & hash & signature)
        EXPECT_TRUE(
            publicKey.verifyAsDER(signature, TW::Data(preImageHash.begin(), preImageHash.end())));
    }

    /// Step 3: Compile transaction info
    const Data outputData =
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
        EXPECT_EQ(outputData.size(), 786);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded().size(), 518);
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
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signatureVec[0]}, pubkeyVec);
        EXPECT_EQ(outputData.size(), 2);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0);
        EXPECT_EQ(output.error(), Common::Proto::Error_signing);
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
        EXPECT_EQ(outputData.size(), 2);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0);
        EXPECT_EQ(output.error(), Common::Proto::Error_signing);
    }
}

TEST(TransactionCompiler, EthereumCompileWithSignatures) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeEthereum;
    const auto txInputData0 =
        TransactionCompiler::buildInput(coin,
                                        "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F", // from
                                        "0x3535353535353535353535353535353535353535", // to
                                        "1000000000000000000",                        // amount
                                        "ETH",                                        // asset
                                        "",                                           // memo
                                        ""                                            // chainId
        );

    // Check, by parsing
    EXPECT_EQ((int)txInputData0.size(), 61);
    Ethereum::Proto::SigningInput input;
    ASSERT_TRUE(input.ParseFromArray(txInputData0.data(), (int)txInputData0.size()));
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
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_EQ((int)txInputData.size(), 75);

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0);

    TxCompiler::Proto::PreSigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(output.errorcode(), 0);

    auto preImageHash = data(output.datahash());
    EXPECT_EQ(hex(preImageHash), "15e180a6274b2f6a572b9b51823fce25ef39576d10188ecdcd7de44526c47217");

    // Simulate signature, normally obtained from signature server
    const Data publicKeyData =
        parse_hex("044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad711"
                  "9ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232a");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1Extended);
    const auto signature =
        parse_hex("360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a19"
                  "1d844db458893b928f3efbfee90c9febf51ab84c9796677900");

    // Verify signature (pubkey & hash & signature)
    { EXPECT_TRUE(publicKey.verify(signature, preImageHash)); }

    /// Step 3: Compile transaction info
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKeyData});

    const auto ExpectedTx =
        "f86c0b8504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0"
        "360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07ba053bd9d1376e15a191d844db4"
        "58893b928f3efbfee90c9febf51ab84c97966779";
    {
        EXPECT_EQ(outputData.size(), 183);
        Ethereum::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded().size(), 110);
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Ethereum::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(txInputData.data(), (int)txInputData.size()));
        auto key = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
        input.set_private_key(key.data(), key.size());

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

TEST(TransactionCompiler, EthereumBuildTransactionInput) {
    const auto coin = TWCoinTypeEthereum;
    const auto txInputData0 =
        TransactionCompiler::buildInput(coin,
                                        "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F", // from
                                        "0x3535353535353535353535353535353535353535", // to
                                        "1000000000000000000",                        // amount
                                        "ETH",                                        // asset
                                        "Memo",                                       // memo
                                        "05"                                          // chainId
        );

    // Check, by parsing
    EXPECT_EQ((int)txInputData0.size(), 61);
    Ethereum::Proto::SigningInput input;
    ASSERT_TRUE(input.ParseFromArray(txInputData0.data(), (int)txInputData0.size()));
    EXPECT_EQ(hex(input.chain_id()), "05");
    EXPECT_EQ(input.to_address(), "0x3535353535353535353535353535353535353535");
    ASSERT_TRUE(input.transaction().has_transfer());
    EXPECT_EQ(hex(input.transaction().transfer().amount()), "0de0b6b3a7640000");
}

TEST(TransactionCompiler, EthereumBuildTransactionInputInvalidAddress) {
    const auto coin = TWCoinTypeEthereum;
    EXPECT_EXCEPTION(
        TransactionCompiler::buildInput(coin,
                                        "0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F", // from
                                        "__INVALID_ADDRESS__",                        // to
                                        "1000000000000000000",                        // amount
                                        "ETH",                                        // asset
                                        "",                                           // memo
                                        ""                                            // chainId
                                        ),
        "Invalid to address");
}

TEST(TransactionCompiler, SolanaCompileWithSignatures) {
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
    auto inputData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
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
    const Data publicKeyData =
        parse_hex("0d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature =
        parse_hex("a8c610697087eaf8a34b3facbe06f8e9bb9603bb03270dad021ffcd2fc37b6e9efcdcb78b227401f"
                  "000eb9231c67685240890962e44a17fd27fc2ff7b971df03");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKeyData});
    const auto ExpectedTx =
        "5bWxVCP5fuzkKSGby9hnsLranszQJR2evJGTfBrpDQ4rJceW1WxKNrWqVPBsN2QCAGmE6W7VaYkyWjv39HhGrr1Ne2"
        "QSUuHZdyyn7hK4pxzLPMgPG8fY1XvXdppWMaKMLmhriLkckzGKJMaE3pWBRFBKzigXY28714uUNndb7S9hVakxa59h"
        "rLph39CMgAkcj6b8KYvJEkb1YdYytHSZNGi4kVVTNqiicNgPdf1gmG6qz9zVtnqj9JtaD2efdS8qxsKnvNWSgb8Xxb"
        "T6dwyp7msUUi7d27cYaPTpK";
    EXPECT_EQ(outputData.size(), 296);
    Solana::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    EXPECT_EQ(output.encoded(), ExpectedTx);
    EXPECT_EQ(output.encoded().size(), 293);

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        input.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TransactionCompiler, NEOCompileWithSignatures) {
    const auto coin = TWCoinTypeNEO;
    /// Step 1: Prepare transaction input (protobuf)
    const std::string NEO_ASSET_ID = "9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5";
    const std::string GAS_ASSET_ID = "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60";

    TW::NEO::Proto::SigningInput input;
    auto privateKey = PrivateKey(parse_hex("F18B2F726000E86B4950EBEA7BFF151F69635951BC4A31C44F28EE6AF7AEC128"));
    auto publicKey = privateKey.getPublicKey(::publicKeyType(coin));
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
    auto inputData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.errorcode(), 0);

    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImageHash), "7fd5629cfc7cb0f8f01f15fc6d8b37ed1240c4f818d0b397bac65266aa6466da");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature = parse_hex("5046619c8e20e1fdeec92ce95f3019f6e7cc057294eb16b2d5e55c105bf32eb27e1fc01c1858576228f1fef8c0945a8ad69688e52a4ed19f5b85f5eff7e961d7");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    const auto outputData = TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKeyData});

    NEO::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    auto expectedTx = "800000019c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de0100029b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000ea610aa6db39bd8c8556c9569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500fcbbc414000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac0141405046619c8e20e1fdeec92ce95f3019f6e7cc057294eb16b2d5e55c105bf32eb27e1fc01c1858576228f1fef8c0945a8ad69688e52a4ed19f5b85f5eff7e961d7232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac";
    EXPECT_EQ(hex(output.encoded()), expectedTx);
    
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        NEO::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        NEO::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), expectedTx);
    }
}

TEST(TransactionCompiler, StellarCompileWithSignatures) {
    const auto coin = TWCoinTypeStellar;
    /// Step 1: Prepare transaction input (protobuf)
    TW::Stellar::Proto::SigningInput input;
    auto privateKey = PrivateKey(parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722"));
    auto publicKey = privateKey.getPublicKey(::publicKeyType(coin));
    
    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_fee(1000);
    input.set_sequence(2);
    input.mutable_op_payment()->set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.mutable_op_payment()->set_amount(10000000);
    auto& memoText = *input.mutable_memo_text();
    memoText.set_text("Hello, world!");
    
    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.errorcode(), 0);

    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImageHash), "1e8786a0162630b2393e0f6c51f16a2d7860715023cb19bf25cad14490b1f8f3");

    auto signature = parse_hex("5042574491827aaccbce1e2964c05098caba06194beb35e595aabfec9f788516a833f755f18144f4a2eedb3123d180f44e7c16037d00857c5c5b7033ebac2c01");
    
    /// Step 3: Compile transaction info
    const auto outputData = TransactionCompiler::compileWithSignatures(coin, inputData, { signature }, {});

    TW::Stellar::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    const auto tx = "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAEAAAANSGVsbG8sIHdvcmxkIQAAAAAAAAEAAAAAAAAAAQAAAADFgLYxeg6zm/f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBQQldEkYJ6rMvOHilkwFCYyroGGUvrNeWVqr/sn3iFFqgz91XxgUT0ou7bMSPRgPROfBYDfQCFfFxbcDPrrCwB";
    EXPECT_EQ(output.signature(), tx);
    
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Stellar::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Stellar::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(output.signature(), tx);
    }
}