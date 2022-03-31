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
#include "proto/Theta.pb.h"
#include "proto/Cosmos.pb.h"
#include "proto/NEO.pb.h"
#include "proto/NULS.pb.h"
#include "proto/Ripple.pb.h"
#include "proto/Solana.pb.h"
#include "proto/Ontology.pb.h"

#include "proto/Stellar.pb.h"
#include "proto/Tezos.pb.h"
#include "proto/NEAR.pb.h"
#include "proto/IOST.pb.h"
#include "proto/Tron.pb.h"
#include "proto/VeChain.pb.h"

#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWStellarPassphrase.h>

#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"

#include "Base58.h"
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
    auto privateKey =
        PrivateKey(parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722"));
    auto publicKey = privateKey.getPublicKey(::publicKeyType(coin));

    input.set_passphrase(TWStellarPassphrase_Stellar);
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_fee(1000);
    input.set_sequence(2);
    input.mutable_op_payment()->set_destination(
        "GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.mutable_op_payment()->set_amount(10000000);
    auto& memoText = *input.mutable_memo_text();
    memoText.set_text("Hello, world!");

    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
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

TEST(TransactionCompiler, NULSCompileWithSignatures) {
    const auto coin = TWCoinTypeNULS;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::NULS::Proto::SigningInput();
    auto from = std::string("NULSd6HgWabfcG6H7NDK2TJvtoU3wxY1YLKwJ");
    auto to = std::string("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe");
    uint32_t chainId = 1;
    uint32_t idassetsId = 1;
    auto amount = TW::store((TW::uint256_t)10000000);
    auto amountStr = std::string(amount.begin(), amount.end());
    auto nonce = std::string("0000000000000000");
    input.set_from(from);
    input.set_to(to);
    input.set_amount(amountStr);
    input.set_chain_id(chainId);
    input.set_idassets_id(idassetsId);
    input.set_nonce(nonce);
    input.set_balance("100000000");
    input.set_timestamp((uint32_t)1569228280);
    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(hex(preImage),
              "0200f885885d00008c01170100012c177a01a7afbe98e094007b99476534fb7926b701000100201d9a00"
              "000000000000000000000000000000000000000000000000000000000800000000000000000001170100"
              "01f05e7878971f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000"
              "0000000000000000000000000000000000000000000000");
    auto preImageHash = preSigningOutput.datahash();
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
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKeyData});
    const auto ExpectedEncoded = parse_hex(
        "0200f885885d00008c01170100012c177a01a7afbe98e094007b99476534fb7926b701000100201d9a00000000"
        "00000000000000000000000000000000000000000000000000080000000000000000000117010001f05e787897"
        "1f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000000000000000000000"
        "00000000000000000000000000006a21033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0a"
        "ff0ee045473045022100a5234269eab6fe8a1510dd0cb36070a03464b48856e1ef2681dbb79a5ec656f9022029"
        "61ac01d401a6f849acc958c6c9653f49282f5a0916df036ea8766918bac195");
    const auto ExpectedTx = std::string(ExpectedEncoded.begin(), ExpectedEncoded.end());
    EXPECT_EQ(outputData.size(), 259);
    TW::NULS::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    EXPECT_EQ(output.encoded(), ExpectedTx);
    EXPECT_EQ(output.encoded().size(), 256);

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::NULS::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        input.set_private_key(key.data(), key.size());

        TW::NULS::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TransactionCompiler, ThetaCompileWithSignatures) {
    const auto coin = TWCoinTypeTheta;
    /// Step 1: Prepare transaction input (protobuf)
    const auto pkFrom =
        PrivateKey(parse_hex("0x93a90ea508331dfdf27fb79757d4250b4e84954927ba0073cd67454ac432c737"));
    const auto publicKey = pkFrom.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    TW::Theta::Proto::SigningInput input;
    input.set_chain_id("privatenet");
    input.set_to_address("0x9F1233798E905E173560071255140b4A8aBd3Ec6");
    auto amount = store(uint256_t(10));
    input.set_theta_amount(amount.data(), amount.size());
    auto tfuelAmount = store(uint256_t(20));
    input.set_tfuel_amount(tfuelAmount.data(), tfuelAmount.size());
    auto fee = store(uint256_t(1000000000000));
    input.set_fee(fee.data(), fee.size());
    input.set_sequence(1);
    std::string pubkeyStr(publicKey.bytes.begin(), publicKey.bytes.end());
    input.set_public_key(pubkeyStr);
    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.errorcode(), 0);
    EXPECT_EQ(hex(preSigningOutput.datahash()), "2dc419e9919e65f129453419dc72a6bee99b2281dfddf754807a5c212ae35678");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature = parse_hex("5190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preSigningOutput.datahash())));

    /// Step 3: Compile transaction info
    const auto outputData = TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKeyData});

    Theta::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    auto expectedTx = "02f887c78085e8d4a51000f863f861942e833968e5bb786ae419c4d13189fb081cc43babc70a85e8d4a5101401b8415190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501d9d8949f1233798e905e173560071255140b4a8abd3ec6c20a14";
    EXPECT_EQ(hex(output.encoded()), expectedTx);

    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        Theta::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        input.set_private_key(pkFrom.bytes.data(), pkFrom.bytes.size());

        Theta::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), expectedTx);
    }
}

