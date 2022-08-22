// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "TransactionCompiler.h"
#include "proto/Algorand.pb.h"
#include "proto/Binance.pb.h"
#include "proto/Bitcoin.pb.h"
#include "proto/Common.pb.h"
#include "proto/Cosmos.pb.h"
#include "proto/Decred.pb.h"
#include "proto/Ethereum.pb.h"
#include "proto/EOS.pb.h"
#include "proto/Filecoin.pb.h"
#include "proto/NEO.pb.h"
#include "proto/NULS.pb.h"
#include "proto/Ontology.pb.h"
#include "proto/Ripple.pb.h"
#include "proto/Solana.pb.h"
#include "proto/Theta.pb.h"
#include "proto/Harmony.pb.h"
#include "proto/IOST.pb.h"
#include "proto/NEAR.pb.h"
#include "proto/NEO.pb.h"
#include "proto/NULS.pb.h"
#include "proto/Oasis.pb.h"
#include "proto/Ontology.pb.h"
#include "proto/Polkadot.pb.h"
#include "proto/Ripple.pb.h"
#include "proto/Solana.pb.h"
#include "proto/Stellar.pb.h"
#include "proto/Tezos.pb.h"
#include "proto/Theta.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "proto/Tron.pb.h"
#include "proto/VeChain.pb.h"
#include "proto/Oasis.pb.h"
#include "proto/Cardano.pb.h"
#include "proto/IoTeX.pb.h"

#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWStellarPassphrase.h>

#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/TransactionInput.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionSigner.h"
#include "Decred/Signer.h"
#include "Groestlcoin/Signer.h"
#include "Groestlcoin/Transaction.h"
#include "NEO/Address.h"
#include "NEO/Script.h"
#include "NEO/TransactionAttribute.h"
#include "NEO/TransactionAttributeUsage.h"
#include "Zcash/Signer.h"
#include "Zcash/Transaction.h"
#include "Zcash/TransactionBuilder.h"
#include "Zen/Signer.h"
#include "Zen/TransactionBuilder.h"

#include "Base58.h"
#include "Base64.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "uint256.h"

#include "Bitcoin/TxComparisonHelper.h"
#include "interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TransactionCompiler, AlgorandCompileWithSignatures) {
    /// Step 1: Prepare transaction input (protobuf)
    auto privateKey = parse_hex("d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b");
    auto key = PrivateKey(privateKey);
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto note = parse_hex("68656c6c6f");
    auto genesisHash = Base64::decode("wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=");
    
    auto input = Algorand::Proto::SigningInput();
    auto& transaction = *input.mutable_transfer();
    transaction.set_to_address("CRLADAHJZEW2GFY2UPEHENLOGCUOU74WYSTUXQLVLJUJFHEUZOHYZNWYR4");
    transaction.set_amount(1000000000000ull);
    input.set_first_round(1937767ull);
    input.set_last_round(1938767ull);
    input.set_fee(263000ull);
    input.set_genesis_id("mainnet-v1.0");
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_note(note.data(), note.size());
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto txInputData = data(input.SerializeAsString());
    auto preImageHashes = TransactionCompiler::preImageHashes(TWCoinTypeAlgorand, txInputData);
    
    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImage = data(preSigningOutput.data());
    EXPECT_EQ(hex(preImage), "54588aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179");

    // Simulate signature, normally obtained from signature server
    const auto signature = key.sign(preImage, TWCurveED25519);
    
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(TWCoinTypeAlgorand, txInputData, {signature}, {publicKey.bytes});

    const auto ExpectedTx =
        "82a3736967c440baa00062adcdcb5875e4435cdc6885d26bfe5308ab17983c0fda790b7103051fcb111554e5badfc0ac7edf7e1223a434342a9eeed5cdb047690827325051560ba374786e8aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179";
    {
        Algorand::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }
    
    // double check
    {
        Algorand::Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeAlgorand);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            TWCoinTypeAlgorand, txInputData, {signature, signature}, {publicKey.bytes});
        Algorand::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            TWCoinTypeAlgorand, txInputData, {}, {});
        Algorand::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, FilecoinCompileWithSignatures) {
    auto coin = TWCoinTypeFilecoin;

    /// Step 1: Prepare transaction input (protobuf)
    Filecoin::Proto::SigningInput input;
    auto privateKey = parse_hex("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe");
    auto key = PrivateKey(privateKey);
    auto publicKey = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    auto toAddress =
        "f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq";
    uint64_t nonce = 2;
    // 600 FIL
    // auto value = parse_hex("2086ac351052600000");
    auto value = store(uint256_t(600) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));
    uint64_t gasLimit = 1000;
    // auto gasFeeCap = parse_hex("25f273933db5700000");
    auto gasFeeCap = store(uint256_t(700) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));
    // auto gasPremium = parse_hex("2b5e3af16b18800000");
    auto gasPremium = store(uint256_t(800) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));

    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_to(toAddress);
    input.set_nonce(nonce);
    input.set_value(value.data(), value.size());
    input.set_gas_limit(gasLimit);
    input.set_gas_fee_cap(gasFeeCap.data(), gasFeeCap.size());
    input.set_gas_premium(gasPremium.data(), gasPremium.size());
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto txInputData = data(input.SerializeAsString());
    auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    
    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = data(preSigningOutput.data_hash());
    EXPECT_EQ(hex(preImageHash), "8368c0f622b2c529c7fa147d75aa02aaa7fc13fc4847d4dc57e7a5c59048aafe");

    // Simulate signature, normally obtained from signature server
    const auto signature = key.sign(preImageHash, TWCurveSECP256k1);
    
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});

    const auto ExpectedTx = R"({"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"700000000000000000000","GasLimit":1000,"GasPremium":"800000000000000000000","Nonce":2,"To":"f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq","Value":"600000000000000000000"},"Signature":{"Data":"jMRu+OZ/lfppgmqSfGsntFrRLWZnUg3ZYmJTTRLsVt4V1310vR3VKGJpaE6S4sNvDOE6sEgmN9YmfTkPVK2qMgE=","Type":1}})";
    {
        Filecoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        ASSERT_EQ(output.json(), ExpectedTx);
    }
    
    // double check
    {
        Filecoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(output.json(), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Filecoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Filecoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, BinanceCompileWithSignatures) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeBinance;
    const auto txInputData = TransactionCompiler::buildInput(
        coin,
        "bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2",  // from
        "bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5",  // to
        "1",  // amount
        "BNB",  // asset
        "",  // memo
        "Binance-Chain-Nile"  // testnet chainId
    );

    {
        // Check, by parsing
        EXPECT_EQ(txInputData.size(), 88ul);
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
    ASSERT_GT(preImageHashes.size(), 0ul);

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = data(preSigningOutput.data_hash());
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
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKeyData});

    const auto ExpectedTx =
        "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001"
        "121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35"
        "920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c"
        "253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a5"
        "04f9e8310679";
    {
        EXPECT_EQ(outputData.size(), 189ul);
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

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Binance::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Binance::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
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

    // Serialize input
    const auto txInputData = data(signingInput.SerializeAsString());

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
    for (const auto& h: preSigningOutput.hash_public_keys()) {
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
    const Data compileWithSignatures = TransactionCompiler::compileWithSignatures(coin, txInputData, signatureVec, pubkeyVec);

    const auto ExpectedTx = "010000000001036021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d0000000000ffffffffd6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e0100000000ffffffff07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa80000000000ffffffff02804f1200000000001600145360df8231ac5965147c9d90ca930a2aafb05232cb92040000000000160014bd92088bb7e82d611a9b94fbb74a0908152b784f02473044022041294880caa09bb1b653775310fcdd1458da6b8e7d7fae34e37966414fe115820220646397c9d2513edc5974ecc336e9b287de0cdf071c366f3b3dc3ff309213e4e401210217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc0247304402201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a400121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382024730440220764e3d5b3971c4b3e70b23fb700a7462a6fe519d9830e863a1f8388c402ad0b102207e777f7972c636961f92375a2774af3b7a2a04190251bbcb31d19c70927952dc0121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb49338200000000";
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
        EXPECT_EQ(hex(PrivateKey(key0).getPublicKey(TWPublicKeyTypeSECP256k1).bytes), hex(inPubKey0));
        EXPECT_EQ(hex(PrivateKey(key1).getPublicKey(TWPublicKeyTypeSECP256k1).bytes), hex(inPubKey1));
        *input.add_private_key() = std::string(key0.begin(), key0.end());
        *input.add_private_key() = std::string(key1.begin(), key1.end());

        Bitcoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    {   // Negative: not enough signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(coin, txInputData, {signatureVec[0]}, pubkeyVec);
        EXPECT_GT(outputData.size(), 1ul);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
    {   // Negative: invalid public key
        const auto publicKeyBlake = parse_hex("b689ab808542e13f3d2ec56fe1efe43a1660dcadc73ce489fde7df98dd8ce5d9");
        EXPECT_EXCEPTION(TransactionCompiler::compileWithSignatures(coin, txInputData, signatureVec,
            {pubkeyVec[0], pubkeyVec[1], publicKeyBlake}), "Invalid public key");
    }
    {   // Negative: wrong signature (formally valid)
        const Data outputData = TransactionCompiler::compileWithSignatures(coin, txInputData,
            {parse_hex("415502201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f3b51"),
            signatureVec[1], signatureVec[2]},
            pubkeyVec);
        EXPECT_EQ(outputData.size(), 2ul);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signing);
    }
}

TEST(TransactionCompiler, BitcoinDiamondCompileWithSignatures) {
    // tx on mainnet
    // http://explorer.btcd.io/#/tx?tx=6f8db2317c0940ff97c461e5e9b89692c6c1fded15fb30ae8b9cc2429ce43f66

    const auto coin = TWCoinTypeBitcoinDiamond;
    const int64_t amount = 196007725;
    const int64_t fee = 1014;
    const std::string toAddress = "39mKL9gxk29f2RiofywHYRDmgXPv1ur8uC";

    auto toScript = Bitcoin::Script::lockScriptForAddress(toAddress, coin);
    ASSERT_EQ(hex(toScript.bytes), "a914589133651fd11901381ecb4d3beef58bc28ba2e787");

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("15ehpdrZqfZ5rj2e4T4hZKMi3kA8qdSyQu");
    input.set_coin_type(coin);

    auto txHash0 = parse_hex("6ce528c1192a9be648dd8c960695a15454c4c77b5a1dd5c8a5a208e6ae7e0ca8");
    std::reverse(txHash0.begin(), txHash0.end());
    
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(501008739);

    auto utxoAddr0 = "15ehpdrZqfZ5rj2e4T4hZKMi3kA8qdSyQu";
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, coin);
    ASSERT_EQ(hex(script0.bytes), "76a9143301f83977102415e34cccd5ca15136a3dba87d588ac");
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    EXPECT_EQ(input.utxo_size(), 1);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(305000000);
    auto preBlockHash = parse_hex("840980d100724999ea20e8b14ddd5ea5e37e2beacb9157a17fe87d0854bc7e6f");
    std::reverse(preBlockHash.begin(), preBlockHash.end());
    plan.set_preblockhash(preBlockHash.data(), (int)preBlockHash.size());

    // Extend input with accepted plan
    *input.mutable_plan() = plan;

    // Serialize input
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT((int)txInputData.size(), 0);

    /// Step 2: Obtain preimage hashes
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    TW::Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), (int)preImageHashes.size()));

    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].data_hash()),
              "13565ac8d1d5a8a721417e0391cd13ea1a212b51b9d6bba093babaa203ed9d74");
    
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].public_key_hash()), "3301f83977102415e34cccd5ca15136a3dba87d5");

    auto publicKeyHex = "02f65e76c2a7c239bd6c8b18dc10b71d463b96c0b0d827c97345e6bbe8ee8f2ddc";
    auto publicKey = PublicKey(parse_hex(publicKeyHex), TWPublicKeyTypeSECP256k1);
    auto preImageHash = preSigningOutput.hash_public_keys()[0].data_hash();
    auto signature = parse_hex("3045022100e2c048cdf844c77275ac92cc27cfc357155d42d9a82d5d22f62247dce7681467022052c57d744a2ea91970b14e8863efdbcb3fb91f6448c027c25a8e86b752acb5ce");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(
        publicKey.verifyAsDER(signature, TW::Data(preImageHash.begin(), preImageHash.end())));

    // Simulate signatures, normally obtained from signature server.
    std::vector<Data> signatureVec;
    std::vector<Data> pubkeyVec;
    signatureVec.push_back(signature);
    pubkeyVec.push_back(publicKey.bytes);

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, signatureVec, pubkeyVec);

    const auto ExpectedTx =
        "0c0000006f7ebc54087de87fa15791cbea2b7ee3a55edd4db1e820ea99497200d180098401a80c7eaee608a2a5c8d51d5a7bc7c45454a19506968cdd48e69b2a19c128e56c000000006b483045022100e2c048cdf844c77275ac92cc27cfc357155d42d9a82d5d22f62247dce7681467022052c57d744a2ea91970b14e8863efdbcb3fb91f6448c027c25a8e86b752acb5ce012102f65e76c2a7c239bd6c8b18dc10b71d463b96c0b0d827c97345e6bbe8ee8f2ddcffffffff022dd7ae0b0000000017a914589133651fd11901381ecb4d3beef58bc28ba2e78740ee2d12000000001976a9143301f83977102415e34cccd5ca15136a3dba87d588ac00000000";
    {
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: not enough signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, pubkeyVec);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
    
    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
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
                coin, txInputData, signatureVec, {publicKeyBlake}),
            "Invalid public key");
    }
    { // Negative: wrong signature (formally valid)
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData,
            {parse_hex("415502201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b349022"
                       "00a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f3b51")
                       },
            pubkeyVec);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signing);
    }
}

