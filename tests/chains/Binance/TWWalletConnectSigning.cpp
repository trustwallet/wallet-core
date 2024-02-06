// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "proto/Binance.pb.h"
#include "proto/WalletConnect.pb.h"
#include "Coin.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWWalletConnectRequest.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Binance {

TEST(TWWalletConnectSign, SendOrder) {
    auto private_key = parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832");
    const auto payload = R"({"signerAddress":"bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2","signDoc":{"account_number":"19","chain_id":"chain-bnb","memo":"","data":null,"msgs":[{"inputs":[{"address":"bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2","coins":[{"amount":1001000000,"denom":"BNB"}]}],"outputs":[{"address":"bnb13zeh6hs97d5eu2s5qerguhv8ewwue6u4ywa6yf","coins":[{"amount":1001000000,"denom":"BNB"}]}]}],"sequence":"23","source":"1"}})";

    WalletConnect::Proto::ParseRequestInput parsingInput;
    parsingInput.set_method(WalletConnect::Proto::Method::CosmosSignAmino);
    parsingInput.set_payload(payload);

    const auto parsinginputData = parsingInput.SerializeAsString();
    const auto parsingInputDataPtr = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(parsinginputData.c_str()), parsinginputData.size()));

    const auto outputDataPtr = WRAPD(TWWalletConnectRequestParse(TWCoinTypeBinance, parsingInputDataPtr.get()));

    WalletConnect::Proto::ParseRequestOutput parsingOutput;
    parsingOutput.ParseFromArray(
        TWDataBytes(outputDataPtr.get()),
        static_cast<int>(TWDataSize(outputDataPtr.get()))
    );

    EXPECT_EQ(parsingOutput.error(), Common::Proto::SigningError::OK);

    // Step 2: Set missing fields.
    ASSERT_TRUE(parsingOutput.has_binance());
    Proto::SigningInput signingInput = parsingOutput.binance();

    signingInput.set_private_key(private_key.data(), private_key.size());

    Proto::SigningOutput output;
    ANY_SIGN(signingInput, TWCoinTypeBinance);

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(hex(output.signature()), "3c24c784c6bbf99d54ffabb153edcb6d3c4a774936df5c72a5d32897256f8e062f320fb4753302fb0a96f08c475974d20edfd1a27bbeeda73587f58ddc958975");
    EXPECT_EQ(output.signature_json(), R"({"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Amo1kgCI2Yw4iMpoxT38k/RWRgJgbLuH8P5e5TPbOOUC"},"signature":"PCTHhMa7+Z1U/6uxU+3LbTxKd0k231xypdMolyVvjgYvMg+0dTMC+wqW8IxHWXTSDt/Ronu+7ac1h/WN3JWJdQ=="})");
}

} // namespace TW::Binance