TEST(TransactionCompiler, TezosCompileWithSignatures) {
    const auto coin = TWCoinTypeTezos;

    /// Step 1: Prepare transaction input (protobuf)
    auto privateKey = PrivateKey(parse_hex("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));
    auto publicKey = privateKey.getPublicKey(::publicKeyType(coin));
    auto revealKey = parse_hex("311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff95");

    TW::Tezos::Proto::SigningInput input;
    auto& operations = *input.mutable_operation_list();
    operations.set_branch("BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp");

    auto& reveal = *operations.add_operations();
    auto& revealData = *reveal.mutable_reveal_operation_data();
    revealData.set_public_key(revealKey.data(), revealKey.size());
    reveal.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    reveal.set_fee(1272);
    reveal.set_counter(30738);
    reveal.set_gas_limit(10100);
    reveal.set_storage_limit(257);
    reveal.set_kind(Tezos::Proto::Operation::REVEAL);

    auto& transaction = *operations.add_operations();
    auto& txData = *transaction.mutable_transaction_operation_data();
    txData.set_amount(1);
    txData.set_destination("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transaction.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transaction.set_fee(1272);
    transaction.set_counter(30739);
    transaction.set_gas_limit(10100);
    transaction.set_storage_limit(257);
    transaction.set_kind(Tezos::Proto::Operation::TRANSACTION);

    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.errorcode(), 0);

    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImageHash), "12e4f8b17ad3b316a5a56960db76c7d6505dbf2fff66106be75c8d6753daac0e");

    auto signature = parse_hex("0217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05");

    /// Step 3: Compile transaction info
    const auto outputData = TransactionCompiler::compileWithSignatures(coin, inputData, { signature }, {});

    TW::Tezos::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    const auto tx = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff956c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f741ef614b0e35fcc8c90dfa3b0b95721000217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05";
    EXPECT_EQ(hex(output.encoded()), tx);

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Tezos::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Tezos::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), tx);
    }
}

TEST(TransactionCompiler, NEARCompileWithSignatures) {
    auto privateKeyBytes = Base58::bitcoin.decode(
        "3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    auto publicKey = Base58::bitcoin.decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");
    const auto coin = TWCoinTypeNEAR;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::NEAR::Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_receiver_id("whatever.near");
    input.set_nonce(1);
    auto blockHash = Base58::bitcoin.decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_public_key(publicKey.data(), publicKey.size());

    input.add_actions();
    auto& transfer = *input.mutable_actions(0)->mutable_transfer();
    Data deposit(16, 0);
    deposit[0] = 1;
    transfer.set_deposit(deposit.data(), deposit.size());
    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.errorcode(), 0);
    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImageHash),
              "eea6e680f3ea51a7f667e9a801d0bfadf66e03d41ed54975b3c6006351461b32");
    auto signature = parse_hex("969a83332186ee9755e4839325525806e189a3d2d2bb4b4760e94443e97e1c4f22d"
                               "eeef0059a8e9713100eda6e19144da7e8a0ef7e539b20708ba1d8d021bd01");

    /// Step 3: Compile transaction info
    const auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKey});

    TW::NEAR::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    const auto tx = "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341"
                    "538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901d"
                    "f296be6adc4cc4df34d040efa2435224b6986910e630c2fef60100000003010000000000000000"
                    "0000000000000000969a83332186ee9755e4839325525806e189a3d2d2bb4b4760e94443e97e1c"
                    "4f22deeef0059a8e9713100eda6e19144da7e8a0ef7e539b20708ba1d8d021bd01";
    EXPECT_EQ(hex(output.signed_transaction()), tx);
    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        TW::NEAR::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        input.set_private_key(privateKeyBytes.data(), 32);

        TW::NEAR::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.signed_transaction()), tx);
    }
}

