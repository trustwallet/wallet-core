
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "proto/Harmony.pb.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Harmony::tests {

static auto TEST_RECEIVER = "one129r9pj3sk0re76f7zs3qz92rggmdgjhtwge62k";

static uint256_t LOCAL_NET = 0x2;

TEST(TWAnySignerHarmony, Sign) {
    Proto::SigningInput input;

    auto transactionMessage = input.mutable_transaction_message();
    transactionMessage->set_to_address(TEST_RECEIVER);
    const auto privateKey = parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48");

    input.set_private_key(privateKey.data(), privateKey.size());

    auto value = store(LOCAL_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t("0x1"));
    transactionMessage->set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    transactionMessage->set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    transactionMessage->set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x1"));
    transactionMessage->set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    transactionMessage->set_to_shard_id(value.data(), value.size());

    value = store(uint256_t("0x6bfc8da5ee8220000"));
    transactionMessage->set_amount(value.data(), value.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeHarmony);

    auto shouldBeV = "28";
    auto shouldBeR = "84cc200aab11f5e1b2f7ece0d56ec67385ac50cefb6e3dc2a2f3e036ed575a5c";
    auto shouldBeS = "643f18005b790cac8d8e7dc90e6147df0b83874b52db198864694ea28a79e6fc";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);

    ASSERT_EQ(hex(output.encoded()), "f86a0180825208018094514650ca30b3c79f693e14220115434236d44aeb8906bfc8da5ee82200008028a084cc200aab11f5e1b2f7ece0d56ec67385ac50cefb6e3dc2a2f3e036ed575a5ca0643f18005b790cac8d8e7dc90e6147df0b83874b52db198864694ea28a79e6fc");
}

TEST(TWAnySignerHarmony, SignJSON) {
    auto json = STRING(R"({"chainId":"Ag==","transactionMessage":{"nonce":"AQ==","gasPrice":"AA==","gasLimit":"Ugg=","toAddress":"one129r9pj3sk0re76f7zs3qz92rggmdgjhtwge62k","amount":"Br/I2l7oIgAA","fromShardId":"AQ==","toShardId":"AA=="}})");
    auto key = DATA("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48");
    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeHarmony));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeHarmony));
    assertStringsEqual(result, "f86a0180825208018094514650ca30b3c79f693e14220115434236d44aeb8906bfc8da5ee82200008028a084cc200aab11f5e1b2f7ece0d56ec67385ac50cefb6e3dc2a2f3e036ed575a5ca0643f18005b790cac8d8e7dc90e6147df0b83874b52db198864694ea28a79e6fc");
}

} // namespace TW::Harmony::tests
