
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"
#include "HexCoding.h"
#include "proto/Icon.pb.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Icon::tests {

TEST(TWAnySignerIcon, Sign) {
    auto key = parse_hex("2d42994b2f7735bbc93a3e64381864d06747e574aa94655c516f9ad0a74eed79");
    auto input = Proto::SigningInput();

    input.set_from_address("hxbe258ceb872e08851f1f59694dac2558708ece11");
    input.set_to_address("hx5bfdb090f43a808005ffc27c25b213145e80b7cd");

    auto value = uint256_t(1000000000000000000);
    auto valueData = store(value);
    input.set_value(valueData.data(), valueData.size());

    auto stepLimit = uint256_t("74565");
    auto stepLimitData = store(stepLimit);
    input.set_step_limit(stepLimitData.data(), stepLimitData.size());

    auto one = uint256_t("01");
    auto oneData = store(one);
    input.set_network_id(oneData.data(), oneData.size());
    input.set_nonce(oneData.data(), oneData.size());

    input.set_timestamp(1516942975500598);
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeICON);

    auto expected = std::string("{\"from\":\"hxbe258ceb872e08851f1f59694dac2558708ece11\",\"nid\":\"0x1\",\"nonce\":\"0x1\",\"signature\":\"xR6wKs+IA+7E91bT8966jFKlK5mayutXCvayuSMCrx9KB7670CsWa0B7LQzgsxU0GLXaovlAT2MLs1XuDiSaZQE=\",\"stepLimit\":\"0x12345\",\"timestamp\":\"0x563a6cf330136\",\"to\":\"hx5bfdb090f43a808005ffc27c25b213145e80b7cd\",\"value\":\"0xde0b6b3a7640000\",\"version\":\"0x3\"}");
    ASSERT_EQ(output.encoded(), expected);
}

} // namespace TW::Icon::tests