TEST(TransactionCompiler, EOSCompileWithSignatures) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeEOS;
    EOS::Proto::SigningInput input;
    auto chainId = parse_hex("cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f");
    auto refBlock = parse_hex("000067d6f6a7e7799a1f3d487439a679f8cf95f1c986f35c0d2fa320f51a7144");
    auto key = parse_hex("559aead08264d5795d3909718cdd05abd49572e84fe55590eef31a88a08fdffd");

    auto& asset = *input.mutable_asset();
    asset.set_amount(300000);
    asset.set_decimals(4);
    asset.set_symbol("TKN");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_reference_block_id(refBlock.data(), refBlock.size());
    input.set_reference_block_time(1554209118);
    input.set_currency("token");
    input.set_sender("token");
    input.set_recipient("eosio");
    input.set_memo("my second transfer");
    input.set_private_key(key.data(), key.size());
    input.set_private_key_type(EOS::Proto::KeyType::MODERNK1);

    auto txInputData = data(input.SerializeAsString());
    {
        EOS::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(output.json_encoded(), R"({"compression":"none","packed_context_free_data":"","packed_trx":"6e67a35cd6679a1f3d4800000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232330000000080a920cd0000000000ea3055e09304000000000004544b4e00000000126d79207365636f6e64207472616e7366657200","signatures":["SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7"]})");
    }

    /// Step 2: Obtain preimage hash
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0ul);

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = data(preSigningOutput.data_hash());
    EXPECT_EQ(hex(preImageHash), "14fc3299ee3e1113096bf1869dfa14c04a7ffdedd8ebdabf530683e4cfcd726c");

    // Simulate signature, normally obtained from signature server
    const PublicKey publicKey = PrivateKey(key).getPublicKey(TWPublicKeyTypeNIST256p1);
    const auto signature = parse_hex("1f6c4efceb5a6dadab271fd7e2153d97d22690938475b23f017cf9ec29e20d25725e90e541e130daa83c38fc4c933725f05837422c3f4a51f8c1d07208c8fd5e0b");//data("SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7");

    // Verify signature (pubkey & hash & signature)
    // { EXPECT_TRUE(publicKey.verify(signature, preImageHash)); }

    /// Step 3: Compile transaction info
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});

    const auto ExpectedTx = R"({"compression":"none","packed_context_free_data":"","packed_trx":"6e67a35cd6679a1f3d4800000000010000000080a920cd000000572d3ccdcd010000000080a920cd00000000a8ed3232330000000080a920cd0000000000ea3055e09304000000000004544b4e00000000126d79207365636f6e64207472616e7366657200","signatures":["SIG_K1_K9RdLC7DEDWjTfR64GU8BtDHcAjzR1ntcT651JMcfHNTpdsvDrUwfyzF1FkvL9fxEi2UCtGJZ9zYoNbJoMF1fbU64cRiJ7"]})";
    {
        EOS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.json_encoded(), ExpectedTx);
    }

    input.set_private_key_type(EOS::Proto::KeyType::LEGACY);
    {
        EOS::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        EXPECT_EQ(output.error(), Common::Proto::Error_internal);
        EXPECT_TRUE(output.json_encoded().empty());
    }
}

