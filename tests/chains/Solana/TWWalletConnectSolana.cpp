// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base58.h"
#include "proto/Solana.pb.h"
#include "proto/WalletConnect.pb.h"
#include "Coin.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWWalletConnectRequest.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Solana {

TEST(TWWalletConnectSolana, Transfer) {
    auto private_key = Base58::decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr");
    const auto payload = R"({"transaction":"AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA="})";

    WalletConnect::Proto::ParseRequestInput parsingInput;
    parsingInput.set_method(WalletConnect::Proto::Method::SolanaSignTransaction);
    parsingInput.set_payload(payload);

    const auto parsinginputData = parsingInput.SerializeAsString();
    const auto parsingInputDataPtr = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(parsinginputData.c_str()), parsinginputData.size()));

    const auto outputDataPtr = WRAPD(TWWalletConnectRequestParse(TWCoinTypeSolana, parsingInputDataPtr.get()));

    WalletConnect::Proto::ParseRequestOutput parsingOutput;
    parsingOutput.ParseFromArray(
        TWDataBytes(outputDataPtr.get()),
        static_cast<int>(TWDataSize(outputDataPtr.get()))
    );

    EXPECT_EQ(parsingOutput.error(), Common::Proto::SigningError::OK);

    // Step 2: Set missing fields.
    ASSERT_TRUE(parsingOutput.has_solana());
    Proto::SigningInput signingInput = parsingOutput.solana();

    signingInput.set_private_key(private_key.data(), private_key.size());
    signingInput.set_tx_encoding(Proto::Encoding::Base64);

    Proto::SigningOutput output;
    ANY_SIGN(signingInput, TWCoinTypeSolana);

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(output.encoded(), "AQPWaOi7dMdmQpXi8HyQQKwiqIftrg1igGQxGtZeT50ksn4wAnyH4DtDrkkuE0fqgx80LTp4LwNN9a440SrmoA8BAAEDZsL1CMnFVcrMn7JtiOiN1U4hC7WovOVof2DX51xM0H/GizyJTHgrBanCf8bGbrFNTn0x3pCGq30hKbywSTr6AgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAgIAAQwCAAAAKgAAAAAAAAA=");
}

} // namespace TW::Binance
