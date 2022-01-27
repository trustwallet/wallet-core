// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWTransactionHelper.h>
#include <TrustWalletCore/TWData.h>
#include "proto/Binance.pb.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "interface/TWTestUtilities.h"
#include <gtest/gtest.h>
#include <thread>

using namespace TW;

TEST(TWTransactionHelper, ExternalSignatureSignBinance) {
    /// Step 1: Prepare transaction input (protobuf)
    const auto txInputData = WRAPD(TWTransactionHelperBuildInput(
        TWCoinTypeBinance,
        STRING("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2").get(),  // from
        STRING("bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5").get(),  // to
        STRING("1").get(),  // amount
        STRING("BNB").get(),  // asset
        STRING("").get(),  // memo
        STRING("").get()  // chainId
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
    const auto preImageHash = WRAPD(TWTransactionHelperPreImageHash(TWCoinTypeBinance, txInputData.get()));

    const auto preImageHashData = data(TWDataBytes(preImageHash.get()), TWDataSize(preImageHash.get()));
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
    const auto outputData = WRAPD(TWTransactionHelperCompileWithSignature(
        TWCoinTypeBinance,
        txInputData.get(),
        WRAPD(TWDataCreateWithData((TWData*)&signature)).get(),
        WRAPD(TWDataCreateWithData((TWData*)&publicKeyData)).get())
    );

    const auto ExpectedTx = "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679";
    {
        EXPECT_EQ(TWDataSize(outputData.get()), 189);
        Binance::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()), (int)TWDataSize(outputData.get())));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }

    { // Double check: check if simple signature process gives the same result
        Binance::Proto::SigningInput input;
        ASSERT_TRUE(input.ParseFromArray(TWDataBytes(txInputData.get()), (int)TWDataSize(txInputData.get())));
        auto key = parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832");
        input.set_private_key(key.data(), key.size());

        Binance::Proto::SigningOutput output;
        ANY_SIGN(input, TWCoinTypeBinance);

        ASSERT_EQ(hex(output.encoded()), ExpectedTx);
    }
}