TEST(TransactionCompiler, VergeCompileWithSignatures) {
    const auto coin = TWCoinTypeVerge;
    
    // tx on mainnet
    // https://verge-blockchain.info/tx/21314157b60ddacb842d2a749429c4112724b7a078adb9e77ba502ea2dd7c230

    const int64_t amount = 9999995000000;
    const int64_t fee = 120850;
    const std::string toAddress = "DQZboqURLgrBzBz4Kfbs3yV6fZ3DrNFRjQ";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("DCUWt5ctZcPdPMYPV2o1xK1kqv7jNwxu4h");
    input.set_coin_type(coin);
    input.set_time(1584059579);

    auto txHash0 = parse_hex("ee839754c8e93d620cbec9a1c51e7b69016d00839741b03af2c039852d941212");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(4294967295);
    utxo0->set_amount(20000000000000);

    auto script0 = parse_hex("76a91479471b92b3c94b37544fff430556043d9acd53b188ac");
    utxo0->set_script(script0.data(), script0.size());

    EXPECT_EQ(input.utxo_size(), 1);

    // Plan
    Bitcoin::Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, coin);

    plan.set_amount(amount);
    plan.set_fee(fee);
    plan.set_change(10000004879150);

    // Extend input with accepted plan
    *input.mutable_plan() = plan;

    // Serialize input
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT(txInputData.size(), 0ul);

    /// Step 2: Obtain preimage hashes
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    TW::Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), (int)preImageHashes.size()));

    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].data_hash()),
               "f7498449e2b8d33d4ff00c72b05c820e5262f43360d9f38455dcfd8f6425c9b2");
    
    EXPECT_EQ(hex(preSigningOutput.hash_public_keys()[0].public_key_hash()), "79471b92b3c94b37544fff430556043d9acd53b1");

    auto publicKeyHex = "02b2655122379a375a47e7a204a9dc4572cec5dbe4db4c51fea0c9fa03061fdb0b";
    auto publicKey = PublicKey(parse_hex(publicKeyHex), TWPublicKeyTypeSECP256k1);
    auto preImageHash = preSigningOutput.hash_public_keys()[0].data_hash();
    auto signature = parse_hex("3044022039e18d10ab4793d0564cfa675286d2ffd016b8f936c696fd3b72267b621dcd400220653d4761be6b12261629c4240033a08d9767a5f16851dc91a190c8a8d25ecbe0");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(
        publicKey.verifyAsDER(signature, TW::Data(preImageHash.begin(), preImageHash.end())));

    // Simulate signatures, normally obtained from signature server.
    std::vector<Data> signatureVec;
    std::vector<Data> pubkeyVec;
    signatureVec.push_back(signature);
    pubkeyVec.push_back(publicKey.bytes);

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, signatureVec, pubkeyVec);

    const auto ExpectedTx =
        "01000000bbd46a5e011212942d8539c0f23ab0419783006d01697b1ec5a1c9be0c623de9c8549783ee010000006a473044022039e18d10ab4793d0564cfa675286d2ffd016b8f936c696fd3b72267b621dcd400220653d4761be6b12261629c4240033a08d9767a5f16851dc91a190c8a8d25ecbe0012102b2655122379a375a47e7a204a9dc4572cec5dbe4db4c51fea0c9fa03061fdb0bffffffff02c054264e180900001976a914d50cce1f1449ac5630a0a731cbfcf7d7208a6e7d88ac2e13bd4e180900001976a91450751a6dc46f7068ac3c6350f6a85f7c20fd5e2988ac00000000";
    {
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: not enough signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, pubkeyVec);
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, ZcashCompileWithSignatures) {
    const auto coin = TWCoinTypeZcash;

    // tx on mainnet
    // https://explorer.zcha.in/transactions/387939ff8eb07dd264376eeef2e126394ab139802b1d80e92b21c1a2ae54fe92
    const int64_t amount = 17615;
    const int64_t fee = 10000;
    const std::string toAddress = "t1biXYN8wJahR76SqZTe1LBzTLf3JAsmT93";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_coin_type(TWCoinTypeZcash);

    auto txHash0 = parse_hex("2381825cd9069a200944996257e25b9403ba3e296bbc1dd98b01019cc7028cde");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(27615);

    // real key 1p "m/44'/133'/0'/0/14"
    auto utxoKey0 = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    auto utxoAddr0 = TW::deriveAddress(TWCoinTypeZcash, utxoKey0);
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, TWCoinTypeZcash);
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());
    input.add_private_key(utxoKey0.bytes.data(), utxoKey0.bytes.size());

    auto plan = Zcash::TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = 0;

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // build preimage
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT(txInputData.size(), 0ul);

    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0ul);

    Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    ASSERT_EQ(preSigningOutput.hash_public_keys().size(), 1);

    auto preImageHash = data(preSigningOutput.hash_public_keys()[0].data_hash());
    EXPECT_EQ(hex(preImageHash),
              "1472faba6529ac6d88f87f6ab881e438c3c8a17482b4a82ef13212333868258a");

    // compile
    auto publicKey = utxoKey0.getPublicKey(TWPublicKeyTypeSECP256k1);
    TW::Data signature = parse_hex("3045022100e6e5071811c08d0c2e81cb8682ee36a8c6b645f5c08747acd3e828de2a4d8a9602200b13b36a838c7e8af81f2d6e7e694ede28833a480cfbaaa68a47187655298a7f");
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});
    Bitcoin::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    ASSERT_EQ(hex(signingOutput.encoded()), "0400008085202f8901de8c02c79c01018bd91dbc6b293eba03945be25762994409209a06d95c828123000000006b483045022100e6e5071811c08d0c2e81cb8682ee36a8c6b645f5c08747acd3e828de2a4d8a9602200b13b36a838c7e8af81f2d6e7e694ede28833a480cfbaaa68a47187655298a7f0121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ffffffff01cf440000000000001976a914c3bacb129d85288a3deb5890ca9b711f7f71392688ac00000000000000000000000000000000000000");

    {
        auto result = Zcash::Signer::sign(input);
        ASSERT_EQ(hex(result.encoded()), hex(signingOutput.encoded()));
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, ZenCompileWithSignatures) {
    const auto coin = TWCoinTypeZen;

    const int64_t amount = 200000;
    const std::string toAddress = "znma8BydGx1p7SZ17g5JMMWXqSoRSE7BNdQ";

    auto blockHash = parse_hex("000000000396ef95695b498168964e1733aca9fe47bb4f9b2851dcd0ec0edad0");
    std::reverse(blockHash.begin(), blockHash.end());
    auto blockHeight = 1163482;

    auto sblockHash = parse_hex("0000000002906dc9ef21c60d08cd03d192cba94de66095c63082d8e7e9436d40");
    std::reverse(sblockHash.begin(), sblockHash.end());
    auto sblockHeight = 1163438;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_change_address("zncug4MEDrunR5WgdWfGB1t9Bjp8RCpKxA6");
    input.set_coin_type(coin);
    input.set_lock_time(1163772);

    auto txHash0 = parse_hex("89f799d7aaf17dbc619f5c68aa5a5ae55ceec779f9009203a87359217405f8d8");
    std::reverse(txHash0.begin(), txHash0.end());

    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(txHash0.data(), txHash0.size());
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(4294967294);
    utxo0->set_amount(1249057);

    auto utxoAddr0 = "znj6M9EbCmU7UKN2zgAQ8j1GwUnr4QbZBYt";
    // build utxo scriptPubKey
    // check 89f799d7aaf17dbc619f5c68aa5a5ae55ceec779f9009203a87359217405f8d8,1
    auto script0 = Bitcoin::Script::lockScriptForAddress(utxoAddr0, coin, sblockHash, sblockHeight);
    utxo0->set_script(script0.bytes.data(), script0.bytes.size());

    auto plan = Zen::TransactionBuilder::plan(input);
    ASSERT_EQ(plan.fee, 226);
    plan.preBlockHash = blockHash;
    plan.preBlockHeight = blockHeight;
    plan.fee = 302;
    plan.change = 1249057 - plan.amount - plan.fee;

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // build preimage
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT(txInputData.size(), 0ul);

    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0ul);

    Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    ASSERT_EQ(preSigningOutput.hash_public_keys().size(), 1);

    auto preImageHash = data(preSigningOutput.hash_public_keys()[0].data_hash());
    EXPECT_EQ(hex(preImageHash),
              "882e2e61e740ff3d5889995679bf3dcda1b872e0d93be23c89a4fd4e3837f200");

    // compile
    auto publicKey = PublicKey(parse_hex("02806408d2f6d5095bb73e89f9edbe02fe81853f25c541d33da4422c6916c1d0e1"), TWPublicKeyTypeSECP256k1);
    auto signature = parse_hex("3045022100b27a4d10a4c5e758c4a379ccf7050eae6d8d4dacf5c65894d024de5ab947d4640220194ffccb29c95fe0ae3fb91a40276536494102891c6c5a9aee6063106fa55d30");
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});
    Bitcoin::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    // txid: 0fc555f8e205e66576f760d99270eaa6d60480c0e816209b2058387b65c2a000
    ASSERT_EQ(hex(signingOutput.encoded()), "0100000001d8f80574215973a8039200f979c7ee5ce55a5aaa685c9f61bc7df1aad799f789010000006b483045022100b27a4d10a4c5e758c4a379ccf7050eae6d8d4dacf5c65894d024de5ab947d4640220194ffccb29c95fe0ae3fb91a40276536494102891c6c5a9aee6063106fa55d30012102806408d2f6d5095bb73e89f9edbe02fe81853f25c541d33da4422c6916c1d0e1feffffff02400d0300000000003f76a914e0b858909b6b2c14996658085ed907abd880d32d88ac20d0da0eecd0dc51289b4fbb47fea9ac33174e966881495b6995ef96030000000003dac011b4b3001000000000003f76a91481b1b83b2ae8a4cddd72750dc5252c4bddd4e57e88ac20d0da0eecd0dc51289b4fbb47fea9ac33174e966881495b6995ef96030000000003dac011b4fcc11100");

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, GroestlcoinCompileWithSignatures) {
    const auto coin = TWCoinTypeGroestlcoin;

    Bitcoin::Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(2500);
    input.set_byte_fee(1);
    input.set_to_address("grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne");
    input.set_change_address("31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P");

    auto utxoKey0 = parse_hex("3c3385ddc6fd95ba7282051aeb440bc75820b8c10db5c83c052d7586e3e98e84");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());

    auto utxo0 = input.add_utxo();
    auto utxo0Script = Bitcoin::Script(parse_hex("76a91498af0aaca388a7e1024f505c033626d908e3b54a88ac"));
    utxo0->set_script(utxo0Script.bytes.data(), utxo0Script.bytes.size());
    utxo0->set_amount(5000);
    auto hash0 = parse_hex("9568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f");
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);

    Bitcoin::Proto::TransactionPlan plan;
    {
        // try plan first
        ANY_PLAN(input, plan, coin);
        EXPECT_TRUE(verifyPlan(plan, {5000}, 2500, 221));
    }

    // Supply plan for signing, to match fee of previously-created real TX
    *input.mutable_plan() = plan;
    input.mutable_plan()->set_fee(226);
    input.mutable_plan()->set_change(2274);

    // build preimage
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT(txInputData.size(), 0ul);

    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0ul);

    Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    ASSERT_EQ(preSigningOutput.hash_public_keys().size(), 1);

    auto preImageHash = data(preSigningOutput.hash_public_keys()[0].data_hash());
    EXPECT_EQ(hex(preImageHash),
              "0fb3da786ad1028574f0b40ff1446515eb85cacccff3f3d0459e191b660597b3");

    // compile
    auto publicKey = PrivateKey(utxoKey0).getPublicKey(TWPublicKeyTypeSECP256k1);
    auto signature = parse_hex("304402202163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa3702202bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d19");
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});
    Bitcoin::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    ASSERT_EQ(hex(signingOutput.encoded()),
              "01000000019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f000000"
              "006a47304402202163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa3702"
              "202bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d19012103b85cc59b"
              "67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91ffffffff02c4090000000000"
              "001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09cee20800000000000017a9140055b0c94d"
              "f477ee6b9f75185dfc9aa8ce2e52e48700000000");

    {
        Bitcoin::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        ASSERT_EQ(output.encoded(), signingOutput.encoded());
    }

     { // Negative: not enough signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature}, {publicKey.bytes, publicKey.bytes});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Bitcoin::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, DecredCompileWithSignatures) {
    const auto coin = TWCoinTypeDecred;

    const int64_t utxoValue = 39900000;
    const int64_t amount = 10000000;
    const int64_t fee = 100000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("Dsesp1V6DZDEtcq2behmBVKdYqKMdkh96hL");
    input.set_change_address("DsUoWCAxprdGNtKQqambFbTcSBgH1SHn9Gp");
    input.set_coin_type(coin);

    auto& utxo = *input.add_utxo();

    auto hash = parse_hex("fdbfe9dd703f306794a467f175be5bd9748a7925033ea1cf9889d7cf4dd11550");
    auto script = parse_hex("76a914b75fdec70b2e731795dd123ab40f918bf099fee088ac");
    auto utxoKey = parse_hex("ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764");

    utxo.set_amount(utxoValue);
    utxo.set_script(script.data(), script.size());
    
    auto& outpoint = *utxo.mutable_out_point();
    outpoint.set_hash(hash.data(), hash.size());
    outpoint.set_index(0);

    auto& plan = *input.mutable_plan();
    plan.set_amount(amount);
    plan.set_available_amount(utxoValue);
    plan.set_fee(fee);
    plan.set_change(utxoValue - amount - fee);
    auto& planUtxo = *plan.add_utxos();
    planUtxo = input.utxo(0);

    // build preimage
    const auto txInputData = data(input.SerializeAsString());
    EXPECT_GT(txInputData.size(), 0ul);

    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    ASSERT_GT(preImageHashes.size(), 0ul);

    Bitcoin::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    ASSERT_EQ(preSigningOutput.hash_public_keys().size(), 1);

    auto preImageHash = data(preSigningOutput.hash_public_keys()[0].data_hash());
    EXPECT_EQ(hex(preImageHash),
              "9e4305478d1a69ee5c89a2e234d1cf270798d447d5db983b8fc3c817afddec34");

    // compile
    auto publicKey = PrivateKey(utxoKey).getPublicKey(TWPublicKeyTypeSECP256k1);
    auto signature = parse_hex("304402206ee887c9239e5fff0048674bdfff2a8cfbeec6cd4a3ccebcc12fac44b24cc5ac0220718f7c760818fde18bc5ba8457d43d5a145cc4cf13d2a5557cba9107e9f4558d");
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKey.bytes});
    Decred::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    ASSERT_EQ(hex(signingOutput.encoded()),
              "0100000001fdbfe9dd703f306794a467f175be5bd9748a7925033ea1cf9889d7cf4dd1155000000000000000000002809698000000000000001976a914989b1aecabf1c24e213cc0f2d8a22ffee25dd4e188ac40b6c6010000000000001976a9142a194fc92e27fef9cc2b057bc9060c580cbb484888ac000000000000000001000000000000000000000000ffffffff6a47304402206ee887c9239e5fff0048674bdfff2a8cfbeec6cd4a3ccebcc12fac44b24cc5ac0220718f7c760818fde18bc5ba8457d43d5a145cc4cf13d2a5557cba9107e9f4558d0121026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6");

    {
        input.add_private_key(utxoKey.data(), utxoKey.size());
        Decred::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        ASSERT_EQ(output.encoded(), signingOutput.encoded());
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Decred::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Decred::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
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
    ASSERT_GT(preImageHashes.size(), 0ul);

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashes.data(), int(preImageHashes.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::SigningError::OK);

    auto preImageHash = data(preSigningOutput.data_hash());
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
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKeyData});

    const auto ExpectedTx =
        "f86c0b8504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0"
        "360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07ba053bd9d1376e15a191d844db4"
        "58893b928f3efbfee90c9febf51ab84c97966779";
    {
        EXPECT_EQ(outputData.size(), 183ul);
        Ethereum::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded().size(), 110ul);
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Ethereum::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(txInputData.data(), (int)txInputData.size()));
        auto key = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
        signingInput.set_private_key(key.data(), key.size());

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Ethereum::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
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

