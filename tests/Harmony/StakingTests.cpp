// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Coin.h"
#include "HDWallet.h"
#include "Harmony/Address.h"
#include "Harmony/StakingSigner.h"
#include "HexCoding.h"
#include "proto/Harmony.pb.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::Harmony {

static uint256_t LOCAL_NET = 0x2;

static auto TEST_ACCOUNT = Address("one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9");

TEST(HarmonyStaking, SignDirectiveNewValidator) {
    //
}

TEST(HarmonyStaking, SignDirectiveEditValidator) {
    //
}

TEST(HarmonyStaking, SignDirectiveDelegate) {
    auto input = Proto::StakingTransactionInput();
    const auto privateKey =
        PrivateKey(parse_hex("b578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(uint256_t("0x2"));
    input.set_chain_id(value.data(), value.size());

    // value = store(uint8_t(DirectiveDelegate))
    auto delegateMsg = input.mutable_delegate_message();
    delegateMsg->set_delegator_address(TEST_ACCOUNT.string());
    delegateMsg->set_validator_address(TEST_ACCOUNT.string());

    value = store(uint256_t("0xa"));
    delegateMsg->set_amount(value.data(), value.size());

    value = store(uint256_t("0xa"));
    input.set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    input.set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x64")); // 0x5208
    input.set_gas_limit(value.data(), value.size());

    auto proto_output = StakingSigner::sign(input);

    auto expectEncoded =
        "f87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c"
        "580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc417"
        "63dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54";

    auto v = "28";
    auto r = "2e43463bf0bfd1563d3702f301f92c15d167892e337d9831d4c2d3ba1507efbc";
    auto s = "14e5f9d9618699316c69e720af3b373bc9d3c3d97962decd13dc3b48d2a4ac75";

    // ASSERT_EQ(hex(proto_output.encoded()), expectEncoded);
    ASSERT_EQ(hex(proto_output.v()), v);
    ASSERT_EQ(hex(proto_output.r()), r);
    ASSERT_EQ(hex(proto_output.s()), s);
}

TEST(HarmonyStaking, SignDirectiveRedelegate) {
    //
}

TEST(HarmonyStaking, SignDirectiveUndelegate) {
    //
}

} // namespace TW::Harmony
