// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Coin.h"
#include "HDWallet.h"
#include "Harmony/Address.h"
#include "Harmony/Signer.h"
#include "HexCoding.h"
#include "proto/Harmony.pb.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::Harmony {

static uint256_t LOCAL_NET = 0x2;

static Address TEST_ACCOUNT;
static bool testAccountDecodeResult =
    Address::decode("one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9", TEST_ACCOUNT);

static auto PRIVATE_KEY =
    PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));

TEST(HarmonyStaking, SignCreateValidator) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto createValidatorMsg = stakingMessage->mutable_create_validator_message();

    createValidatorMsg->set_validator_address(TEST_ACCOUNT.string());

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

    auto proto_output = Signer::sign(input);

    auto expectEncoded =
        "f8ed80f8a494ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c6963"
        "652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121ddc988016345"
        "785d8a0000c9880c7d713b49da0000c887b1a2bc2ec500000a820bb8f1b0b9486167ab9087ab818dc4ce026edb"
        "5bf216863364c32e42df2af03c5ced1ad181e7d12f0e6dd5307a73b622476086116402806428a0476e8a0fe478"
        "e0d03ff10222d4d590bca8cee3ec51b830f4fc4a8bee5d0e9d28a03b2be18e73b2f99d7e2691485a0e166f28e6"
        "2815079c126e68f876dc97339f8f";

    auto v = "28";
    auto r = "476e8a0fe478e0d03ff10222d4d590bca8cee3ec51b830f4fc4a8bee5d0e9d28";
    auto s = "3b2be18e73b2f99d7e2691485a0e166f28e62815079c126e68f876dc97339f8f";

    ASSERT_EQ(hex(proto_output.encoded()), expectEncoded);
    ASSERT_EQ(hex(proto_output.v()), v);
    ASSERT_EQ(hex(proto_output.r()), r);
    ASSERT_EQ(hex(proto_output.s()), s);
}

TEST(HarmonyStaking, SignEditValidator) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto editValidatorMsg = stakingMessage->mutable_edit_validator_message();

    editValidatorMsg->set_validator_address(TEST_ACCOUNT.string());

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

    value = store(uint256_t("0x64")); // 0x5208
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto proto_output = Signer::sign(input);

    auto expectEncoded =
        "f9010801f8bf94ebcd16e8c1d8f493ba04e99a56474122d81a9c58f83885416c69636585616c69636591616c"
        "6963652e6861726d6f6e792e6f6e6583426f6295446f6e2774206d6573732077697468206d65212121c9880163"
        "45785d8a00000a820bb8b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced1ad181e7"
        "d12f0e6dd5307a73b62247608611b0b9486167ab9087ab818dc4ce026edb5bf216863364c32e42df2af03c5ced"
        "1ad181e7d12f0e6dd5307a73b6224760861102806427a05e54b55272f6bf5ffeca10d85976749d6b844cc9f30b"
        "a3285b9ab8a82d53e3e3a03ce04d9a9f834e20b22aa918ead346c84a04b1504fe3ff9e38f21c5e5712f013";

    auto v = "27";
    auto r = "5e54b55272f6bf5ffeca10d85976749d6b844cc9f30ba3285b9ab8a82d53e3e3";
    auto s = "3ce04d9a9f834e20b22aa918ead346c84a04b1504fe3ff9e38f21c5e5712f013";

    ASSERT_EQ(hex(proto_output.encoded()), expectEncoded);
    ASSERT_EQ(hex(proto_output.v()), v);
    ASSERT_EQ(hex(proto_output.r()), r);
    ASSERT_EQ(hex(proto_output.s()), s);
}

TEST(HarmonyStaking, SignDelegate) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto delegateMsg = stakingMessage->mutable_delegate_message();
    delegateMsg->set_delegator_address(TEST_ACCOUNT.string());
    delegateMsg->set_validator_address(TEST_ACCOUNT.string());

    value = store(uint256_t("0xa"));
    delegateMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0x02"));
    stakingMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    stakingMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto proto_output = Signer::sign(input);

    auto expectEncoded =
        "f87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a"
        "9c580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc4"
        "1763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54";

    auto v = "28";
    auto r = "ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80";
    auto s = "5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54";

    ASSERT_EQ(hex(proto_output.encoded()), expectEncoded);
    ASSERT_EQ(hex(proto_output.v()), v);
    ASSERT_EQ(hex(proto_output.r()), r);
    ASSERT_EQ(hex(proto_output.s()), s);
}

TEST(HarmonyStaking, SignUndelegate) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto undelegateMsg = stakingMessage->mutable_undelegate_message();
    undelegateMsg->set_delegator_address(TEST_ACCOUNT.string());
    undelegateMsg->set_validator_address(TEST_ACCOUNT.string());

    value = store(uint256_t("0xa"));
    undelegateMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0x02"));
    stakingMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    stakingMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto proto_output = Signer::sign(input);

    auto expectEncoded =
        "f87303eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c"
        "580a02806428a05bf8c653567defe2c3728732bc9d67dd099a977df91c740a883fd89e03abb6e2a05202c4b516"
        "52d5144c6a30d14d1a7a316b5a4a6b49be985b4bc6980e49f7acb7";

    auto v = "28";
    auto r = "5bf8c653567defe2c3728732bc9d67dd099a977df91c740a883fd89e03abb6e2";
    auto s = "5202c4b51652d5144c6a30d14d1a7a316b5a4a6b49be985b4bc6980e49f7acb7";

    ASSERT_EQ(hex(proto_output.encoded()), expectEncoded);
    ASSERT_EQ(hex(proto_output.v()), v);
    ASSERT_EQ(hex(proto_output.r()), r);
    ASSERT_EQ(hex(proto_output.s()), s);
}

TEST(HarmonyStaking, SignCollectRewards) {
    auto input = Proto::SigningInput();
    input.set_private_key(PRIVATE_KEY.bytes.data(), PRIVATE_KEY.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    auto stakingMessage = input.mutable_staking_message();
    auto collectRewardsMsg = stakingMessage->mutable_collect_rewards();
    collectRewardsMsg->set_delegator_address(TEST_ACCOUNT.string());

    value = store(uint256_t("0x02"));
    stakingMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t("0x0"));
    stakingMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64"));
    stakingMessage->set_gas_limit(value.data(), value.size());

    auto proto_output = Signer::sign(input);

    auto expectEncoded = "f85d04d594ebcd16e8c1d8f493ba04e99a56474122d81a9c5802806428a04c15c72f425"
                         "77001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625a055c13ea17c3efd1cd9"
                         "1f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d";

    auto v = "28";
    auto r = "4c15c72f42577001083a9c7ff9d9724077aec704a524e53dc7c9afe97ca4e625";
    auto s = "55c13ea17c3efd1cd91f2988c7e7673950bac5a08c174f2d0af27a82039f1e3d";

    ASSERT_EQ(hex(proto_output.encoded()), expectEncoded);
    ASSERT_EQ(hex(proto_output.v()), v);
    ASSERT_EQ(hex(proto_output.r()), r);
    ASSERT_EQ(hex(proto_output.s()), s);
}

} // namespace TW::Harmony