TEST(TransactionCompiler, SolanaCompileTransferWithSignatures) {
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
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
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
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedTx =
        "5bWxVCP5fuzkKSGby9hnsLranszQJR2evJGTfBrpDQ4rJceW1WxKNrWqVPBsN2QCAGmE6W7VaYkyWjv39HhGrr1Ne2"
        "QSUuHZdyyn7hK4pxzLPMgPG8fY1XvXdppWMaKMLmhriLkckzGKJMaE3pWBRFBKzigXY28714uUNndb7S9hVakxa59h"
        "rLph39CMgAkcj6b8KYvJEkb1YdYytHSZNGi4kVVTNqiicNgPdf1gmG6qz9zVtnqj9JtaD2efdS8qxsKnvNWSgb8Xxb"
        "T6dwyp7msUUi7d27cYaPTpK";

    EXPECT_EQ(outputData.size(), 296ul);

    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 293ul);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TransactionCompiler, SolanaCompileCreateNonceAccountWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto sender = std::string("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    uint64_t rent = 10000000;
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("mFmK2xFMhzJJaUN5cctfdCizE9dtgcSASSEDh1Yzmat"));
    message.set_nonce_account(nonceAccount);
    message.set_rent(rent);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 2);
    auto signer1 = preSigningOutput.signers(0);
    EXPECT_EQ(signer1, sender);
    auto signer2 = preSigningOutput.signers(1);
    EXPECT_EQ(signer2, nonceAccount);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImageHash),
        "020003050d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c057f6ed937bb447a670"
        "0c9684d2e182b1a6661838a86cca7d0aac18be2e098b2106a7d517192c568ee08a845f73d29788cf035c3145b2"
        "1ab344d8062ea940000006a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000000000"
        "00000000000000000000000000000000000000000000000000000000000b563fd13b46e844f12f54fa8a0e78c4"
        "4d95dbae4953368b7135f1e0de111cb50204020001340000000080969800000000005000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000040301020324060000000d044a62d0a4"
        "dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData = Base58::bitcoin.decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const Data nonceAccountPublicKeyData = Base58::bitcoin.decode(nonceAccount);
    const PublicKey nonceAccountPublicKey =
        PublicKey(nonceAccountPublicKeyData, TWPublicKeyTypeED25519);
    const auto signature = Base58::bitcoin.decode(
        "3dbiGHLsFqnwA1PXx7xmoikzv6v9g9BXvZts2126qyE163BypurkvgbDiF5RmrEZRiT2MG88v6xwyJTkhhDRuFc9");
    const auto nonceAccountSignature = Base58::bitcoin.decode(
        "jFq4PbbEM1fuPbq5CkUYgzs7a21g6rvFkfLJAUUGP5QMKYhHBE6nB1dqtwaJsABgyUvrR8QjT2Ej73cXNz7Vur1");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    EXPECT_TRUE(nonceAccountPublicKey.verify(nonceAccountSignature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    auto outputData = TransactionCompiler::compileWithSignatures(
        coin, inputStrData, {signature, nonceAccountSignature},
        {publicKeyData, nonceAccountPublicKeyData});
    const auto ExpectedTx =
        "3wu6xJSbb2NysVgi7pdfMgwVBT1knAdeCr9NR8EktJLoByzM4s9SMto2PPmrnbRqPtHwnpAKxXkC4vqyWY2dRBgdGG"
        "CC1bep6qN5nSLVzpPYAWUSq5cd4gfYMAVriFYRRNHmYUnEq8vMn4vjiECmZoHrpabBj8HpXGqYBo87sbZa8ZPCxUcB"
        "71hxXiHWZHj2rovx2kr75Uuv1buWXyW6M8uR4UNvQcPPvzVbwBG82RjDYTuancMSAxmrVNR8GLBQNhrCCYrZyte3EW"
        "gEyMQxxfW8T3xNXqnbgdfvFJ3UjRBxXj3hrmv17xEivTjfs81aG2AAi24yiYrk8ep7eQqwDHVSArsrynnwVKVNUcCQ"
        "CnSy7fuiuS7FweFX8DEN1K9BrfecHyWrF15fYzhkmWSs64aH6ZTYHWPv5znhFKYmAuopGwbsBEb2j5p8NS3iJZ2skb"
        "2wi47n1rpLZfoCHWKxNiikkDUJTGQNcSDrGUMfeW5aGubJrCfecPKEo9Wo9kd36iSsxYPYSWNKrz2HTooa1rCRhqjX"
        "D8dyX3bXGV8TK6W2sEgf4JkcDnNoWQLbindcP8XR";
    
    EXPECT_EQ(outputData.size(), 583ul);
    
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 580ul);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        auto nonceAccountKey =
            parse_hex("2a9737aca3cde2dc0b4f3ae3487e3a90000490cb39fbc979da32b974ff5d7490");
        signingInput.set_private_key(key.data(), key.size());
        auto& aMessage = *signingInput.mutable_create_nonce_account();
        aMessage.set_nonce_account_private_key(nonceAccountKey.data(), nonceAccountKey.size());
        aMessage.set_rent(rent);

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TransactionCompiler, SolanaCompileWithdrawNonceAccountWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_withdraw_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto sender = std::string("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto recipient = std::string("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    uint64_t value = 10000000;
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("5ccb7sRth3CP8fghmarFycr6VQX3NcfyDJsMFtmdkdU8"));
    message.set_nonce_account(nonceAccount);
    message.set_recipient(recipient);
    message.set_value(value);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 1);
    auto signer = preSigningOutput.signers(0);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(hex(preImageHash),
              "010003060d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c057f6ed937bb4"
              "47a6700c9684d2e182b1a6661838a86cca7d0aac18be2e098b2124c255a8bc3e8496217a2cd2a1894b9b"
              "9dcace04fcd9c0d599acdaaea40a1b6106a7d517192c568ee08a845f73d29788cf035c3145b21ab344d8"
              "062ea940000006a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a0000000000000000"
              "00000000000000000000000000000000000000000000000000000000448e50d73f42e3163f5926922aad"
              "d2bca6bdd91f97b3eb7b750e2cecfd810f6d01050501020304000c050000008096980000000000");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData = Base58::bitcoin.decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature = Base58::bitcoin.decode(
        "MxbTCAUmBLiESDLK1NiK5ab41mL2SpAPKSbvGdYQQD5eKgAJRdFEJ8MV9HqBhDQHdsS2LG3QMQQVJp51ekGu6KM");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    const Data outputData = TransactionCompiler::compileWithSignatures(
        coin, inputStrData, {signature},
        {publicKeyData});
    const auto ExpectedTx =
        "7gdEdDymvtfPfVgVvCTPzafmZc1Z8Zu4uXgJDLm8KGpLyPHysxFGjtFzimZDmGtNhQCh22Ygv3ZtPZmSbANbafikR3"
        "S1tvujatHW9gMo35jveq7TxwcGoNSqc7tnH85hkEZwnDryVaiKRvtCeH3dgFE9YqPHxiBuZT5eChHJvVNb9iTTdMsJ"
        "XMusRtzeRV45CvrLKUvsAH7SSWHYW6bGow5TbEJie4buuz2rnbeVG5cxaZ6vyG2nJWHNuDPWZJTRi1MFEwHoxst3a5"
        "jQPv9UrG9rNZFCw4uZizVcG6HEqHWgQBu8gVpYpzFCX5SrhjGPZpbK3YmHhUEMEpJx3Fn7jX7Kt4t3hhhrieXppoqK"
        "NuqjeNVjfEf3Q8dJRfuVMLdXYbmitCVTPQzYKWBR6ERqWLYoAVqjoAS2pRUw1nrqi1HR";
    
    EXPECT_EQ(outputData.size(), 431ul);
    
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 428ul);
    }
    
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TransactionCompiler, NEOCompileWithSignatures) {
    const auto coin = TWCoinTypeNEO;
    /// Step 1: Prepare transaction input (protobuf)
    const std::string NEO_ASSET_ID =
        "9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5";
    const std::string GAS_ASSET_ID =
        "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60";

    TW::NEO::Proto::SigningInput input;
    auto privateKey =
        PrivateKey(parse_hex("F18B2F726000E86B4950EBEA7BFF151F69635951BC4A31C44F28EE6AF7AEC128"));
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

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));

    auto expectedTx =
        "800000019c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de0100029b7cffdaa674"
        "beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000ea610aa6db39bd8c8556c9"
        "569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500fcbbc4"
        "14000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac0141405046619c8e20e1fdeec92ce95f3019f6e7cc"
        "057294eb16b2d5e55c105bf32eb27e1fc01c1858576228f1fef8c0945a8ad69688e52a4ed19f5b85f5eff7e961"
        "d7232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac";

    /// Step 3: Compile transaction info
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

TEST(TransactionCompiler, NEONep5TokenCompileWithSignatures) {
    // tx on mainnet
    // https://neoscan.io/transaction/6368FC6127DD7FAA3B7548CA97162D5BE18D4B2FA0046A79853E5864318E19B8
    
    const auto coin = TWCoinTypeNEO;
    /// Step 1: Prepare transaction input (protobuf)
    const std::string ASSET_ID =
        "f56c89be8bfcdec617e2402b5c3fd5b6d71b820d";
    const std::string GAS_ASSET_ID =
        "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60";

    TW::NEO::Proto::SigningInput input;
    auto publicKey = PublicKey(parse_hex("030ab39b99d8675cd9bd90aaec37cba964297cc817078d33e508ab11f1d245c068"), ::publicKeyType(coin));
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
    auto inputData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputData);

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

    auto expectedTx =
        "d101510480778e0614f88235a26e55cce0747ee827f39fd8167849672b14235a717ed7ed18a43de47499c3d05b8d4a4bcf3a53c1087472616e7366657267f56c89be8bfcdec617e2402b5c3fd5b6d71b820df166000000000000000002f008f15508a6ea4e15e820235a717ed7ed18a43de47499c3d05b8d4a4bcf3a014c976fdfbc5abe4270c5e300e2dd2d0d4917746bcbe72fa212d997507bee31f2000001e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60b976690000000000235a717ed7ed18a43de47499c3d05b8d4a4bcf3a0141408b707d23f84d39ddaad7da100e2d8b657ef6c0858c6c09edc029f441f28e49ff6af994ba7ad180f90e12dd9d7828f8f28785ae5079ed9a52bb5ddd3bcce1b1892321030ab39b99d8675cd9bd90aaec37cba964297cc817078d33e508ab11f1d245c068ac";
    
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKeyData});

    {
        NEO::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }
}

