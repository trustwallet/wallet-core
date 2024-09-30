// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include "Base64.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

TEST(TWAnySignerStride, SignLiquidStaking) {
    auto privateKey = parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(136412);
    input.set_chain_id("stride-1");
    input.set_memo("");
    input.set_sequence(0);
    input.set_private_key(privateKey.data(), privateKey.size());

    auto msg = input.add_messages();
    auto& message = *msg->mutable_msg_stride_liquid_staking_stake();
    message.set_creator("stride1mry47pkga5tdswtluy0m8teslpalkdq0a2sjge");
    message.set_amount("100000");
    message.set_host_denom("uatom");

    auto& fee = *input.mutable_fee();
    fee.set_gas(500000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("ustrd");
    amountOfFee->set_amount("0");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStride);

    // Successfully broadcasted: https://www.mintscan.io/stride/txs/48E51A2571D99453C4581B30CECA2A1156C0D1EBACCD3619729B5A35AD67CC94?height=3485243
    auto expectedJson = R"(
                {
                    "mode":"BROADCAST_MODE_BLOCK",
                    "tx_bytes":"CmMKYQofL3N0cmlkZS5zdGFrZWliYy5Nc2dMaXF1aWRTdGFrZRI+Ci1zdHJpZGUxbXJ5NDdwa2dhNXRkc3d0bHV5MG04dGVzbHBhbGtkcTBhMnNqZ2USBjEwMDAwMBoFdWF0b20SYgpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAsv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarlEgQKAggBEhAKCgoFdXN0cmQSATAQoMIeGkCDaZHV5/Z3CAQC5DXkaHmF6OKUiS5XKDsl3ZnBaaVuJjlSWV2vA7MPwGbC17P6jbVJt58ZLcxIWFt76UO3y1ix"
                })";
    assertJSONEqual(output.serialized(), expectedJson);
    EXPECT_EQ(hex(output.signature()), "836991d5e7f677080402e435e4687985e8e294892e57283b25dd99c169a56e263952595daf03b30fc066c2d7b3fa8db549b79f192dcc48585b7be943b7cb58b1");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

TEST(TWAnySignerStride, SignLiquidStakingRedeem) {
    auto privateKey = parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(136412);
    input.set_chain_id("stride-1");
    input.set_memo("");
    input.set_sequence(1);
    input.set_private_key(privateKey.data(), privateKey.size());

    auto msg = input.add_messages();
    auto& message = *msg->mutable_msg_stride_liquid_staking_redeem();
    message.set_creator("stride1mry47pkga5tdswtluy0m8teslpalkdq0a2sjge");
    message.set_amount("40000");
    message.set_receiver("cosmos1mry47pkga5tdswtluy0m8teslpalkdq07pswu4");
    message.set_host_zone("cosmoshub-4");

    auto& fee = *input.mutable_fee();
    fee.set_gas(1000000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("ustrd");
    amountOfFee->set_amount("0");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStride);

    // Successfully broadcasted: https://www.mintscan.io/stride/txs/B3D3A92A2FFB92A480A4B547A4303E6932204972A965D687DB4FB6B4E16B2C42?height=3485343
    auto expectedJson = R"(
                {
                    "mode":"BROADCAST_MODE_BLOCK",
                    "tx_bytes":"CpgBCpUBCh8vc3RyaWRlLnN0YWtlaWJjLk1zZ1JlZGVlbVN0YWtlEnIKLXN0cmlkZTFtcnk0N3BrZ2E1dGRzd3RsdXkwbTh0ZXNscGFsa2RxMGEyc2pnZRIFNDAwMDAaC2Nvc21vc2h1Yi00Ii1jb3Ntb3MxbXJ5NDdwa2dhNXRkc3d0bHV5MG04dGVzbHBhbGtkcTA3cHN3dTQSZApQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAsv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarlEgQKAggBGAESEAoKCgV1c3RyZBIBMBDAhD0aQKf84TYoPqwnXw22r0dok2fYplUFu003TlIfpoT+wqTZF1lHPC+RTAoJob6x50CnfvGlgJFBEQYPD+Ccv659VVA="
                })";
    assertJSONEqual(output.serialized(), expectedJson);
    EXPECT_EQ(TW::Base64::encode(data(output.signature())), "p/zhNig+rCdfDbavR2iTZ9imVQW7TTdOUh+mhP7CpNkXWUc8L5FMCgmhvrHnQKd+8aWAkUERBg8P4Jy/rn1VUA==");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

} // namespace TW::Cosmos::tests
