// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

TEST(TWAnySignerStargaze, SignNftTransferCW721) {
    auto privateKey = parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(188393);
    input.set_chain_id("stargaze-1");
    input.set_memo("");
    input.set_sequence(5);
    input.set_private_key(privateKey.data(), privateKey.size());

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_execute_contract_generic();

    const auto tokenContractAddress = "stars14gmjlyfz5mpv5d8zrksn0tjhz2wwvdc4yk06754alfasq9qen7fsknry42";
    const auto txMessage = R"({"transfer_nft": {"recipient": "stars1kd5q7qejlqz94kpmd9pvr4v2gzgnca3lvt6xnp","token_id": "1209"}})";

    message.set_sender_address("stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy");
    message.set_contract_address(tokenContractAddress);
    message.set_execute_msg(txMessage);

    auto& fee = *input.mutable_fee();
    fee.set_gas(666666);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("ustars");
    amountOfFee->set_amount("1000");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStargaze);

    // https://www.mintscan.io/stargaze/txs/300836A5BF9002CF38EE34A8C56E8E7E6854FA64F1DEB3AE108F381A48150F7C
    auto expectedJson = R"(
                {
                    "mode":"BROADCAST_MODE_BLOCK",
                    "tx_bytes":"CoACCv0BCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QS1AEKLHN0YXJzMW1yeTQ3cGtnYTV0ZHN3dGx1eTBtOHRlc2xwYWxrZHEwMmE4bmh5EkBzdGFyczE0Z21qbHlmejVtcHY1ZDh6cmtzbjB0amh6Mnd3dmRjNHlrMDY3NTRhbGZhc3E5cWVuN2Zza25yeTQyGmJ7InRyYW5zZmVyX25mdCI6IHsicmVjaXBpZW50IjogInN0YXJzMWtkNXE3cWVqbHF6OTRrcG1kOXB2cjR2Mmd6Z25jYTNsdnQ2eG5wIiwidG9rZW5faWQiOiAiMTIwOSJ9fRJoClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECy/215HKJMyIpTmDPCIPUPfQx4QidKey0R6nm1VBFquUSBAoCCAEYBRIUCg4KBnVzdGFycxIEMTAwMBCq2CgaQMx+l2sdM5DAPbDyY1p173MLnjGyNWIcRmaFiVNphLuTV3tjhwPbsXEA0hyRxyWS3vN0/xUF/JEsO9wRspj2aJ4="
                })";
    assertJSONEqual(output.serialized(), expectedJson);
    EXPECT_EQ(hex(output.signature()), "cc7e976b1d3390c03db0f2635a75ef730b9e31b235621c46668589536984bb93577b638703dbb17100d21c91c72592def374ff1505fc912c3bdc11b298f6689e");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

TEST(TWAnySignerStargaze, Sign) {
    auto privateKey = parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(188393);
    input.set_chain_id("stargaze-1");
    input.set_memo("");
    input.set_sequence(0);
    input.set_private_key(privateKey.data(), privateKey.size());

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy");
    message.set_to_address("stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("ustars");
    amountOfTx->set_amount("10000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(80000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("ustars");
    amountOfFee->set_amount("1000");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStargaze);

    // https://www.mintscan.io/stargaze/txs/98D5E36CA7080DDB286FE924A5A9976ABD4EBE49C92A09D322F29AD30DE4BE4D
    auto expectedJson = R"(
                {
                    "mode":"BROADCAST_MODE_BLOCK",
                    "tx_bytes":"CpABCo0BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm0KLHN0YXJzMW1yeTQ3cGtnYTV0ZHN3dGx1eTBtOHRlc2xwYWxrZHEwMmE4bmh5EixzdGFyczFtcnk0N3BrZ2E1dGRzd3RsdXkwbTh0ZXNscGFsa2RxMDJhOG5oeRoPCgZ1c3RhcnMSBTEwMDAwEmYKTgpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQLL/bXkcokzIilOYM8Ig9Q99DHhCJ0p7LRHqebVUEWq5RIECgIIARIUCg4KBnVzdGFycxIEMTAwMBCA8QQaQHAkntxzC1oH7Yde4+KEmnB+K3XbJIYw0q6MqMPEY65YAwBDNDOdaTu/rpehus/20MvBfbAEZiw9+whzXLpkQ5A="
                })";
    assertJSONEqual(output.serialized(), expectedJson);
    EXPECT_EQ(hex(output.signature()), "70249edc730b5a07ed875ee3e2849a707e2b75db248630d2ae8ca8c3c463ae5803004334339d693bbfae97a1bacff6d0cbc17db004662c3dfb08735cba644390");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

} // namespace TW::Cosmos::tests
