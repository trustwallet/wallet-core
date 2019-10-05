
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "Harmony/StakingTransaction.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "TWTestUtilities.h"
#include "proto/Harmony.pb.h"
#include "uint256.h"
#include <TrustWalletCore/TWHarmonyStakingSigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace Harmony;

static auto TEST_ACCOUNT = Address("one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9");

TEST(TWHarmonyStakingSigner, Delegate) {
    Proto::StakingTransactionInput input;

    const auto privateKey =
        PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto delegateMsg = input.mutable_delegate_message();
    delegateMsg->set_delegator_address(TEST_ACCOUNT.string());
    delegateMsg->set_validator_address(TEST_ACCOUNT.string());

    value = store(uint256_t("0xa"));
    delegateMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0xa"));
    input.set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    input.set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    input.set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonyStakingSignerSign(inputTWData);

    auto output = Proto::StakingTransactionOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "2e43463bf0bfd1563d3702f301f92c15d167892e337d9831d4c2d3ba1507efbc";
    auto shouldBeS = "14e5f9d9618699316c69e720af3b373bc9d3c3d97962decd13dc3b48d2a4ac75";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}
