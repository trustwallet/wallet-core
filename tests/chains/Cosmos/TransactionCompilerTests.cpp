// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Cosmos.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(CosmosCompiler, CompileWithSignatures) {
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
        EXPECT_EQ(output.error(), Common::Proto::Error_invalid_params);
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

    { // JSON
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, jsonInputData, {signature}, {publicKey.bytes});
        Cosmos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(hex(output.serialized()), "7b226d6f6465223a22626c6f636b222c227478223a7b22666565223a7b22616d6f756e74223a5b7b22616d6f756e74223a2231303030222c2264656e6f6d223a227561746f6d227d5d2c22676173223a22323030303030227d2c226d656d6f223a22222c226d7367223a5b7b2274797065223a22636f736d6f732d73646b2f4d736753656e64222c2276616c7565223a7b22616d6f756e74223a5b7b22616d6f756e74223a22343030303030222c2264656e6f6d223a227561746f6d227d5d2c2266726f6d5f61646472657373223a22636f736d6f73316d6b793639636e38656b74777930383435766563397570736470686b7478743033676b776c78222c22746f5f61646472657373223a22636f736d6f733138733068646e736c6c6763636c7765753961796d77346e676b7472326b30726b7967647a6470227d7d5d2c227369676e617475726573223a5b7b227075625f6b6579223a7b2274797065223a2274656e6465726d696e742f5075624b6579536563703235366b31222c2276616c7565223a2241757a76584f51336f774c47663556476a65537a487a62704566526e312b616c4b30484234543464566a5a4a227d2c227369676e6174757265223a227454794f72627572724845486131347169773738653953746f5a7979476d6f6b7539384978597257436d744e38516f356d54654b6130424b4b44666747344c6d6d4e64775963725874715151374634644c33633236673d3d227d5d7d7d");
    }
}