TEST(TransactionCompiler, NEOInvocationTransactionCompileWithSignatures) {
    // tx on mainnet
    // https://neoscan.io/transaction/73f540f5ce6fd4f363262e4b168d411f5443c694f3c53beee36fc03861c00356
    
    const auto coin = TWCoinTypeNEO;
    /// Step 1: Prepare transaction input (protobuf)
    auto ASSET_ID = parse_hex("f46719e2d16bf50cddcef9d4bbfece901f73cbb6");
    std::reverse(ASSET_ID.begin(), ASSET_ID.end());

    const std::string GAS_ASSET_ID =
        "e72d286979ee6cb1b7e65dfddfb2e384100b8d148e7758de42e4168b71792c60";

    TW::NEO::Proto::SigningInput input;
    auto publicKey = PublicKey(parse_hex("02337b2d3982d71aa234a112cd8507260f480994d20129921f5a95c77f8bbe1bb3"), ::publicKeyType(coin));

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
    auto inputData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputData);

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
        TransactionCompiler::compileWithSignatures(coin, inputData, {signature}, {publicKeyData});

    NEO::Proto::SigningOutput output;
    ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

    auto expectedTx =
        "d101500500d6117e03144b721e06b50cc74e68b417716e3b099fb99757a8145872d3dd8741af4c8d5a94f8a1bfff5c617be01b53c1087472616e7366657267b6cb731f90cefebbd4f9cedd0cf56bd1e21967f4000000000000000003205872d3dd8741af4c8d5a94f8a1bfff5c617be01bf11d46726f6d204e656f4c696e652061742031363539303030373533343031fe224e55577138626836486363746e5357346167384a4a4b453734333841637374554d5400000141400c22129691f4438adf0ff178acc4811dee1c625df0f65909791e2c0f563cd88f7967f0ccbb6b60609e5225fb7b2873d510fe42c43c2741d90ca002afb4861d5c232102337b2d3982d71aa234a112cd8507260f480994d20129921f5a95c77f8bbe1bb3ac";
    EXPECT_EQ(hex(output.encoded()), expectedTx);
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
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "1e8786a0162630b2393e0f6c51f16a2d7860715023cb19bf25cad14490b1f8f3");

    auto signature = parse_hex("5042574491827aaccbce1e2964c05098caba06194beb35e595aabfec9f788516a83"
                               "3f755f18144f4a2eedb3123d180f44e7c16037d00857c5c5b7033ebac2c01");

    const auto tx = "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/"
                    "DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAEAAAANSGVsbG8sIHdvcmxkIQAAAAAAAAEAAAAAAAAA"
                    "AQAAAADFgLYxeg6zm/"
                    "f81Po8Gf2rS4m7q79hCV7kUFr27O16rgAAAAAAAAAAAJiWgAAAAAAAAAABGd9cogAAAEBQQldEkYJ6"
                    "rMvOHilkwFCYyroGGUvrNeWVqr/sn3iFFqgz91XxgUT0ou7bMSPRgPROfBYDfQCFfFxbcDPrrCwB";
    
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {});

    {
        TW::Stellar::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.signature(), tx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Stellar::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Stellar::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.signature(), tx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {});
        Stellar::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.signature().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
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
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(hex(preImage),
              "0200f885885d00008c01170100012c177a01a7afbe98e094007b99476534fb7926b701000100201d9a00"
              "000000000000000000000000000000000000000000000000000000000800000000000000000001170100"
              "01f05e7878971f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000"
              "0000000000000000000000000000000000000000000000");
    auto preImageHash = preSigningOutput.data_hash();
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
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedEncoded = parse_hex(
        "0200f885885d00008c01170100012c177a01a7afbe98e094007b99476534fb7926b701000100201d9a00000000"
        "00000000000000000000000000000000000000000000000000080000000000000000000117010001f05e787897"
        "1f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000000000000000000000"
        "00000000000000000000000000006a21033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0a"
        "ff0ee045473045022100a5234269eab6fe8a1510dd0cb36070a03464b48856e1ef2681dbb79a5ec656f9022029"
        "61ac01d401a6f849acc958c6c9653f49282f5a0916df036ea8766918bac195");
    const auto ExpectedTx = std::string(ExpectedEncoded.begin(), ExpectedEncoded.end());
    
    EXPECT_EQ(outputData.size(), 259ul);
    
    {
        TW::NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 256ul);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::NULS::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        TW::NULS::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKeyData});
        NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
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
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    EXPECT_EQ(hex(preSigningOutput.data_hash()),
              "2dc419e9919e65f129453419dc72a6bee99b2281dfddf754807a5c212ae35678");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature =
        parse_hex("5190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134ccdef0"
                  "53ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preSigningOutput.data_hash())));

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});

    auto expectedTx = "02f887c78085e8d4a51000f863f861942e833968e5bb786ae419c4d13189fb081cc43babc70a"
                      "85e8d4a5101401b8415190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb"
                      "7fff267cb8fe9134ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501d9d8"
                      "949f1233798e905e173560071255140b4a8abd3ec6c20a14";

    {
        Theta::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        Theta::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(pkFrom.bytes.data(), pkFrom.bytes.size());

        Theta::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKeyData});
        Theta::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }
}

TEST(TransactionCompiler, TezosCompileWithSignatures) {
    const auto coin = TWCoinTypeTezos;

    /// Step 1: Prepare transaction input (protobuf)
    auto privateKey =
        PrivateKey(parse_hex("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));
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
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "12e4f8b17ad3b316a5a56960db76c7d6505dbf2fff66106be75c8d6753daac0e");

    auto signature = parse_hex("0217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a10db70c987"
                               "74cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05");

    const auto tx =
        "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35f"
        "cc8c90dfa3b0b95721f80992f001f44e810200311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10"
        "906f511cff956c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80993f001f44e810201000081faa75f74"
        "1ef614b0e35fcc8c90dfa3b0b95721000217034271b815e5f0c0a881342838ce49d7b48cdf507c72b1568c69a1"
        "0db70c98774cdad1a74df760763e25f760ff13afcbbf3a1f2c833a0beeb9576a579c05";

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {});

    {
        TW::Tezos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), tx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Tezos::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Tezos::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), tx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {});
        Tezos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
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
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "eea6e680f3ea51a7f667e9a801d0bfadf66e03d41ed54975b3c6006351461b32");
    auto signature = parse_hex("969a83332186ee9755e4839325525806e189a3d2d2bb4b4760e94443e97e1c4f22d"
                               "eeef0059a8e9713100eda6e19144da7e8a0ef7e539b20708ba1d8d021bd01");

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKey});

    const auto tx = "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341"
                    "538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901d"
                    "f296be6adc4cc4df34d040efa2435224b6986910e630c2fef60100000003010000000000000000"
                    "0000000000000000969a83332186ee9755e4839325525806e189a3d2d2bb4b4760e94443e97e1c"
                    "4f22deeef0059a8e9713100eda6e19144da7e8a0ef7e539b20708ba1d8d021bd01";

    {
        TW::NEAR::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.signed_transaction()), tx);
    }
    
    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        TW::NEAR::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKeyBytes.data(), 32);

        TW::NEAR::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.signed_transaction()), tx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey});
        NEAR::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.signed_transaction().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
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
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "451ed1e542da2422ed152bff6f30c95e2a8ee2153f4d36f15c45914fa2d2e9f1");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = publicKey.bytes;
    const auto signature =
        parse_hex("1e5e2de66512658e9317fa56766678166abcf492d020863935723db2030f736710e13437cef0981f"
                  "cc376eae45349759508767d407b6c9963712910ada2c3606");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preSigningOutput.data_hash())));

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});

    const auto tx =
        "7b2274696d65223a2231363437343231353739393031353535303030222c2265787069726174696f6e223a2231"
        "363437343231383739393031353535303030222c226761735f726174696f223a312c226761735f6c696d697422"
        "3a3130303030302c22636861696e5f6964223a313032332c22616374696f6e73223a5b7b22636f6e7472616374"
        "223a22746f6b656e2e696f7374222c22616374696f6e5f6e616d65223a227472616e73666572222c2264617461"
        "223a225b5c22494f53545c222c5c226173746173746f6f6f5c222c5c22746573745f696f737465645c222c5c22"
        "302e3030315c222c5c22746573745c225d227d5d2c22616d6f756e745f6c696d6974223a5b7b22746f6b656e22"
        "3a222a222c2276616c7565223a22756e6c696d69746564227d5d2c227075626c6973686572223a226173746173"
        "746f6f6f222c227075626c69736865725f73696773223a5b7b22616c676f726974686d223a322c227369676e61"
        "74757265223a22486c3474356d55535a593654462f7057646d5a34466d7138394a4c51494959354e5849397367"
        "4d5063326351345451337a76435948387733627135464e4a645a5549646e31416532795a59334570454b326977"
        "3242673d3d222c227075626c69635f6b6579223a2234687a496d2b6d383234536f663866645641474545332b64"
        "667931554c7a69786e6f4c5047694a565879383d227d5d7d";

    {
        IOST::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), tx);
    }
    
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::IOST::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));

        TW::IOST::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), tx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        IOST::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: invalid signatures
        const auto invalidSignature =
            parse_hex("fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a09");
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {invalidSignature}, {publicKey.bytes});
        IOST::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_internal);
        EXPECT_EQ(output.error_message(), "Invalid signature/hash/publickey combination");
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        IOST::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, TronCompileWithSignatures) {
    const auto privateKey =
        PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
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
    const auto txTrieRoot =
        parse_hex("845ab51bf63c2c21ee71a4dc0ac3781619f07a7cd05e1e0bd8ba828979332ffa");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash =
        parse_hex("00000000003cb800a7e69e9144e3d16f0cf33f33a95c7ce274097822c67243c1");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3979265);
    const auto witnessAddress = parse_hex("41b487cdc02de90f15ac89a68c82f44cbfe3d915ea");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb");
    auto signature = parse_hex("77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603"
                               "a183aa12124adeee7991bf55acc8e488a6ca04fb393b1a8ac16610eeafdfc00");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preSigningOutput.data_hash())));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKey.bytes});

    const auto tx =
        "{\"raw_data\":{\"contract\":[{\"parameter\":{\"type_url\":\"type.googleapis.com/"
        "protocol.TransferAssetContract\",\"value\":{\"amount\":4,\"asset_name\":"
        "\"31303030393539\",\"owner_address\":\"415cd0fb0ab3ce40f3051414c604b27756e69e43db\",\"to_"
        "address\":\"41521ea197907927725ef36d70f25f850d1659c7c7\"}},\"type\":"
        "\"TransferAssetContract\"}],\"expiration\":1541926116000,\"ref_block_bytes\":\"b801\","
        "\"ref_block_hash\":\"0e2bc08d550f5f58\",\"timestamp\":1539295479000},\"signature\":["
        "\"77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603a183aa12124adeee7991b"
        "f55acc8e488a6ca04fb393b1a8ac16610eeafdfc00\"],\"txID\":"
        "\"546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb\"}";

    {
        TW::Tron::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json(), tx);
    }
    
    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        TW::Tron::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), 32);

        TW::Tron::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.json(), tx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        Tron::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.json().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
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
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(hex(preImage),
              "5354580012000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92"
              "cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b681148400b6b6d08d5d495653d73e"
              "da6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d");
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "8624dbbd5da9ccc8f7a50faf8af8709837db72f51a50cac15a6cd28ce6107b3d");
    // Simulate signature, normally obtained from signature server
    const auto signature = privateKey.sign(parse_hex("8624dbbd5da9ccc8f7a50faf8af8709837db72f51a50cac15a6cd28ce6107b3d"), TWCurveSECP256k1);
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKey.bytes});
    EXPECT_EQ(outputData.size(), 185ul);

    const auto ExpectedTx = std::string(
        "12000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92cefb3a45"
        "37b3edb0b6044c04af27c01583c577823ecc69a9a21119b6744630440220067f20b3eebfc7107dd0bcc7"
        "2337a236ac3be042c0469f2341d76694a17d4bb9022048393d7ee7dcb729783b33f5038939ddce1bb833"
        "7e66d752974626854556bbb681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aec"
        "f29090ac428a9c43f230a829220d");

    {
        TW::Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 182ul);
        ASSERT_EQ(output.error(), TW::Common::Proto::SigningError::OK);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Ripple::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(key.data(), key.size());

        TW::Ripple::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: not enough signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        Ripple::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TWTransactionCompiler, CosmosCompileWithSignatures) {
    const auto coin = TWCoinTypeCosmos;
    TW::Cosmos::Proto::SigningInput input;

    PrivateKey privateKey =
        PrivateKey(parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af"));
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
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    auto preImage = preSigningOutput.data();
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(
        hex(preImage),
        "0a92010a8f010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e64126f0a2d636f736d6f"
        "73316d6b793639636e38656b74777930383435766563397570736470686b7478743033676b776c78122d636f73"
        "6d6f733138733068646e736c6c6763636c7765753961796d77346e676b7472326b30726b7967647a64701a0f0a"
        "057561746f6d120634303030303012650a4e0a460a1f2f636f736d6f732e63727970746f2e736563703235366b"
        "312e5075624b657912230a2102ecef5ce437a302c67f95468de4b31f36e911f467d7e6a52b41c1e13e1d563649"
        "12040a02080112130a0d0a057561746f6d12043130303010c09a0c1a0b636f736d6f736875622d342083ab21");
    EXPECT_EQ(hex(preImageHash),
              "fa7990e1814c900efaedf1bdbedba22c22336675befe0ae39974130fc204f3de");

    auto expectedTx = 
            "7b226d6f6465223a2242524f4144434153545f4d4f44455f424c4f434b222c2274785f6279746573223a224370"
            "4942436f3842436877765932397a6257397a4c6d4a68626d7375646a46695a5852684d53354e633264545a5735"
            "6b456d384b4c574e7663323176637a467461336b324f574e754f475672644864354d4467304e585a6c597a6c31"
            "63484e6b63476872644868304d444e6e61336473654249745932397a6257397a4d54687a4d47686b626e4e7362"
            "47646a593278335a58553559586c74647a52755a327430636a4a724d484a726557646b656d52774767384b4258"
            "566864473974456759304d4441774d4441535a51704f436b594b4879396a62334e7462334d7559334a35634852"
            "764c6e4e6c593341794e545a724d53355164574a4c5a586b5349776f6841757a76584f51336f774c4766355647"
            "6a65537a487a62704566526e312b616c4b30484234543464566a5a4a4567514b4167674245684d4b44516f4664"
            "57463062323053424445774d444151774a6f4d476b437676564536643239503330634f392f6c6e587947756e57"
            "4d50784e5931324e75714463436e466b4e4d304834435551646c314763392b6f67494a62726f356e797a5a7a6c"
            "7639726c322f47735a6f782f4a586f4358227d";
    Data signature;

    {
        TW::Cosmos::Proto::SigningOutput output;
        ANY_SIGN(input, coin);

        assertJSONEqual(
            output.serialized(),
            "{\"tx_bytes\": "
            "\"CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZl"
            "Yzl1cHNkcGhrdHh0MDNna3dseBItY29zbW9zMThzMGhkbnNsbGdjY2x3ZXU5YXltdzRuZ2t0cjJrMHJreWdkemRwGg"
            "8KBXVhdG9tEgY0MDAwMDASZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAuzvXOQ3owLG"
            "f5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJEgQKAggBEhMKDQoFdWF0b20SBDEwMDAQwJoMGkCvvVE6d29P30cO9/"
            "lnXyGunWMPxNY12NuqDcCnFkNM0H4CUQdl1Gc9+ogIJbro5nyzZzlv9rl2/GsZox/JXoCX\", \"mode\": "
            "\"BROADCAST_MODE_BLOCK\"}");

        signature = data(output.signature());
        EXPECT_EQ(hex(signature),
                "afbd513a776f4fdf470ef7f9675f21ae9d630fc4d635d8dbaa0dc0a716434cd07e02510765d4673dfa88"
                "0825bae8e67cb367396ff6b976fc6b19a31fc95e8097");
        
        ASSERT_TRUE(publicKey.verify(signature, data(preImageHash.data())));
        EXPECT_EQ(hex(output.serialized()), expectedTx);
    }

    {
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, protoInputData, {signature}, {publicKey.bytes});
        Cosmos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        
        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(hex(output.serialized()), expectedTx);
    }

    { // Negative: not enough signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, protoInputData, {signature, signature}, {publicKey.bytes});
        Cosmos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.serialized().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, protoInputData, {}, {});
        Cosmos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.serialized().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    /// Step 3: Obtain json preimage hash
    input.set_signing_mode(TW::Cosmos::Proto::JSON);
    auto jsonInputString = input.SerializeAsString();
    auto jsonInputData = TW::Data(jsonInputString.begin(), jsonInputString.end());

    const auto jsonPreImageHashData = TransactionCompiler::preImageHashes(coin, jsonInputData);
    auto jsonPreSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(jsonPreSigningOutput.ParseFromArray(jsonPreImageHashData.data(),
                                                    (int)jsonPreImageHashData.size()));
    ASSERT_EQ(jsonPreSigningOutput.error(), 0);
    auto jsonPreImage = jsonPreSigningOutput.data();
    auto jsonPreImageHash = jsonPreSigningOutput.data_hash();
    EXPECT_EQ(hex(jsonPreImage),
              "7b226163636f756e745f6e756d626572223a22353436313739222c22636861696e5f6964223a22636f73"
              "6d6f736875622d34222c22666565223a7b22616d6f756e74223a5b7b22616d6f756e74223a2231303030"
              "222c2264656e6f6d223a227561746f6d227d5d2c22676173223a22323030303030227d2c226d656d6f22"
              "3a22222c226d736773223a5b7b2274797065223a22636f736d6f732d73646b2f4d736753656e64222c22"
              "76616c7565223a7b22616d6f756e74223a5b7b22616d6f756e74223a22343030303030222c2264656e6f"
              "6d223a227561746f6d227d5d2c2266726f6d5f61646472657373223a22636f736d6f73316d6b79363963"
              "6e38656b74777930383435766563397570736470686b7478743033676b776c78222c22746f5f61646472"
              "657373223a22636f736d6f733138733068646e736c6c6763636c7765753961796d77346e676b7472326b"
              "30726b7967647a6470227d7d5d2c2273657175656e6365223a2230227d");
    EXPECT_EQ(hex(jsonPreImageHash),
              "0a31f6cd50f1a5c514929ba68a977e222a7df2dc11e8470e93118cc3545e6b37");
    
    signature = Base64::decode("tTyOrburrHEHa14qiw78e9StoZyyGmoku98IxYrWCmtN8Qo5mTeKa0BKKDfgG4LmmNdwYcrXtqQQ7F4dL3c26g==");
    {
        auto result = TW::anySignJSON(coin, jsonPreImage, privateKey.bytes);
        EXPECT_EQ(result, "{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[],\"gas\":\"0\"},\"memo\":\"\",\"msg\":[],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJ\"},\"signature\":\"tTyOrburrHEHa14qiw78e9StoZyyGmoku98IxYrWCmtN8Qo5mTeKa0BKKDfgG4LmmNdwYcrXtqQQ7F4dL3c26g==\"}]}}");
    }

    {   // JSON
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, jsonInputData, {signature}, {publicKey.bytes});
        Cosmos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        
        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(hex(output.serialized()), "7b226d6f6465223a22626c6f636b222c227478223a7b22666565223a7b22616d6f756e74223a5b7b22616d6f756e74223a2231303030222c2264656e6f6d223a227561746f6d227d5d2c22676173223a22323030303030227d2c226d656d6f223a22222c226d7367223a5b7b2274797065223a22636f736d6f732d73646b2f4d736753656e64222c2276616c7565223a7b22616d6f756e74223a5b7b22616d6f756e74223a22343030303030222c2264656e6f6d223a227561746f6d227d5d2c2266726f6d5f61646472657373223a22636f736d6f73316d6b793639636e38656b74777930383435766563397570736470686b7478743033676b776c78222c22746f5f61646472657373223a22636f736d6f733138733068646e736c6c6763636c7765753961796d77346e676b7472326b30726b7967647a6470227d7d5d2c227369676e617475726573223a5b7b227075625f6b6579223a7b2274797065223a2274656e6465726d696e742f5075624b6579536563703235366b31222c2276616c7565223a2241757a76584f51336f774c47663556476a65537a487a62704566526e312b616c4b30484234543464566a5a4a227d2c227369676e6174757265223a227454794f72627572724845486131347169773738653953746f5a7979476d6f6b7539384978597257436d744e38516f356d54654b6130424b4b44666747344c6d6d4e64775963725874715151374634644c33633236673d3d227d5d7d7d");
    }
}

