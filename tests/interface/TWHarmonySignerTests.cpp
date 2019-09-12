
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "Harmony/Transaction.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "TWTestUtilities.h"
#include "proto/Harmony.pb.h"
#include "uint256.h"
#include <TrustWalletCore/TWHarmonySigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace Harmony;

static auto TEST_RECEIVER = Address("one129r9pj3sk0re76f7zs3qz92rggmdgjhtwge62k");

static uint256_t LOCAL_NET = 0x2;

TEST(TWHarmonySigner, Sign) {
    Proto::SigningInput input;

    input.set_to_address(TEST_RECEIVER.string());
    const auto privateKey =
        PrivateKey(parse_hex("4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48"));
    auto payload = parse_hex("");
    input.set_payload(payload.data(), payload.size());
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto value = store(LOCAL_NET);
    input.set_chain_id(value.data(), value.size());

    value = store(uint256_t("0x1"));
    input.set_nonce(value.data(), value.size());

    value = store(uint256_t(""));
    input.set_gas_price(value.data(), value.size());

    value = store(uint256_t("0x5208"));
    input.set_gas_limit(value.data(), value.size());

    value = store(uint256_t("0x1"));
    input.set_from_shard_id(value.data(), value.size());

    value = store(uint256_t("0x0"));
    input.set_to_shard_id(value.data(), value.size());

    value = store(uint256_t("0x6bfc8da5ee8220000"));
    input.set_amount(value.data(), value.size());

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWHarmonySignerSign(inputTWData);

    auto output = Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto shouldBeV = "28";
    auto shouldBeR = "84cc200aab11f5e1b2f7ece0d56ec67385ac50cefb6e3dc2a2f3e036ed575a5c";
    auto shouldBeS = "643f18005b790cac8d8e7dc90e6147df0b83874b52db198864694ea28a79e6fc";

    ASSERT_EQ(hex(output.v()), shouldBeV);
    ASSERT_EQ(hex(output.r()), shouldBeR);
    ASSERT_EQ(hex(output.s()), shouldBeS);
}
