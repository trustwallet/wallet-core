// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base64.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "TrustWalletCore/TWAnySigner.h"
#include "TestUtilities.h"
#include "TransactionCompiler.h"

namespace TW::Cosmos::evmos::tests {

TEST(EvmosCompiler, CompileWithSignatures) {
    // Successfully broadcasted: https://www.mintscan.io/evmos/transactions/02105B186FCA473C9F467B2D3BF487F6CE5DB26EE54BCD1667DDB7A2DA0E2489

    const auto coin = TWCoinTypeNativeEvmos;
    TW::Cosmos::Proto::SigningInput input;

    PrivateKey privateKey =
        PrivateKey(parse_hex("727513ec3c54eb6fae24f2ff756bbc4c89b82945c6538bbd173613ae3de719d3"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    /// Step 1: Prepare transaction input (protobuf)
    input.set_account_number(106619981);
    input.set_chain_id("evmos_9001-2");
    input.set_memo("");
    input.set_sequence(0);

    PublicKey publicKey = privateKey.getPublicKey(TWCoinTypePublicKeyType(coin));
    const auto pubKeyBz = publicKey.bytes;
    ASSERT_EQ(hex(pubKeyBz), "04088ac2919987d927368cb2be2ade44cd0ed3616745a9699cae264b3fc5a7c3607d99f441b8340990ee990cb3eaf560f1f0bafe600c7e94a4be8392166984f728");
    input.set_public_key(pubKeyBz.data(), pubKeyBz.size());

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("evmos1d0jkrsd09c7pule43y3ylrul43lwwcqa7vpy0g");
    message.set_to_address("evmos17dh3frt0m6kdd3m9lr6e6sr5zz0rz8cvxd7u5t");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("aevmos");
    amountOfTx->set_amount("10000000000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(137840);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("aevmos");
    amountOfFee->set_amount("5513600000000000");

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
        "0a9c010a99010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412790a2c65766d6f733164306a6b7273643039633770756c6534337933796c72756c34336c7777637161377670793067122c65766d6f733137646833667274306d366b6464336d396c723665367372357a7a30727a3863767864377535741a1b0a066165766d6f7312113130303030303030303030303030303030127b0a570a4f0a282f65746865726d696e742e63727970746f2e76312e657468736563703235366b312e5075624b657912230a2102088ac2919987d927368cb2be2ade44cd0ed3616745a9699cae264b3fc5a7c36012040a02080112200a1a0a066165766d6f7312103535313336303030303030303030303010f0b4081a0c65766d6f735f393030312d3220cdc8eb32");
    EXPECT_EQ(hex(preImageHash),
        "9912eb629e215027b8d587939b1af72a9f70ae326bcaf48dfe77a729fc4ac632");


    auto expectedTx = R"({"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpwBCpkBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnkKLGV2bW9zMWQwamtyc2QwOWM3cHVsZTQzeTN5bHJ1bDQzbHd3Y3FhN3ZweTBnEixldm1vczE3ZGgzZnJ0MG02a2RkM205bHI2ZTZzcjV6ejByejhjdnhkN3U1dBobCgZhZXZtb3MSETEwMDAwMDAwMDAwMDAwMDAwEnsKVwpPCigvZXRoZXJtaW50LmNyeXB0by52MS5ldGhzZWNwMjU2azEuUHViS2V5EiMKIQIIisKRmYfZJzaMsr4q3kTNDtNhZ0WpaZyuJks/xafDYBIECgIIARIgChoKBmFldm1vcxIQNTUxMzYwMDAwMDAwMDAwMBDwtAgaQKrmMaaSKnohf3ahyCOYdRJKBKJjr4WkkA/cbn6FRdF0Gd6FHSzBP8S4v4VNiy3KC47TD0C+sUBO413gCzjo8/U="})";
    Data signature;

    {
        TW::Cosmos::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        assertJSONEqual(
            output.serialized(),
            expectedTx);

        signature = data(output.signature());
        EXPECT_EQ(hex(signature),
                  "aae631a6922a7a217f76a1c8239875124a04a263af85a4900fdc6e7e8545d17419de851d2cc13fc4b8bf854d8b2dca0b8ed30f40beb1404ee35de00b38e8f3f5");

        ASSERT_TRUE(publicKey.verify(signature, data(preImageHash.data())));
    }

    {
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, protoInputData, {signature}, {publicKey.bytes});
        Cosmos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(output.serialized(), expectedTx);
        EXPECT_EQ(hex(output.signature()), hex(signature));
    }
}

}
