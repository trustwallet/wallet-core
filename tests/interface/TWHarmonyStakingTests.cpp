
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

TEST(TWHarmonyStakingSigner, NewValidator) {
    Proto::StakingTransactionInput input;

    const auto privateKey =
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto newValidatorMsg = input.mutable_new_validator_message();
    auto description = newValidatorMsg->mutable_description();
    description->set_name("Alice");
    description->set_identity("alice");
    description->set_website("alice.harmony.one");
    description->set_security_contact("Bob");
    description->set_details("Don't mess with me!!!");
    auto commission = newValidatorMsg->mutable_commission();

    value = store(uint256_t("100"));
    commission->mutable_rate()->set_value(value.data(), value.size());

    value = store(uint256_t("0"));
    commission->mutable_rate()->set_precision(value.data(), value.size());

    value = store(uint256_t("150"));
    commission->mutable_max_rate()->set_value(value.data(), value.size());

    value = store(uint256_t("0"));
    commission->mutable_max_rate()->set_precision(value.data(), value.size());

    value = store(uint256_t("5"));
    commission->mutable_max_change_rate()->set_value(value.data(), value.size());

    value = store(uint256_t("0"));
    commission->mutable_max_change_rate()->set_precision(value.data(), value.size());

    value = store(uint256_t("10"));
    newValidatorMsg->set_min_self_delegation(value.data(), value.size());
    newValidatorMsg->set_staking_address(TEST_ACCOUNT.string());

    value = parse_hex("b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df"
                      "2af03c5ced1ad181e7d12f0e6dd5307a73b62247608611");
    newValidatorMsg->set_pub_key(value.data(), value.size());

    value = store(uint256_t("100"));
    newValidatorMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0x02"));
    input.set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    input.set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64")); // 0x5208
    input.set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonyStakingSignerSign(inputTWData);

    auto output = Proto::StakingTransactionOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "0b1a797d11f7b0dad42abd66c542fab8af0f028b7159bb70e44fe68b2e4d9f2c";
    auto shouldBeS = "7b223662bdb4e1a084f8c506095886a1f5eda051927fab3516ab9258efc34cd7";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}

TEST(TWHarmonyStakingSigner, EditValidator) {
    Proto::StakingTransactionInput input;

    const auto privateKey =
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto editValidatorMsg = input.mutable_edit_validator_message();

    auto description = editValidatorMsg->mutable_description();
    description->set_name("Alice");
    description->set_identity("alice");
    description->set_website("alice.harmony.one");
    description->set_security_contact("Bob");
    description->set_details("Don't mess with me!!!");

    editValidatorMsg->set_staking_address(TEST_ACCOUNT.string());

    auto commissionRate = editValidatorMsg->mutable_commission_rate();
    value = store(uint256_t("100"));
    commissionRate->set_value(value.data(), value.size());
    value = store(uint256_t("0"));
    commissionRate->set_precision(value.data(), value.size());

    value = store(uint256_t("10"));
    editValidatorMsg->set_min_self_delegation(value.data(), value.size());

    value = store(uint256_t("0x02"));
    input.set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    input.set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64")); // 0x5208
    input.set_gas_limit(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonyStakingSignerSign(inputTWData);

    auto output = Proto::StakingTransactionOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "71b68b38864e75af60bf05e52b53278e864dbf2eb4a33adeacaa6e1b31f21e59";
    auto shouldBeS = "1ee06acb4d2bc22105454a79ef089fc0794ddba6e2849d9e4236180b47e973ed";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}

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

TEST(TWHarmonyStakingSigner, Redelegate) {
    Proto::StakingTransactionInput input;

    const auto privateKey =
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto redelegateMsg = input.mutable_redelegate_message();
    redelegateMsg->set_delegator_address(TEST_ACCOUNT.string());
    redelegateMsg->set_validator_src_address(TEST_ACCOUNT.string());
    redelegateMsg->set_validator_dst_address(TEST_ACCOUNT.string());

    value = store(uint256_t("0xa"));
    redelegateMsg->set_amount(value.data(), value.size());

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
    auto shouldBeR = "c479121bf1ea02fa1052a4d54743703fa6eeb16e50ff002d34fcfde736c21d75";
    auto shouldBeS = "7a1b9dac1761ab9fb38cadcdd4b0b28aafc39d1707e913f6b758e05e09b1e517";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}

TEST(TWHarmonyStakingSigner, Undelegate) {
    Proto::StakingTransactionInput input;

    const auto privateKey =
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto undelegateMsg = input.mutable_undelegate_message();
    undelegateMsg->set_delegator_address(TEST_ACCOUNT.string());
    undelegateMsg->set_validator_address(TEST_ACCOUNT.string());

    value = store(uint256_t("0xa"));
    undelegateMsg->set_amount(value.data(), value.size());

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

    auto shouldBeV = "27";
    auto shouldBeR = "d6af2488d3b45658f37ff6bb89f7eaa86f7c1dfce19a68697e778be28efd2320";
    auto shouldBeS = "5b9837bd5c7041318859f9fb444a255f32f4d7e7b49f18830ba75abecdc02390";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}
