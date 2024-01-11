// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWEthereumRlp.h"
#include "proto/EthereumRlp.pb.h"
#include "HexCoding.h"
#include "TestUtilities.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(TWEthereumRlp, Eip1559) {
    auto chainId = store(10);
    auto nonce = store(6);
    auto maxInclusionFeePerGas = 2'000'000'000;
    auto maxFeePerGas = store(3'000'000'000);
    auto gasLimit = store(21'100);
    const auto* to = "0x6b175474e89094c44da98b954eedeac495271d0f";
    auto amount = 0;
    auto payload = parse_hex("a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1");

    EthereumRlp::Proto::EncodingInput input;
    auto* list = input.mutable_item()->mutable_list();

    list->add_items()->set_number_u256(chainId.data(), chainId.size());
    list->add_items()->set_number_u256(nonce.data(), nonce.size());
    list->add_items()->set_number_u64(maxInclusionFeePerGas);
    list->add_items()->set_number_u256(maxFeePerGas.data(), maxFeePerGas.size());
    list->add_items()->set_number_u256(gasLimit.data(), gasLimit.size());
    list->add_items()->set_address(to);
    list->add_items()->set_number_u64(amount);
    list->add_items()->set_data(payload.data(), payload.size());
    // Append an empty `access_list`.
    list->add_items()->mutable_list();

    auto inputData = input.SerializeAsString();
    auto inputTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size()));
    auto outputTWData = WRAPD(TWEthereumRlpEncode(TWCoinTypeEthereum, inputTWData.get()));

    EthereumRlp::Proto::EncodingOutput output;
    output.ParseFromArray(TWDataBytes(outputTWData.get()), static_cast<int>(TWDataSize(outputTWData.get())));

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_TRUE(output.error_message().empty());
    EXPECT_EQ(hex(output.encoded()), "f86c0a06847735940084b2d05e0082526c946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1c0");
}
