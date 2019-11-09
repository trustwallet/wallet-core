
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "Harmony/Staking.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "TWTestUtilities.h"
#include "proto/Harmony.pb.h"
#include "uint256.h"
#include <TrustWalletCore/TWHarmonySigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace Harmony;

static auto TEST_ACCOUNT = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9";

static auto PRIVATE_KEY =
    PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));

TEST(TWHarmonyStakingSigner, CreateValidator) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto createValidatorMsg = stakingMessage->mutable_create_validator_message();

    createValidatorMsg->set_validator_address(TEST_ACCOUNT);

    auto description = createValidatorMsg->mutable_description();
    description->set_name("Alice");
    description->set_identity("alice");
    description->set_website("alice.harmony.one");
    description->set_security_contact("Bob");
    description->set_details("Don't mess with me!!!");

    auto commission = createValidatorMsg->mutable_commission_rates();

    // (value, precision): (1, 1) represents 0.1
    value = store(uint256_t("1"));
    commission->mutable_rate()->set_value(value.data(), value.size());
    value = store(uint256_t("1"));
    commission->mutable_rate()->set_precision(value.data(), value.size());

    // (value, precision): (9, 1) represents 0.9
    value = store(uint256_t("9"));
    commission->mutable_max_rate()->set_value(value.data(), value.size());
    value = store(uint256_t("1"));
    commission->mutable_max_rate()->set_precision(value.data(), value.size());

    // (value, precision): (5, 2) represents 0.05
    value = store(uint256_t("5"));
    commission->mutable_max_change_rate()->set_value(value.data(), value.size());
    value = store(uint256_t("2"));
    commission->mutable_max_change_rate()->set_precision(value.data(), value.size());

    value = store(uint256_t("10"));
    createValidatorMsg->set_min_self_delegation(value.data(), value.size());

    value = store(uint256_t("3000"));
    createValidatorMsg->set_max_total_delegation(value.data(), value.size());

    value = parse_hex("b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df"
                      "2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611");
    createValidatorMsg->add_slot_pub_keys(value.data(), value.size());

    value = store(uint256_t("100"));
    createValidatorMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0x02"));
    stakingMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    stakingMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonySignerSign(inputTWData);

    auto output = Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "476e8a0fe478e0d03ff10222d4d590bca8cee3ec51b830f4fc4a8bee5d0e9d28";
    auto shouldBeS = "3b2be18e73b2f99d7e2691485a0e166f28e62815079c126e68f876dc97339f8f";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}

TEST(TWHarmonyStakingSigner, EditValidator) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto editValidatorMsg = stakingMessage->mutable_edit_validator_message();

    editValidatorMsg->set_validator_address(TEST_ACCOUNT);

    auto description = editValidatorMsg->mutable_description();
    description->set_name("Alice");
    description->set_identity("alice");
    description->set_website("alice.harmony.one");
    description->set_security_contact("Bob");
    description->set_details("Don't mess with me!!!");

    auto commissionRate = editValidatorMsg->mutable_commission_rate();

    // (value, precision): (1, 1) represents 0.1
    value = store(uint256_t("1"));
    commissionRate->set_value(value.data(), value.size());
    value = store(uint256_t("1"));
    commissionRate->set_precision(value.data(), value.size());

    value = store(uint256_t("10"));
    editValidatorMsg->set_min_self_delegation(value.data(), value.size());

    value = store(uint256_t("3000"));
    editValidatorMsg->set_max_total_delegation(value.data(), value.size());

    value = parse_hex("b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df"
                      "2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611");
    editValidatorMsg->set_slot_key_to_remove(value.data(), value.size());
    editValidatorMsg->set_slot_key_to_add(value.data(), value.size());

    value = store(uint256_t("0x02"));
    stakingMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    stakingMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonySignerSign(inputTWData);

    auto output = Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "27";
    auto shouldBeR = "5e54b55272f6bf5ffeca10d85976749d6b844cc9f30ba3285b9ab8a82d53e3e3";
    auto shouldBeS = "3ce04d9a9f834e20b22aa918ead346c84a04b1504fe3ff9e38f21c5e5712f013";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}

TEST(TWHarmonyStakingSigner, Delegate) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto delegateMsg = stakingMessage->mutable_delegate_message();
    delegateMsg->set_delegator_address(TEST_ACCOUNT);
    delegateMsg->set_validator_address(TEST_ACCOUNT);

    value = store(uint256_t("0xa"));
    delegateMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0x2"));
    stakingMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    stakingMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonySignerSign(inputTWData);

    auto output = Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80";
    auto shouldBeS = "5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}

TEST(TWHarmonyStakingSigner, Undelegate) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto undelegateMsg = stakingMessage->mutable_undelegate_message();
    undelegateMsg->set_delegator_address(TEST_ACCOUNT);
    undelegateMsg->set_validator_address(TEST_ACCOUNT);

    value = store(uint256_t("0xa"));
    undelegateMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0x2"));
    stakingMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    stakingMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonySignerSign(inputTWData);

    auto output = Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "5bf8c653567defe2c3728732bc9d67dd099a977df91c740a883fd89e03abb6e2";
    auto shouldBeS = "5202c4b51652d5144c6a30d14d1a7a316b5a4a6b49be985b4bc6980e49f7acb7";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}

TEST(TWHarmonyStakingSigner, CollectRewards) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto collectRewardsMsg = stakingMessage->mutable_collect_rewards();
    collectRewardsMsg->set_delegator_address(TEST_ACCOUNT);

    value = store(uint256_t("0x2"));
    stakingMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    stakingMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const uint8_t *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonySignerSign(inputTWData);

    auto output = Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "4c15c72f42577001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625";
    auto shouldBeS = "55c13ea17c3efd1cd91f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}
