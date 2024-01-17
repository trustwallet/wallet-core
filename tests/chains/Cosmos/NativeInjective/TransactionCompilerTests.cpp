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

namespace TW::Cosmos::nativeInjective::tests {

TEST(NativeInjectiveCompiler, CompileWithSignatures) {
    // Successfully broadcasted: https://www.mintscan.io/injective/transactions/B77D61590353C4AEDEAE2BBFF9E406DCF90E8D3A1A723BF22860F1E0A2617058

    const auto coin = TWCoinTypeNativeInjective;
    TW::Cosmos::Proto::SigningInput input;

    PrivateKey privateKey =
        PrivateKey(parse_hex("727513ec3c54eb6fae24f2ff756bbc4c89b82945c6538bbd173613ae3de719d3"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    /// Step 1: Prepare transaction input (protobuf)
    input.set_account_number(88701);
    input.set_chain_id("injective-1");
    input.set_memo("");
    input.set_sequence(0);

    PublicKey publicKey = privateKey.getPublicKey(TWCoinTypePublicKeyType(coin));
    const auto pubKeyBz = publicKey.bytes;
    ASSERT_EQ(hex(pubKeyBz), "04088ac2919987d927368cb2be2ade44cd0ed3616745a9699cae264b3fc5a7c3607d99f441b8340990ee990cb3eaf560f1f0bafe600c7e94a4be8392166984f728");
    input.set_public_key(pubKeyBz.data(), pubKeyBz.size());

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("inj1d0jkrsd09c7pule43y3ylrul43lwwcqaky8w8c");
    message.set_to_address("inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("inj");
    amountOfTx->set_amount("10000000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(110000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("inj");
    amountOfFee->set_amount("100000000000000");

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
        "0a8f010a8c010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e64126c0a2a696e6a3164306a6b7273643039633770756c6534337933796c72756c34336c77776371616b7938773863122a696e6a31786d706b6d78723461733030656d32337463327a676d7579793267723468337767636c3676641a120a03696e6a120b3130303030303030303030129c010a7c0a740a2d2f696e6a6563746976652e63727970746f2e763162657461312e657468736563703235366b312e5075624b657912430a4104088ac2919987d927368cb2be2ade44cd0ed3616745a9699cae264b3fc5a7c3607d99f441b8340990ee990cb3eaf560f1f0bafe600c7e94a4be8392166984f72812040a020801121c0a160a03696e6a120f31303030303030303030303030303010b0db061a0b696e6a6563746976652d3120fdb405");
    EXPECT_EQ(hex(preImageHash),
        "57dc10c3d1893ff16e1f5a47fa4b2e96f37b9c57d98a42d88c971debb4947ec9");


    auto expectedTx = R"({"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"Co8BCowBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmwKKmluajFkMGprcnNkMDljN3B1bGU0M3kzeWxydWw0M2x3d2NxYWt5OHc4YxIqaW5qMXhtcGtteHI0YXMwMGVtMjN0YzJ6Z211eXkyZ3I0aDN3Z2NsNnZkGhIKA2luahILMTAwMDAwMDAwMDASnAEKfAp0Ci0vaW5qZWN0aXZlLmNyeXB0by52MWJldGExLmV0aHNlY3AyNTZrMS5QdWJLZXkSQwpBBAiKwpGZh9knNoyyvireRM0O02FnRalpnK4mSz/Fp8NgfZn0Qbg0CZDumQyz6vVg8fC6/mAMfpSkvoOSFmmE9ygSBAoCCAESHAoWCgNpbmoSDzEwMDAwMDAwMDAwMDAwMBCw2wYaQPep7ApSEXC7VWbKlz08c6G2mxYtmc4CIFkYmZHsRAY3MzOU/xyedfrYTrEUOTlp8gmJsDbx3+0olJ6QbcAHdCE="})";
    Data signature;

    {
        TW::Cosmos::Proto::SigningOutput output;
        ANY_SIGN(input, coin);
        assertJSONEqual(
            output.serialized(),
            expectedTx);

        signature = data(output.signature());
        EXPECT_EQ(hex(signature),
                  "f7a9ec0a521170bb5566ca973d3c73a1b69b162d99ce022059189991ec440637333394ff1c9e75fad84eb114393969f20989b036f1dfed28949e906dc0077421");

        ASSERT_TRUE(publicKey.verify(signature, data(preImageHash.data())));
    }

    {
        const Data outputData = TransactionCompiler::compileWithSignatures(
            coin, protoInputData, {signature}, {publicKey.bytes});
        Cosmos::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.error(), Common::Proto::OK);
        EXPECT_EQ(output.serialized(), expectedTx);
        EXPECT_EQ(hex(output.signature()), "f7a9ec0a521170bb5566ca973d3c73a1b69b162d99ce022059189991ec440637333394ff1c9e75fad84eb114393969f20989b036f1dfed28949e906dc0077421");
    }
}

}