TEST(TWTransactionCompiler, VechainCompileWithSignatures) {
    const auto coin = TWCoinTypeVeChain;

    /// Step 1: Prepare transaction input (protobuf)
    TW::VeChain::Proto::SigningInput input;
    PrivateKey privateKey =
        PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto publicKey = privateKey.getPublicKey(publicKeyType(coin));

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
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImage = preSigningOutput.data();
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImage),
              "e7010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0");
    EXPECT_EQ(hex(preImageHash),
              "a1b8ef3af3d8c74e97ac6cd732916a8f4c38c0905c8b70d2fa598edf1f62ea04");

    TW::Data signature;
    /// Step 3: Sign
    {
        TW::VeChain::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        ASSERT_EQ(hex(output.encoded()),
                "f86a010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0b8"
                "41bf8edf9600e645b5abd677cb52f585e7f655d1361075d511b37f707a9f31da6702d28739933b264527"
                "a1d05b046f5b74044b88c30c3f5a09d616bd7a4af4901601");

        signature = data(output.signature());
        /// Step 4: Verify signature
        ASSERT_TRUE(publicKey.verify(signature, data(preImageHash.data())));
    }

    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, dataInput, {signature}, {publicKey.bytes});
    
    TW::VeChain::Proto::SigningOutput output1;
    ASSERT_TRUE(output1.ParseFromArray(outputData.data(), (int)outputData.size()));
    ASSERT_EQ(hex(output1.encoded()),
              "f86a010101dcdb943535353535353535353535353535353535353535843130303080808252088001c0b8"
              "41bf8edf9600e645b5abd677cb52f585e7f655d1361075d511b37f707a9f31da6702d28739933b264527"
              "a1d05b046f5b74044b88c30c3f5a09d616bd7a4af4901601");

    { // Negative: more than one signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, dataInput, {signature, signature}, {publicKey.bytes});
        VeChain::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }
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
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
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