TEST(TransactionCompiler, IostCompileWithSignatures) {
    const auto coin = TWCoinTypeIOST;
    /// Step 1: Prepare transaction input (protobuf)
    const auto privKeyBytes = Base58::bitcoin.decode(
        "4TQwN7wWXg26ByuU5WkUPErd5v6PD6HsDuULyGNJgpS979wXF7jRU8NKviJs5boHrRKbLMomKycbek4NyDy6cLb8");
    const auto pkFrom = PrivateKey(Data(privKeyBytes.begin(), privKeyBytes.begin() + 32));
    const auto publicKey = pkFrom.getPublicKey(TWPublicKeyTypeED25519);
    TW::IOST::Proto::SigningInput input;
    input.set_transfer_memo("");
    auto t = input.mutable_transaction_template();
    t->set_publisher("astastooo");
    t->set_time(1647421579901555000);
    t->set_expiration(1647421879901555000);
    t->set_gas_ratio(1);
    t->set_gas_limit(100000);
    t->set_chain_id(1023);
    t->set_delay(0);
    t->set_publisher("astastooo");
    t->add_actions();
    auto action = t->mutable_actions(0);
    action->set_contract("token.iost");
    action->set_action_name("transfer");
    action->set_data("[\"IOST\",\"astastooo\",\"test_iosted\",\"0.001\",\"test\"]");
    t->add_amount_limit();
    auto amountLimit = t->mutable_amount_limit(0);
    amountLimit->set_token("*");
    amountLimit->set_value("unlimited");
    input.mutable_account()->set_active_key(privKeyBytes.data(), 32);

    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.errorcode(), 0);

    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImageHash),
              "451ed1e542da2422ed152bff6f30c95e2a8ee2153f4d36f15c45914fa2d2e9f1");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature =
        parse_hex("1e5e2de66512658e9317fa56766678166abcf492d020863935723db2030f736710e13437cef0981fcc376eae45349759508767d407b6c9963712910ada2c3606"
                  );

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preSigningOutput.datahash())));

    /// Step 3: Compile transaction info
    const auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKeyData});

    IOST::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
    const auto tx = "7b2274696d65223a2231363437343231353739393031353535303030222c2265787069726174696f6e223a2231363437343231383739393031353535303030222c226761735f726174696f223a312c226761735f6c696d6974223a3130303030302c22636861696e5f6964223a313032332c22616374696f6e73223a5b7b22636f6e7472616374223a22746f6b656e2e696f7374222c22616374696f6e5f6e616d65223a227472616e73666572222c2264617461223a225b5c22494f53545c222c5c226173746173746f6f6f5c222c5c22746573745f696f737465645c222c5c22302e3030315c222c5c22746573745c225d227d5d2c22616d6f756e745f6c696d6974223a5b7b22746f6b656e223a222a222c2276616c7565223a22756e6c696d69746564227d5d2c227075626c6973686572223a226173746173746f6f6f222c227075626c69736865725f73696773223a5b7b22616c676f726974686d223a322c227369676e6174757265223a22486c3474356d55535a593654462f7057646d5a34466d7138394a4c51494959354e58493973674d5063326351345451337a76435948387733627135464e4a645a5549646e31416532795a59334570454b3269773242673d3d222c227075626c69635f6b6579223a2234687a496d2b6d383234536f663866645641474545332b64667931554c7a69786e6f4c5047694a565879383d227d5d7d";
    EXPECT_EQ(hex(output.encoded()), tx);

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::IOST::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));

        TW::IOST::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), tx);
    }
}

