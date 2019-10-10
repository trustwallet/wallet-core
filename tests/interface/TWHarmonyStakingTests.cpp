
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
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto delegateMsg = input.mutable_delegate_message();
    delegateMsg->set_delegator_address(TEST_ACCOUNT.string());
    delegateMsg->set_validator_address(TEST_ACCOUNT.string());

    value = store(uint256_t("0xa"));
    delegateMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0x2"));
    input.set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    input.set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    input.set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonyStakingSignerSign(inputTWData);

    auto output = Proto::StakingTransactionOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80";
    auto shouldBeS = "5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}
