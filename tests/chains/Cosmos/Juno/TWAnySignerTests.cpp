// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

TEST(TWAnySignerJuno, Sign) {
    auto privateKey = parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(376606);
    input.set_chain_id("juno-1");
    input.set_memo("");
    input.set_sequence(0);
    input.set_private_key(privateKey.data(), privateKey.size());

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("juno1mry47pkga5tdswtluy0m8teslpalkdq0gnn4mf");
    message.set_to_address("juno1mry47pkga5tdswtluy0m8teslpalkdq0gnn4mf");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("ujuno");
    amountOfTx->set_amount("10000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(80000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("ujuno");
    amountOfFee->set_amount("1000");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeJuno);

    EXPECT_EQ(output.error(), Common::Proto::OK);

    // https://www.mintscan.io/juno/txs/3DCE6AAF19657BCF11D44FD6BE124D57B44E04CA34851DE0ECCE619F70ECC46F
    auto expectedJson = R"(
                {
                    "mode": "BROADCAST_MODE_BLOCK",
                    "tx_bytes": "Co0BCooBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmoKK2p1bm8xbXJ5NDdwa2dhNXRkc3d0bHV5MG04dGVzbHBhbGtkcTBnbm40bWYSK2p1bm8xbXJ5NDdwa2dhNXRkc3d0bHV5MG04dGVzbHBhbGtkcTBnbm40bWYaDgoFdWp1bm8SBTEwMDAwEmUKTgpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQLL/bXkcokzIilOYM8Ig9Q99DHhCJ0p7LRHqebVUEWq5RIECgIIARITCg0KBXVqdW5vEgQxMDAwEIDxBBpABrA2SUNtur1XqAIzNjM4UYtFylKARkfMd2YJUi11qqMkX0rZfmHrELL+QqjERn0o3vsR231fmPGJe4P0Isjwjw=="
                })";
    assertJSONEqual(output.serialized(), expectedJson);
    EXPECT_EQ(hex(output.signature()), "06b03649436dbabd57a80233363338518b45ca52804647cc776609522d75aaa3245f4ad97e61eb10b2fe42a8c4467d28defb11db7d5f98f1897b83f422c8f08f");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

} // namespace TW::Cosmos::tests
