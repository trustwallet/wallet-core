
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Zcash/Signer.h"
#include "Zcash/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "Data.h"
#include "Coin.h"
#include "Zcash/Transaction.h"

#include <TrustWalletCore/TWBitcoinScript.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWZcashTransaction, Encode) {
    // Test vector 3 https://github.com/zcash/zips/blob/master/zip-0243.rst
    auto transaction = Zcash::Transaction();
    transaction.lockTime = 0x0004b029;
    transaction.expiryHeight = 0x0004b048;
    transaction.branchId = Zcash::SaplingBranchID;

    auto outpoint0 = Bitcoin::OutPoint(parse_hex("a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9"), 1);
    transaction.inputs.emplace_back(outpoint0, Bitcoin::Script(parse_hex("483045022100a61e5d557568c2ddc1d9b03a7173c6ce7c996c4daecab007ac8f34bee01e6b9702204d38fdc0bcf2728a69fde78462a10fb45a9baa27873e6a5fc45fb5c76764202a01210365ffea3efa3908918a8b8627724af852fc9b86d7375b103ab0543cf418bcaa7f")), 0xfffffffe);

    auto script0 = Bitcoin::Script(parse_hex("76a9148132712c3ff19f3a151234616777420a6d7ef22688ac"));
    transaction.outputs.emplace_back(0x02625a00, script0);

    auto script1 = Bitcoin::Script(parse_hex("76a9145453e4698f02a38abdaa521cd1ff2dee6fac187188ac"));
    transaction.outputs.emplace_back(0x0098958b, script1);

    auto unsignedData = Data{};
    transaction.encode(unsignedData);

    ASSERT_EQ(hex(unsignedData),
        /* header */          "04000080"
        /* versionGroupId */  "85202f89"
        /* vin */             "01""a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9""01000000""6b483045022100a61e5d557568c2ddc1d9b03a7173c6ce7c996c4daecab007ac8f34bee01e6b9702204d38fdc0bcf2728a69fde78462a10fb45a9baa27873e6a5fc45fb5c76764202a01210365ffea3efa3908918a8b8627724af852fc9b86d7375b103ab0543cf418bcaa7f""feffffff"
        /* vout */            "02""005a620200000000""1976a9148132712c3ff19f3a151234616777420a6d7ef22688ac"
                                  "8b95980000000000""1976a9145453e4698f02a38abdaa521cd1ff2dee6fac187188ac"
        /* lockTime */        "29b00400"
        /* expiryHeight */    "48b00400"
        /* valueBalance */    "0000000000000000"
        /* vShieldedSpend */  "00"
        /* vShieldedOutput */ "00"
        /* vJoinSplit */      "00"
    );

    auto scriptCode = Bitcoin::Script(parse_hex("76a914507173527b4c3318a2aecd793bf1cfed705950cf88ac"));
    auto preImage = transaction.getPreImage(scriptCode, 0, TWBitcoinSigHashTypeAll, 0x02faf080);
    ASSERT_EQ(hex(preImage),
        /* header */              "04000080"
        /* versionGroupId */      "85202f89"
        /* hashPrevouts */        "fae31b8dec7b0b77e2c8d6b6eb0e7e4e55abc6574c26dd44464d9408a8e33f11"
        /* hashSequence */        "6c80d37f12d89b6f17ff198723e7db1247c4811d1a695d74d930f99e98418790"
        /* hashOutputs */         "d2b04118469b7810a0d1cc59568320aad25a84f407ecac40b4f605a4e6868454"
        /* hashJoinSplits */      "0000000000000000000000000000000000000000000000000000000000000000"
        /* hashShieldedSpends */  "0000000000000000000000000000000000000000000000000000000000000000"
        /* hashShieldedOutputs */ "0000000000000000000000000000000000000000000000000000000000000000"
        /* lockTime */            "29b00400"
        /* expiryHeight */        "48b00400"
        /* valueBalance */        "0000000000000000"
        /* hashType */            "01000000"
        /* prevout */             "a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9""01000000"
        /* scriptCode */          "1976a914507173527b4c3318a2aecd793bf1cfed705950cf88ac"
        /* amount */              "80f0fa0200000000"
        /* sequence */            "feffffff"
    );

    auto sighash = transaction.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAll, 0x02faf080, Bitcoin::BASE);
    ASSERT_EQ(hex(sighash), "f3148f80dfab5e573d5edfe7a850f5fd39234f80b5429d3a57edcc11e34c585b");

    // AnyoneCanPay|none
    preImage = transaction.getPreImage(scriptCode, 0, TWBitcoinSigHashType(TWBitcoinSigHashTypeAnyoneCanPay | TWBitcoinSigHashTypeNone), 0x02faf080);
    EXPECT_EQ(hex(preImage),
        "0400008085202f8900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000029b0040048b00400000000000000000082000000a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9010000001976a914507173527b4c3318a2aecd793bf1cfed705950cf88ac80f0fa0200000000feffffff"
    );

    sighash = transaction.getSignatureHash(scriptCode, 0, TWBitcoinSigHashTypeAnyoneCanPay, 0x02faf080, Bitcoin::BASE);
    EXPECT_EQ(hex(sighash), "f0bde4facddbc11f5e9ed2f5d5038083bec4a61627a2715a5ee9be7fb3152e9b");

    // AnyoneCanPay|Single
    preImage = transaction.getPreImage(scriptCode, 0, TWBitcoinSigHashType(TWBitcoinSigHashTypeAnyoneCanPay | TWBitcoinSigHashTypeSingle), 0x02faf080);
    EXPECT_EQ(hex(preImage),
        "0400008085202f890000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000055986938e432f825904fe288aa4feca1fe7eafa24aecd1bd6a9a739536b50a5469be00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000029b0040048b00400000000000000000083000000a8c685478265f4c14dada651969c45a65e1aeb8cd6791f2f5bb6a1d9952104d9010000001976a914507173527b4c3318a2aecd793bf1cfed705950cf88ac80f0fa0200000000feffffff"
    );

    sighash = transaction.getSignatureHash(scriptCode, 0, TWBitcoinSigHashType(TWBitcoinSigHashTypeAnyoneCanPay | TWBitcoinSigHashTypeSingle), 0x02faf080, Bitcoin::BASE);
    EXPECT_EQ(hex(sighash), "1e747b6a4a96aa9e7c1d7968221ec916bd30b514f8bca14b6f74d7c11c0742c2");
}