TEST(TransactionCompiler, OasisCompileWithSignatures) {
    const auto coin = TWCoinTypeOasis;
    /// Step 1: Prepare transaction input (protobuf)
    auto privateKey = PrivateKey(parse_hex("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0"));
    auto publicKey = privateKey.getPublicKey(::publicKeyType(coin));

    auto input = TW::Oasis::Proto::SigningInput();
    auto& transfer = *input.mutable_transfer();
    transfer.set_gas_price(0);
    transfer.set_gas_amount("0");
    transfer.set_nonce(0);
    transfer.set_to("oasis1qrrnesqpgc6rfy2m50eew5d7klqfqk69avhv4ak5");
    transfer.set_amount("10000000");
    transfer.set_context("oasis-core/consensus: tx for chain a245619497e580dd3bc1aa3256c07f68b8dcc13f92da115eadc3b231b083d3c4");

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);

    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash), "373976e01fa0634a40ce8898a869f1056d862e3a0f26d8ae22ebeb5fdbcde9b3");

    auto signature = parse_hex("6e51c18c9b2015c9b49414b3307336597f51ff331873d214ce2db81c9651a34d99529ccaa294a39ccd01c6b0bc2c2239d87c624e5ba4840cf99ac8f9283e240c");

    /// Step 3: Compile transaction info
    auto outputData = TransactionCompiler::compileWithSignatures(coin, inputStrData, { signature }, {publicKey.bytes});
    const auto tx = "a2697369676e6174757265a2697369676e617475726558406e51c18c9b2015c9b49414b3307336597f51ff331873d214ce2db81c9651a34d99529ccaa294a39ccd01c6b0bc2c2239d87c624e5ba4840cf99ac8f9283e240c6a7075626c69635f6b6579582093d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b73756e747275737465645f7261775f76616c7565585ea463666565a2636761730066616d6f756e74410064626f6479a262746f5500c73cc001463434915ba3f39751beb7c0905b45eb66616d6f756e744400989680656e6f6e636500666d6574686f64707374616b696e672e5472616e73666572";
    
    {
        TW::Oasis::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), tx);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        TW::Oasis::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Oasis::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), tx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        Oasis::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        Oasis::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, PolkadotCompileWithSignatures) {
     /// Prepare transaction input (protobuf)
    const auto coin = TWCoinTypePolkadot;
    auto input = Polkadot::Proto::SigningInput();
    auto block_hash = parse_hex("40cee3c3b7f8422f4c512e9ebebdeeff1c28e81cc678ee4864d945d641e05f9b");
    auto genesis_hash = parse_hex("91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3");
    input.set_block_hash(block_hash.data(), block_hash.size());
    input.set_genesis_hash(genesis_hash.data(), genesis_hash.size());
    input.set_nonce(0);
    input.set_spec_version(25);
    input.set_transaction_version(5);
    input.set_network(Polkadot::Proto::POLKADOT);

    auto era = input.mutable_era();
    era->set_block_number(5898150);
    era->set_period(10000); 

    auto call = input.mutable_balance_call();
    auto tx = call->mutable_transfer();
    auto value = parse_hex("210fdc0c00");
    tx->set_to_address("15JWiQUmczAFU3hrZrD2gDyuJdL2BbFaX9yngivb1UWiBJWA");
    tx->set_value(value.data(), value.size());

    auto txInputData = data(input.SerializeAsString());
    const auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    auto preOutput = TxCompiler::Proto::PreSigningOutput();
    preOutput.ParseFromArray(preImageHashes.data(), (int)preImageHashes.size());
    auto preImageHash = preOutput.data_hash();

    {
        EXPECT_EQ(hex(preImageHash), "0500be4c21aa92dcba057e9b719ce1de970f774f064c09b13a3ea3009affb8cb5ec707000cdc0f219dfe0000190000000500000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c340cee3c3b7f8422f4c512e9ebebdeeff1c28e81cc678ee4864d945d641e05f9b");
    }

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = parse_hex("d84accbb64934815506288fafbfc7d275e64aa4e3cd9c5392db6e83b13256bf3");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature = parse_hex("fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a09");

    // Verify signature (pubkey & hash & signature)
    {
        EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    }

    const auto expectedTx = "390284d84accbb64934815506288fafbfc7d275e64aa4e3cd9c5392db6e83b13256bf300fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a099dfe00000500be4c21aa92dcba057e9b719ce1de970f774f064c09b13a3ea3009affb8cb5ec707000cdc0f21";

    {
        /// Compile transaction info
        const Data outputData = TransactionCompiler::compileWithSignatures(coin, txInputData, {signature}, {publicKeyData});
        auto output = Ontology::Proto::SigningOutput();
        output.ParseFromArray(outputData.data(), (int)outputData.size());
        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {signature, signature}, {publicKey.bytes});
        Ontology::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, txInputData, {}, {});
        Ontology::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, NULSTokenCompileWithSignatures) {
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
    auto nonce = std::string("0000000000000000");
    auto asset_nonce = std::string("0000000000000000");
    input.set_from(from);
    input.set_to(to);
    input.set_amount(amountStr);
    input.set_chain_id(chainId);
    input.set_idassets_id(idassetsId);
    input.set_nonce(nonce.data(), nonce.size());
    input.set_asset_nonce(asset_nonce.data(), asset_nonce.size());
    input.set_balance(balanceStr);
    input.set_timestamp((uint32_t)1569228280);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImage),
        "0200f885885d0000d202170100012c177a01a7afbe98e094007b99476534fb7926b70900010080969800000000"
        "0000000000000000000000000000000000000000000000000008000000000000000000170100012c177a01a7af"
        "be98e094007b99476534fb7926b701000100a08601000000000000000000000000000000000000000000000000"
        "0000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120900010080"
        "969800000000000000000000000000000000000000000000000000000000000000000000000000");
    auto preImageHash = preSigningOutput.data_hash();
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
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
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
    EXPECT_EQ(outputData.size(), 328ul);
    
    {
        TW::NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 325ul);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::NULS::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        TW::NULS::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TransactionCompiler, SolanaCompileCreateTokenAccountAndTransferWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_and_transfer_token_transaction();
    auto sender = std::string("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = std::string("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = std::string("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientMainAddress = std::string("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    auto recipientTokenAddress = std::string("BwTAyrCEdkjNyGSGVNSSGh6phn8KQaLN638Evj7PVQfJ");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("AfzzEC8NVXoxKoHdjXLDVzqwqvvZmgPuqyJqjuHiPY1D"));
    message.set_recipient_main_address(recipientMainAddress);
    message.set_token_mint_address(token);
    message.set_recipient_token_address(recipientTokenAddress);
    message.set_sender_token_address(senderTokenAddress);
    message.set_amount(amount);
    message.set_decimals(decimals);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 1);
    auto signer = preSigningOutput.signers(0);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImageHash),
        "0100060994c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685aa287d36838b4ef65c2"
        "c460d1a52498453c259cd6a35ca91064aaead28187ca69485a24ffb4070461bb6d7f1c8b758c6b2dc90029d551"
        "b5fd4eacd82d65e302202544558bb05c2698f88852a7925c5c0ee5ea8711ddb3fe1262150283eee811633b442c"
        "b3912157f13a933d0134282d032b5ffecd01a2dbf1b7790608df002ea700000000000000000000000000000000"
        "0000000000000000000000000000000006ddf6e1d765a193d9cbe146ceeb79ac1cb485ed5f5b37913a8cf5857e"
        "ff00a906a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a000000008c97258f4e2489f1bb3d"
        "1029148e0d830b5a1399daff1084048e7bd8dbe9f8598fb6d19edbbae20ebbc767fba1da4d4b40a4b97479fe52"
        "6a82325cba7cee506802080700010304050607000604020401000a0ca00f00000000000006");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData = Base58::bitcoin.decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature = Base58::bitcoin.decode(
        "pL1m11UEDWn3jMkrNMqLeGwNpKzmhQzJiYaCocgPy7vXKA1tnvEjJbuVq9hTeM9kqMAmxhRpwRY157jDgkRdUZw");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedTx =
        "2qkvFTcMk9kPaHtd7idJ1gJc4zTkuYDUJsC67kXvHjv3zwEyUx92QyhhSeBjL6h3Zaisj2nvTWid2UD1N9hbg9Ty7v"
        "SHLc7mcFVvy3yJmN9tz99iLKsf15rEeKUk3crXWLtKZEpcXJurN7vrxKwjQJnVob2RjyxwVfho1oNZ72BHvqToRM1W"
        "2KbcYhxK4d9zB4QY5tR2dzgCHWqAjf9Yov3y9mPBYCQBtw2GewrVMDbg5TK81E9BaWer3BcEafc3NCnRfcFEp7ZUXs"
        "GAgJYx32uUoJPP8ByTqBsp2JWgHyZhoz1WUUYRqWKZthzotErVetjSik4h5GcXk9Nb6kzqEf4nKEZ22eyrP5dv3eZM"
        "uGUUpMYUT9uF16T72s4TTwqiWDPFkidD33tACx74JKGoDraHEvEeAPrv6iUmC675kMuAV4EtVspVc5SnKXgRWRxb4d"
        "cH3k7K4ckjSxYZwg8UhTXUgPxA936jBr2HeQuPLmNVn2muA1HfL2DnyrobUP9vHpbL3HHgM2fckeXy8LAcjnoE9TTa"
        "AKX32wo5xoMj9wJmmtcU6YbXN4KgZ";
    EXPECT_EQ(outputData.size(), 572ul);
    
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 569ul);
    }
    
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = Base58::bitcoin.decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TransactionCompiler, SolanaCompileAdvanceNonceAccountWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto sender = std::string("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_advance_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("4KQLRUfd7GEVXxAeDqwtuGTdwKd9zMfPycyAG3wJsdck"));
    message.set_nonce_account(nonceAccount);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 1);
    auto signer = preSigningOutput.signers(0);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImageHash),
        "0100020494c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685a57f6ed937bb447a670"
        "0c9684d2e182b1a6661838a86cca7d0aac18be2e098b2106a7d517192c568ee08a845f73d29788cf035c3145b2"
        "1ab344d8062ea940000000000000000000000000000000000000000000000000000000000000000000003149e6"
        "70959884ea98bb33bca21c9505f1fc17b1d51ca59555a5d58c93f0f9c90103030102000404000000");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData = Base58::bitcoin.decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature = Base58::bitcoin.decode(
        "2gwuvwJ3mdEsjA8Gid6FXYuSwa2AAyFY6Btw8ifwSc2SPsfKBnD859C5mX4tLy6zQFHhKxSMMsW49o3dbJNiXDMo");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedTx =
        "7YPgNzjCnUd2zBb6ZC6bf1YaoLjhJPHixLUdTjqMjq1YdzADJCx2wsTTBFFrqDKSHXEL6ntRq8NVJTQMGzGH5AQRKw"
        "tKtutehxesmtzkZCPY9ADZ4ijFyveLmTt7kjZXX7ZWVoUmKAqiaYsPTex728uMBSRJpV4zRw2yKGdQRHTKy2QFEb9a"
        "cwLjmrbEgoyzPCarxjPhw21QZnNcy8RiYJB2mzZ9nvhrD5d2jB5TtdiroQPgTSdKFzkNEd7hJUKpqUppjDFcNHGK73"
        "FE9pCP2dKxCLH8Wfaez8bLtopjmWun9cbikxo7LZsarYzMXvxwZmerRd1";
    EXPECT_EQ(outputData.size(), 330ul);

    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 327ul);
    }
    
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = Base58::bitcoin.decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(TransactionCompiler, CardanoCompileWithSignaturesAndPubKeyType) {
     /// Prepare transaction input (protobuf)
    const auto coin = TWCoinTypeCardano;
    auto input = Cardano::Proto::SigningInput();
    input.mutable_transfer_message()->set_to_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    input.mutable_transfer_message()->set_change_address("addr1v8mv75d2evhr4kt048cx7m3f97x363ajadha8xv8dp96nuggpv8rn");
    input.mutable_transfer_message()->set_amount(1850000);

    auto* utxo1 = input.add_utxos();
    utxo1->set_address("addr1v8mv75d2evhr4kt048cx7m3f97x363ajadha8xv8dp96nuggpv8rn");
    utxo1->set_amount(1000000);
    auto txHash = parse_hex("d87f6e99c8d3a0fb22b1ea4de477f5a6d1f0e419450c2a194304371cada0ebb9");
    utxo1->mutable_out_point()->set_tx_hash(txHash.data(), txHash.size());
    utxo1->mutable_out_point()->set_output_index(0);

    auto* utxo2 = input.add_utxos();
    utxo2->set_address("addr1v8mv75d2evhr4kt048cx7m3f97x363ajadha8xv8dp96nuggpv8rn");
    utxo2->set_amount(4040957);
    utxo2->mutable_out_point()->set_tx_hash(txHash.data(), txHash.size());
    utxo2->mutable_out_point()->set_output_index(1);

    auto* tokenBundle1 = utxo2->add_token_amount();
    tokenBundle1->set_policy_id("122d15a15dc753d2b3ca9ee46c1c6ca41dda38d735942d9d259c785b");
    tokenBundle1->set_asset_name("TTToken-1");
    const auto tokenAmount1 = store(uint256_t(3000000));
    tokenBundle1->set_amount(tokenAmount1.data(), tokenAmount1.size());

    auto* tokenBundle2 = utxo2->add_token_amount();
    tokenBundle2->set_policy_id("122d15a15dc753d2b3ca9ee46c1c6ca41dda38d735942d9d259c785b");
    tokenBundle2->set_asset_name("TTToken-2");
    const auto tokenAmount2 = store(uint256_t(3000000));
    tokenBundle2->set_amount(tokenAmount2.data(), tokenAmount2.size());

    auto* tokenBundle3 = utxo2->add_token_amount();
    tokenBundle3->set_policy_id("122d15a15dc753d2b3ca9ee46c1c6ca41dda38d735942d9d259c785b");
    tokenBundle3->set_asset_name("TTToken-3");
    const auto tokenAmount3 = store(uint256_t(5000000));
    tokenBundle3->set_amount(tokenAmount3.data(), tokenAmount3.size());

    auto inputData = data(input.SerializeAsString());
    const auto preImageHash = TransactionCompiler::preImageHashes(coin, inputData);

    auto preOut = TxCompiler::Proto::PreSigningOutput();
    preOut.ParseFromArray(preImageHash.data(), (int)preImageHash.size());
    EXPECT_EQ(hex(preOut.data_hash()),"3e5a7c1d1afbc7e3ca783daba1beb12010fc4ecc748722558697509212c9f186");

    // Simulate signature, normally obtained from signature server
    const auto publicKeyData = parse_hex("17c55d712152ccabf28215fe2d008d615f94796e098a97f1aa43d986ac3cb946");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto sig = parse_hex("1096ddcfb2ad21a4c0d861ef3fabe18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d829bf0bcf1b631e86f0e");

    /// Compile transaction info
    const Data outData = TransactionCompiler::compileWithSignaturesAndPubKeyType(coin, inputData, {sig}, {publicKeyData}, TWPublicKeyTypeED25519);

    const auto expectedTx = 
        "83a40082825820d87f6e99c8d3a0fb22b1ea4de477f5a6d1f0e419450c2a194304371cada0ebb901825820d87f6e99c8d3a0fb22b1ea4de477f5a6d1f0e419450c2a194304371cada0ebb9000182825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a34681a001c3a9082581d61f6cf51aacb2e3ad96fa9f06f6e292f8d1d47b2eb6fd39987684ba9f1821a002e0feea1581c122d15a15dc753d2b3ca9ee46c1c6ca41dda38d735942d9d259c785ba3495454546f6b656e2d311a002dc6c0495454546f6b656e2d321a002dc6c0495454546f6b656e2d331a004c4b40021a0002a0bf0300a1008182582017c55d712152ccabf28215fe2d008d615f94796e098a97f1aa43d986ac3cb94658401096ddcfb2ad21a4c0d861ef3fabe18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d829bf0bcf1b631e86f0ef6";

    {
        auto output = Cardano::Proto::SigningOutput();
        output.ParseFromArray(outData.data(), (int)outData.size());
        EXPECT_EQ(hex(output.encoded()), expectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        const Data outputData = TransactionCompiler::compileWithSignaturesAndPubKeyType(
            coin, inputData, {sig, sig}, {publicKeyData}, TWPublicKeyTypeED25519);
        Cardano::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        const Data outputData = TransactionCompiler::compileWithSignaturesAndPubKeyType(
            coin, inputData, {}, {}, TWPublicKeyTypeED25519);
        Cardano::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, HarmonyCompileWithSignatures) {
    // txHash 0x238c0db5f139422d64d12b3d5208243b4b355bfb87024cec7795660291a628d0 on https://explorer.ps.hmny.io/
    /// Step 1: Prepare transaction input (protobuf)
    auto coin = TWCoinTypeHarmony;
    auto input = TW::Harmony::Proto::SigningInput();
    auto trasactionMsg = input.mutable_transaction_message();
    auto receiver = "one1y563nrrtcpu7874cry68ehxwrpteyhp0sztlym";
    trasactionMsg->set_to_address(receiver);
    auto payload = parse_hex("");
    trasactionMsg->set_payload(payload.data(), payload.size());

    uint256_t MAIN_NET = 0x4;
    auto value = store(MAIN_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t(0));
    trasactionMsg->set_nonce(value.data(), value.size());

    value = store(uint256_t(1000000000000000));
    trasactionMsg->set_gas_price(value.data(), value.size());

    value = store(uint256_t(1000000));
    trasactionMsg->set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x0"));
    trasactionMsg->set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    trasactionMsg->set_to_shard_id(value.data(), value.size());

    value = store(uint256_t(10));
    trasactionMsg->set_amount(value.data(), value.size());

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImage = data(preSigningOutput.data());
    auto preImageHash = data(preSigningOutput.data_hash());

    std::string expectedPreImage = "e98087038d7ea4c68000830f42408080942535198c6bc079e3fab819347cdcce1857925c2f0a80048080";
    std::string expectedPreImageHash = "fd1be8579542dc60f15a6218887cc1b42945bf04b50205d15ad7df8b5fac5714";
    ASSERT_EQ(hex(preImage), expectedPreImage);
    ASSERT_EQ(hex(preImageHash), expectedPreImageHash);

    const auto privateKey = PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    Data signature = parse_hex("43824f50bf4b16ebe1020114de16e3579bdb5f3dcaa26117de87a73b5414b72550506609fd60e3cb565b1f9bae0952d37f3a6c6be262380f7f18cbda5216f34300");
    const PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, preImageHash));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKey.bytes});
    const auto ExpectedTx =
        "f8698087038d7ea4c68000830f42408080942535198c6bc079e3fab819347cdcce1857925c2f0a802ba043824f50bf4b16ebe1020114de16e3579bdb5f3dcaa26117de87a73b5414b725a050506609fd60e3cb565b1f9bae0952d37f3a6c6be262380f7f18cbda5216f343";
    
    {
        TW::Harmony::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }
    
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::Harmony::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Harmony::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKey.bytes});
        Harmony::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {}, {});
        Harmony::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(TransactionCompiler, IoTeXCompileWithSignatures) {
    const auto coin = TWCoinTypeIoTeX;

    const auto privateKey0 =
        parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f");
    const auto privateKey1 =
        parse_hex("6021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d");
    const auto pubKey0 =
        parse_hex("034e18306ae9ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c");
    const auto pubKey1 =
        parse_hex("0253ad2f3b734a197f64911242aabc9b5b10bf5744949f5396e56427f35448eafa");
    const auto ExpectedTx0 =
        "0a4c0801107b18f8062203393939523e0a033435361229696f313837777a703038766e686a6a706b79646e7239"
        "37716c68386b683064706b6b797466616d386a1a0c68656c6c6f20776f726c64211241044e18306ae9ef4ec9d0"
        "7bf6e705442d4d1a75e6cdf750330ca2d880f2cc54607c9c33deb9eae9c06e06e04fe9ce3d43962cc67d5aa34f"
        "beb71270d4bad3d648d91a41555cc8af4181bf85c044c3201462eeeb95374f78aa48c67b87510ee63d5e502372"
        "e53082f03e9a11c1e351de539cedf85d8dff87de9d003cb9f92243541541a000";
    const auto ExpectedTx1 =
        "0a4c0801107b18f8062203393939523e0a033435361229696f313837777a703038766e686a6a706b79646e7239"
        "37716c68386b683064706b6b797466616d386a1a0c68656c6c6f20776f726c642112410453ad2f3b734a197f64"
        "911242aabc9b5b10bf5744949f5396e56427f35448eafa84a5d74b49ecb56e011b18c3d5a300e8cff7c6b39d33"
        "0d1d3799c4700a0b1be21a41de4be56ce74dce8e526590f5b5f947385b00947c4c2ead014429aa706a2470055c"
        "56c7e57d1b119b487765d59b21bcdeafac25108f6929a14f9edf4b2309534501";

    const auto prkey0 = PrivateKey(privateKey0);
    const PublicKey pbkey0 = prkey0.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    const auto prkey1 = PrivateKey(privateKey1);
    const PublicKey pbkey1 = prkey1.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::IoTeX::Proto::SigningInput();
    input.set_version(1);
    input.set_nonce(123);
    input.set_gaslimit(888);
    input.set_gasprice("999");
    auto tsf = input.mutable_transfer();
    tsf->set_amount("456");
    tsf->set_recipient("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j");
    auto text = parse_hex("68656c6c6f20776f726c6421"); // "hello world!"
    tsf->set_payload(text.data(), text.size());

    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());

    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    auto preImage = data(preSigningOutput.data());
    auto preImageHash = data(preSigningOutput.data_hash());

    std::string expectedPreImage =
        "0801107b18f8062203393939523e0a033435361229696f313837777a703038766e686a6a706b79646e72393771"
        "6c68386b683064706b6b797466616d386a1a0c68656c6c6f20776f726c6421";
    std::string expectedPreImageHash =
        "0f17cd7f43bdbeff73dfe8f5cb0c0045f2990884e5050841de887cf22ca35b50";
    ASSERT_EQ(hex(preImage), expectedPreImage);
    ASSERT_EQ(hex(preImageHash), expectedPreImageHash);

    Data signature = parse_hex("555cc8af4181bf85c044c3201462eeeb95374f78aa48c67b87510ee63d5e502372e"
                               "53082f03e9a11c1e351de539cedf85d8dff87de9d003cb9f92243541541a000");

    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(pbkey0.verify(signature, preImageHash));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {pbkey0.bytes});
    
    {
        TW::IoTeX::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(output.encoded()), ExpectedTx0);
    }
    
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::IoTeX::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        EXPECT_EQ(hex(PrivateKey(privateKey0).getPublicKey(TWPublicKeyTypeSECP256k1).bytes),
                  hex(pubKey0));
        signingInput.set_privatekey(prkey0.bytes.data(), prkey0.bytes.size());
        TW::IoTeX::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx0);
    }

    { // more signatures
        TW::IoTeX::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        EXPECT_EQ(hex(PrivateKey(privateKey1).getPublicKey(TWPublicKeyTypeSECP256k1).bytes),
                  hex(pubKey1));
        signingInput.set_privatekey(prkey1.bytes.data(), prkey1.bytes.size());
        TW::IoTeX::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx1);
    }

    { // Negative: invalid public key
        const auto publicKeyBlake =
            parse_hex("6641abedacf9483b793afe1718689cc9420bbb1c");
        EXPECT_EXCEPTION(
            TransactionCompiler::compileWithSignatures(
                coin, inputStrData, {signature}, {publicKeyBlake}),
            "Invalid public key");
    }

    { // Negative: not enough signatures
        outputData =
            TransactionCompiler::compileWithSignatures(coin, inputStrData, {}, {pbkey0.bytes});

        TW::IoTeX::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
        EXPECT_EQ(output.error_message(), "empty signatures or publickeys");
    }

    { // Negative: not enough publicKey
        outputData =
            TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {});

        TW::IoTeX::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
        EXPECT_EQ(output.error_message(), "empty signatures or publickeys");
    }

    { // Negative: not one to on
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature}, {pbkey0.bytes, pbkey1.bytes});

        TW::IoTeX::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
        EXPECT_EQ(output.error_message(), "signatures and publickeys size can only be one");
    }
}