TEST(TransactionCompiler, TronCompileWithSignatures) {
    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    const auto coin = TWCoinTypeTron;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Tron::Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& transfer = *transaction.mutable_transfer_asset();
    transfer.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    transfer.set_to_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    transfer.set_amount(4);
    transfer.set_asset_name("1000959");
    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1541890116000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1541890116000);
    const auto txTrieRoot = parse_hex("845ab51bf63c2c21ee71a4dc0ac3781619f07a7cd05e1e0bd8ba828979332ffa");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000003cb800a7e69e9144e3d16f0cf33f33a95c7ce274097822c67243c1");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3979265);
    const auto witnessAddress = parse_hex("41b487cdc02de90f15ac89a68c82f44cbfe3d915ea");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.errorcode(), 0);
    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImageHash),
              "546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb");
    auto signature = parse_hex("77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603a183aa12124adeee7991bf55acc8e488a6ca04fb393b1a8ac16610eeafdfc00");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preSigningOutput.datahash())));
    /// Step 3: Compile transaction info
    const auto outputData =
            TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKey.bytes});

    TW::Tron::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    const auto tx = "{\"raw_data\":{\"contract\":[{\"parameter\":{\"type_url\":\"type.googleapis.com/protocol.TransferAssetContract\",\"value\":{\"amount\":4,\"asset_name\":\"31303030393539\",\"owner_address\":\"415cd0fb0ab3ce40f3051414c604b27756e69e43db\",\"to_address\":\"41521ea197907927725ef36d70f25f850d1659c7c7\"}},\"type\":\"TransferAssetContract\"}],\"expiration\":1541926116000,\"ref_block_bytes\":\"b801\",\"ref_block_hash\":\"0e2bc08d550f5f58\",\"timestamp\":1539295479000},\"signature\":[\"77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603a183aa12124adeee7991bf55acc8e488a6ca04fb393b1a8ac16610eeafdfc00\"],\"txID\":\"546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb\"}";
    EXPECT_EQ(output.json(), tx);
    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        TW::Tron::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        input.set_private_key(privateKey.bytes.data(), 32);

        TW::Tron::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(output.json(), tx);
    }
}

TEST(TransactionCompiler, RippleCompileWithSignatures) {
    const auto coin = TWCoinTypeXRP;
    /// Step 1: Prepare transaction input (protobuf)
    auto key = parse_hex("ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764");
    auto input = TW::Ripple::Proto::SigningInput();
    auto privateKey = TW::PrivateKey(key);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    input.set_amount(29000000);
    input.set_fee(200000);
    input.set_sequence(1);
    input.set_account("rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF");
    input.set_destination("rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF");
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());
    auto inputString = input.SerializeAsString();
    auto inputData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(hex(preImage),
              "5354580012000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92"
              "cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b681148400b6b6d08d5d495653d73e"
              "da6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d");
    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImageHash),
              "8624dbbd5da9ccc8f7a50faf8af8709837db72f51a50cac15a6cd28ce6107b3d");
    // Simulate signature, normally obtained from signature server
    const auto signature =
        parse_hex("30440220067f20b3eebfc7107dd0bcc72337a236ac3be042c0469f2341d76694a17d4bb902204839"
                  "3d7ee7dcb729783b33f5038939ddce1bb8337e66d752974626854556bbb6");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verifyAsDER(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKey.bytes});
    const auto ExpectedTx = std::string(
        "12000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92cefb3a45"
        "37b3edb0b6044c04af27c01583c577823ecc69a9a21119b6744630440220067f20b3eebfc7107dd0bcc7"
        "2337a236ac3be042c0469f2341d76694a17d4bb9022048393d7ee7dcb729783b33f5038939ddce1bb833"
        "7e66d752974626854556bbb681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aec"
        "f29090ac428a9c43f230a829220d");
    EXPECT_EQ(outputData.size(), 185);

    TW::Ripple::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    EXPECT_EQ(output.encoded().size(), 182);
    ASSERT_EQ(output.error(), TW::Common::Proto::SigningError::OK);

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Ripple::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(inputData.data(), (int)inputData.size()));
        input.set_private_key(key.data(), key.size());

        TW::Ripple::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