TEST(TWZcashTransaction, SaplingSigning) {
    // tx on mainnet
    // https://explorer.zcha.in/transactions/ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256
    const int64_t amount = 488000;
    const int64_t fee = 6000;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_coin_type(TWCoinTypeZcash);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS");

    auto hash0 = DATA("53685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(494000);
    auto script0 = parse_hex("76a914f84c7f4dd3c3dc311676444fdead6e6d290d50e388ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));

    auto plan = Zcash::TransactionBuilder::plan(input);
    plan.amount = amount;
    plan.fee = fee;
    plan.change = 0;
    plan.branchId = Data(Zcash::SaplingBranchID.begin(), Zcash::SaplingBranchID.end());

    auto& protoPlan = *input.mutable_plan();
    protoPlan = plan.proto();

    // Sign
    auto result = Bitcoin::TransactionSigner<Zcash::Transaction, Zcash::TransactionBuilder>::sign(input);
    ASSERT_TRUE(result) << std::to_string(result.error());
    auto signedTx = result.payload();

    // txid = "ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256"

    Data serialized;
    signedTx.encode(serialized);
    ASSERT_EQ(hex(serialized),
        "04000080"
        "85202f89"
        "01"
            "53685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a""00000000""6b483045022100ca0be9f37a4975432a52bb65b25e483f6f93d577955290bb7fb0060a93bfc92002203e0627dff004d3c72a957dc9f8e4e0e696e69d125e4d8e275d119001924d3b48012103b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6""ffffffff"
        "01"
            "4072070000000000""1976a91449964a736f3713d64283fd0018626ba50091c7e988ac"
        "00000000"
        "00000000"
        "0000000000000000"
        "00"
        "00"
        "00"
    );
}

TEST(TWZcashTransaction, BlossomSigning) {
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
    auto utxoKey0 = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"), TWCurveSECP256k1);
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

    // Sign
    auto result = Bitcoin::TransactionSigner<Zcash::Transaction, Zcash::TransactionBuilder>::sign(input);
    ASSERT_TRUE(result) << std::to_string(result.error());
    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(serialized);
    ASSERT_EQ(hex(serialized), "0400008085202f8901de8c02c79c01018bd91dbc6b293eba03945be25762994409209a06d95c828123000000006b483045022100e6e5071811c08d0c2e81cb8682ee36a8c6b645f5c08747acd3e828de2a4d8a9602200b13b36a838c7e8af81f2d6e7e694ede28833a480cfbaaa68a47187655298a7f0121024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ffffffff01cf440000000000001976a914c3bacb129d85288a3deb5890ca9b711f7f71392688ac00000000000000000000000000000000000000");
}

TEST(TWZcashTransaction, Zip0317Fee) {
    // tx on mainnet
    // https://blockchair.com/zcash/transaction/092379d65d9b33be1322b2833e20cb573f87e49f73a3537c172354453dcee3a4

    const auto myAddress = "t1Nx4n8MXhXVTZMY6Vx2zbxsCz5VstD9nuv";
    const auto myPrivateKey = parse_hex("5313c6cb5767fac88a303dab4f5d96ee55b547ec99da0db7a20694ac9e395668");

    auto input = Bitcoin::Proto::SigningInput();
    input.set_coin_type(TWCoinTypeZcash);
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_zip_0317(true);
    input.set_to_address("t1S3JTzDWR7FzANsn3erXRPms2BfWVQgH9T");
    input.set_use_max_amount(true);
    input.add_private_key(myPrivateKey.data(), myPrivateKey.size());

    auto txHash = parse_hex("f8a8bdcd4b1b3c6b69b50ebbb26921c43583bb93f20e3ccf3c650791ef969b4e");
    std::reverse(txHash.begin(), txHash.end());
    auto redeemScript = Bitcoin::Script::lockScriptForAddress(myAddress, TWCoinTypeZcash).bytes;

    auto addUtxo = [&txHash, &redeemScript, &input](const uint32_t vout, const int64_t amount) {
        auto utxo = input.add_utxo();
        utxo->mutable_out_point()->set_hash(txHash.data(), txHash.size());
        utxo->mutable_out_point()->set_index(vout);
        utxo->mutable_out_point()->set_sequence(UINT32_MAX);
        utxo->set_script(redeemScript.data(), redeemScript.size());
        utxo->set_amount(amount);
    };

    addUtxo(0, 7000);
    addUtxo(1, 1'505'490);
    addUtxo(2, 7100);
    addUtxo(3, 7200);
    addUtxo(4, 7300);
    addUtxo(5, 7400);
    addUtxo(6, 7500);
    addUtxo(7, 7600);
    addUtxo(8, 7700);
    addUtxo(9, 7800);
    addUtxo(10, 7900);
    addUtxo(11, 8000);
    addUtxo(12, 8001);
    addUtxo(13, 8002);
    addUtxo(14, 8003);
    addUtxo(15, 8004);

    auto plan = Zcash::TransactionBuilder::plan(input);
    plan.branchId = Data(Zcash::Nu6BranchID.begin(), Zcash::Nu6BranchID.end());
    *input.mutable_plan() = plan.proto();

    // Sign
    auto result = Bitcoin::TransactionSigner<Zcash::Transaction, Zcash::TransactionBuilder>::sign(input);
    ASSERT_TRUE(result) << std::to_string(result.error());
    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(serialized);
    ASSERT_EQ(hex(serialized), "0400008085202f89104e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8000000006b4830450221008697d7c738af36b6c2009eee98ab8d10356168cdab1ad3499a993e55ecf5ab56022011762fd1b95abcc55b04a13b395f00d131d2588b29cbb892fa0438920f5bc151012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8010000006b483045022100eb066fc7ab4cbdd42e6e50479bc3e4a5717f0d2c29626831b649d86d8e204df40220333b886a0eb196055f22e19dc9f01c46c57258e91b150cd5587cda1b707a1056012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8020000006b483045022100a1d2744150254ae05942c42721d89e02d0c9992b75d7db2bce3ebfe8e2e6a0e902200abe593108cf1cdddeb02403c15dc087d2dd274c2f85a63bac2248ab2ce3ef34012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8030000006b483045022100f791d7d491a20b7ebd31e0465b9adb83e5994d0fa092c4c213d1a9d97ad2fb3b02207223f97c35cd3f482ff93bdae55a4d7c3087cffd790d689777a9a32271e835c7012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8040000006a47304402204f8fa75701453de79dde52936d2526c6bd31d98d45cbe481df25fcd482054620022056221c611c6af5c66bb302ebadabe76c158aa83c47b4927e90182e6fea0bb392012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8050000006b483045022100d28ed7ea432c2d122815be053c25a044e9d02a8dc5f52e12c58d7a833627a9a90220575fa325028e0abecc2be8c40db5fd8552337dc62d3acb9a8e919dd597927b81012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8060000006b48304502210082dc355620bb855e4fd04984054858376bb28d07f97b149ab49cb7ec6c42559c022005ce1af01f00d452afbc51b8a3c1f14e681f93552e94d66906a71f1ba1c00e3c012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8070000006b483045022100d06a9e04bc6be40913fda047ba19ed24f9a4a8cbd5e338994e22609d6a1a11b202207bf5fee15e9a8c1b17095f7f804d16ba02cba5071bda3383de3ee0a46d3b1dd5012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8080000006a4730440220617f682e60ff8f7fa4784b4d318891cdbac461a99f48087034064ed813d2063f022060cb338a8ee49898ec774d431d0867b5a15382be90c685f39fde4a41af8ff0a7012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8090000006b4830450221008e4f66cb5c69d98cc9a4f1e895fe3c645d4640f4a5f7e8337c3beea34915ab170220320e8d14cd3dbd26eab1c41eca2146089a59dafde04334cd321554183e809417012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f80a0000006b483045022100c4bbecaecdf6a9eb4a776b4f99541659dc73b8f2c28937e34e7cb637b5105d8302200092a7ae0eee8b4925e8c207c057f43f705b94e468053d4028a785f4652bc2b1012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f80b0000006b4830450221008f2228ac57a30d07cbfed7b0d39977e563d23f4f4776451f76e8b401c618f0710220095a73c8bef932d1865e55656620d3071221be279afd66f0827e39ca4eaa26d3012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f80c0000006b483045022100a52c7692a09c308ac9cd87c85afeaa37d69c661b8f7b6cdf8c02876037359cb8022006a3da236a86466add64fa6a38655d2a2b6fba05b84e25fa2583210a435be858012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f80d0000006a47304402202ce1f193c23e0262fdf62cb74c1669fa7c9e9de5a801434df43c0dc69b1d6aa1022048641ab533f539a5185136a6b2d933944703fa83ddae233297b98d6f89845792012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f80e0000006b483045022100db80a6d02c5cc9c21e94868654be891102a4e664ceab29edbd6ebc9106fc27290220509ddb845a48c2f94f4ec7995d12b01305ecc98eb49dd5b26826f6e4bd1ceaec012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f80f0000006b483045022100e40aba96f9dcaafb1ce43acf2cfec44f3c2c59340c8d0fa3cc46c6249efb27ca0220184c20c35ffd585efbb9d36049bcf60670f0120968c435dde2333631b5e1b102012103b6ced6ffee0d78974da26d910c8b36781e8598019a3982a04286384452418405ffffffff01a07f1700000000001976a914599686197c40d39a8e6272355f206a9523fab00288ac00000000000000000000000000000000000000");
}

TEST(TWZcashTransaction, SigningWithError) {
    const int64_t amount = 17615;
    const std::string toAddress = "t1biXYN8wJahR76SqZTe1LBzTLf3JAsmT93";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_coin_type(TWCoinTypeZcash);

    // Sign
    auto result = Zcash::Signer::sign(input);
    ASSERT_NE(result.error(), Common::Proto::OK);

    // PreImageHashes
    auto preResult = Zcash::Signer::preImageHashes(input);
    ASSERT_NE(preResult.error(), Common::Proto::OK);
}