TEST(TWTransactionCompiler, CosmosCompileWithSignatures) {
    const auto coin = TWCoinTypeCosmos;
    TW::Cosmos::Proto::SigningInput input;

    PrivateKey privateKey = PrivateKey( parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    /// Step 1: Prepare transaction input (protobuf)
    input.set_account_number(546179);
    input.set_chain_id("cosmoshub-4");
    input.set_memo("");
    input.set_sequence(0);

    PublicKey publicKey = privateKey.getPublicKey(TWCoinTypePublicKeyType(coin));
    const auto pubKeyBz = publicKey.bytes;
    input.set_public_key(pubKeyBz.data(), pubKeyBz.size());

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx");
    message.set_to_address("cosmos18s0hdnsllgcclweu9aymw4ngktr2k0rkygdzdp");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("uatom");
    amountOfTx->set_amount("400000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uatom");
    amountOfFee->set_amount("1000");

    /// Step 2: Obtain protobuf preimage hash
    input.set_signing_mode(TW::Cosmos::Proto::Protobuf);
    auto protoInputString = input.SerializeAsString();
    auto protoInputData = TW::Data(protoInputString.begin(), protoInputString.end());

    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, protoInputData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.errorcode(), 0);
    auto preImage = preSigningOutput.data();
    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImage), "0a92010a8f010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e64126f0a2d636f736d6f73316d6b793639636e38656b74777930383435766563397570736470686b7478743033676b776c78122d636f736d6f733138733068646e736c6c6763636c7765753961796d77346e676b7472326b30726b7967647a64701a0f0a057561746f6d120634303030303012650a4e0a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a2102ecef5ce437a302c67f95468de4b31f36e911f467d7e6a52b41c1e13e1d56364912040a02080112130a0d0a057561746f6d12043130303010c09a0c1a0b636f736d6f736875622d342083ab21");
    EXPECT_EQ(hex(preImageHash), "fa7990e1814c900efaedf1bdbedba22c22336675befe0ae39974130fc204f3de");

    TW::Cosmos::Proto::SigningOutput output;
    ANY_SIGN(input, coin);

    assertJSONEqual(output.serialized(), "{\"tx_bytes\": \"CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseBItY29zbW9zMThzMGhkbnNsbGdjY2x3ZXU5YXltdzRuZ2t0cjJrMHJreWdkemRwGg8KBXVhdG9tEgY0MDAwMDASZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJEgQKAggBEhMKDQoFdWF0b20SBDEwMDAQwJoMGkCvvVE6d29P30cO9/lnXyGunWMPxNY12NuqDcCnFkNM0H4CUQdl1Gc9+ogIJbro5nyzZzlv9rl2/GsZox/JXoCX\", \"mode\": \"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "afbd513a776f4fdf470ef7f9675f21ae9d630fc4d635d8dbaa0dc0a716434cd07e02510765d4673dfa880825bae8e67cb367396ff6b976fc6b19a31fc95e8097");
    ASSERT_TRUE(publicKey.verify(data(output.signature()), data(preImageHash.data())));
    EXPECT_EQ(hex(output.serialized()), "7b226d6f6465223a2242524f4144434153545f4d4f44455f424c4f434b222c2274785f6279746573223a2243704942436f3842436877765932397a6257397a4c6d4a68626d7375646a46695a5852684d53354e633264545a57356b456d384b4c574e7663323176637a467461336b324f574e754f475672644864354d4467304e585a6c597a6c3163484e6b63476872644868304d444e6e61336473654249745932397a6257397a4d54687a4d47686b626e4e736247646a593278335a58553559586c74647a52755a327430636a4a724d484a726557646b656d52774767384b4258566864473974456759304d4441774d4441535a51704f436b594b4879396a62334e7462334d7559334a35634852764c6e4e6c593341794e545a724d53355164574a4c5a586b5349776f6841757a76584f51336f774c47663556476a65537a487a62704566526e312b616c4b30484234543464566a5a4a4567514b4167674245684d4b44516f466457463062323053424445774d444151774a6f4d476b437676564536643239503330634f392f6c6e587947756e574d50784e5931324e75714463436e466b4e4d304834435551646c314763392b6f67494a62726f356e797a5a7a6c7639726c322f47735a6f782f4a586f4358227d");

    /// Step 3: Obtain json preimage hash
    input.set_signing_mode(TW::Cosmos::Proto::JSON);
    auto jsonInputString = input.SerializeAsString();
    auto jsonInputData = TW::Data(jsonInputString.begin(), jsonInputString.end());

    const auto jsonPreImageHashData = TransactionCompiler::preImageHashes(coin, jsonInputData);
    auto jsonPreSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(jsonPreSigningOutput.ParseFromArray(jsonPreImageHashData.data(), (int)jsonPreImageHashData.size()));
    ASSERT_EQ(jsonPreSigningOutput.errorcode(), 0);
    auto jsonPreImage = jsonPreSigningOutput.data();
    auto jsonPreImageHash = jsonPreSigningOutput.datahash();
    EXPECT_EQ(hex(jsonPreImage), "7b226163636f756e745f6e756d626572223a22353436313739222c22636861696e5f6964223a22636f736d6f736875622d34222c22666565223a7b22616d6f756e74223a5b7b22616d6f756e74223a2231303030222c2264656e6f6d223a227561746f6d227d5d2c22676173223a22323030303030227d2c226d656d6f223a22222c226d736773223a5b7b2274797065223a22636f736d6f732d73646b2f4d736753656e64222c2276616c7565223a7b22616d6f756e74223a5b7b22616d6f756e74223a22343030303030222c2264656e6f6d223a227561746f6d227d5d2c2266726f6d5f61646472657373223a22636f736d6f73316d6b793639636e38656b74777930383435766563397570736470686b7478743033676b776c78222c22746f5f61646472657373223a22636f736d6f733138733068646e736c6c6763636c7765753961796d77346e676b7472326b30726b7967647a6470227d7d5d2c2273657175656e6365223a2230227d");
    EXPECT_EQ(hex(jsonPreImageHash), "0a31f6cd50f1a5c514929ba68a977e222a7df2dc11e8470e93118cc3545e6b37");
}

TEST(TWTransactionCompiler, VechainCompileWithSignatures) {
    const auto coin = TWCoinTypeVeChain;

    /// Step 1: Prepare transaction input (protobuf)
    TW::VeChain::Proto::SigningInput input;
    PrivateKey privateKey = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.set_chain_tag(1);
    input.set_block_ref(1);
    input.set_expiration(1);
    input.set_gas_price_coef(0);
    input.set_gas(21000);
    input.set_nonce(1);

    auto& clause = *input.add_clauses();
    auto amount = parse_hex("31303030"); // 1000
    clause.set_to("0x3535353535353535353535353535353535353535");
    clause.set_value(amount.data(), amount.size());

    auto stringInput = input.SerializeAsString();
    auto dataInput = TW::Data(stringInput.begin(), stringInput.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, dataInput);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.errorcode(), 0);

    auto preImage = preSigningOutput.data();
    auto preImageHash = preSigningOutput.datahash();
    EXPECT_EQ(hex(preImage), "e7010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0");
    EXPECT_EQ(hex(preImageHash), "a1b8ef3af3d8c74e97ac6cd732916a8f4c38c0905c8b70d2fa598edf1f62ea04");

    /// Step 3: Sign
    TW::VeChain::Proto::SigningOutput output;
    ANY_SIGN(input, coin);
    ASSERT_EQ(hex(output.encoded()), "f86a010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0b841bf8edf9600e645b5abd677cb52f585e7f655d1361075d511b37f707a9f31da6702d28739933b264527a1d05b046f5b74044b88c30c3f5a09d616bd7a4af4901601");

    /// Step 4: Verify signature
    ASSERT_TRUE(privateKey.getPublicKey(TWCoinTypePublicKeyType(coin)).verify(data(output.signature()), data(preImageHash.data())));
}

TEST(TransactionCompiler, OntCompileWithSignatures) {
    /// Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeOntology;
    auto input = Ontology::Proto::SigningInput();
    input.set_method("transfer");
    input.set_owner_address("AaCTzuhEr6essEEKnSTuxD2GJkmGc4nuJp");
    input.set_to_address("AWBzyqpXcSpgrWyzR6qzUGWc9ZoYT3Bsvk");
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
    auto ontPreImageHash = ontPreOutput.datahash();

    input.set_contract("ONG");
    auto ongTxInputData = data(input.SerializeAsString());
    const auto ongPreImageHashes = TransactionCompiler::preImageHashes(coin, ongTxInputData);
    auto ongPreOutput = TxCompiler::Proto::PreSigningOutput();
    ongPreOutput.ParseFromArray(ongPreImageHashes.data(), (int)ongPreImageHashes.size());
    auto ongPreImageHash = ongPreOutput.datahash();

    {
        EXPECT_EQ(hex(ontPreImageHash), "d3770eb50f1fcddc17ac9d59f1b7e69c4916dbbe4c484cc6ba27dd0792aeb943");
        EXPECT_EQ(hex(ongPreImageHash), "788066583071cfd05a4a10e5b897b9b81d2363c16fd98128ddc81891535567af");
    }

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = parse_hex("038ea73c590f48c7d5a8ba544a928a0c8fb206aab60688793a054db9823602765a");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeNIST256p1);
    const auto ontSignature = parse_hex("b1678dfcda9b9b468d9a97a5b3021a680814180ca08cd17d9e3a9cf512b05a3b286fed9b8f635718c0aabddc9fc1acfbc48561577e35ef92ee97d7fa86e14f47");
    const auto ongSignature = parse_hex("d90c4d76e9d07d3e5c00e4a8768ce09ca66be05cfb7f48ad02632b4f08fcaa6f4e3f6f52eb4278c1579065e54ea5e696b7711f071298576fa7050b21ae614bbe");

    // Verify signature (pubkey & hash & signature)
    {
        EXPECT_TRUE(publicKey.verify(ontSignature, TW::data(ontPreImageHash)));
        EXPECT_TRUE(publicKey.verify(ongSignature, TW::data(ongPreImageHash)));
    }

    /// Compile transaction info
    const Data ontOutputData = TransactionCompiler::compileWithSignatures(coin, ontTxInputData, {ontSignature}, {publicKeyData});
    const Data ongOutputData = TransactionCompiler::compileWithSignatures(coin, ongTxInputData, {ongSignature}, {publicKeyData});
    auto ontOutput = Ontology::Proto::SigningOutput();
    auto ongOutput = Ontology::Proto::SigningOutput();
    ontOutput.ParseFromArray(ontOutputData.data(), ontOutputData.size());
    ongOutput.ParseFromArray(ongOutputData.data(), ongOutputData.size());
    const auto ontExpectedTx = "00d101000000ac0d000000000000204e000000000000ca18ec37ac94f19588926a5302ded54cd909a76e7100c66b14ca18ec37ac94f19588926a5302ded54cd909a76e6a7cc8149e21dda3257e18eb033d9451dda1d9ac8bcfa4776a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b6500014140b1678dfcda9b9b468d9a97a5b3021a680814180ca08cd17d9e3a9cf512b05a3b286fed9b8f635718c0aabddc9fc1acfbc48561577e35ef92ee97d7fa86e14f472321038ea73c590f48c7d5a8ba544a928a0c8fb206aab60688793a054db9823602765aac";
    const auto ongExpectedTx = "00d101000000ac0d000000000000204e000000000000ca18ec37ac94f19588926a5302ded54cd909a76e7100c66b14ca18ec37ac94f19588926a5302ded54cd909a76e6a7cc8149e21dda3257e18eb033d9451dda1d9ac8bcfa4776a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b6500014140d90c4d76e9d07d3e5c00e4a8768ce09ca66be05cfb7f48ad02632b4f08fcaa6f4e3f6f52eb4278c1579065e54ea5e696b7711f071298576fa7050b21ae614bbe2321038ea73c590f48c7d5a8ba544a928a0c8fb206aab60688793a054db9823602765aac";

    {
        EXPECT_EQ(hex(ontOutput.encoded()), ontExpectedTx);
        EXPECT_EQ(hex(ongOutput.encoded()), ongExpectedTx);
    }
}
