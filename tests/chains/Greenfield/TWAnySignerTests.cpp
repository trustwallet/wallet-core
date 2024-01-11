// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "proto/Greenfield.pb.h"
#include "TestUtilities.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

namespace TW::Greenfield::tests {

TEST(TWAnySignerGreenfield, Sign) {
    // Successfully broadcasted: https://greenfieldscan.com/tx/ED8508F3C174C4430B8EE718A6D6F0B02A8C516357BE72B1336CF74356529D19

    Proto::SigningInput input;
    input.set_signing_mode(Proto::Eip712);
    input.set_account_number(15952);
    input.set_cosmos_chain_id("greenfield_5600-1");
    input.set_eth_chain_id("5600");
    input.set_sequence(0);
    input.set_mode(Proto::BroadcastMode::SYNC);
    input.set_memo("Trust Wallet test memo");

    auto &msg = *input.add_messages();
    auto &msgSend = *msg.mutable_send_coins_message();
    msgSend.set_from_address("0xA815ae0b06dC80318121745BE40e7F8c6654e9f3");
    msgSend.set_to_address("0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0");
    auto amountOfTx = msgSend.add_amounts();
    amountOfTx->set_denom("BNB");
    amountOfTx->set_amount("1234500000000000");

    auto &fee = *input.mutable_fee();
    fee.set_gas(1200);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("BNB");
    amountOfFee->set_amount("6000000000000");

    auto privateKey = parse_hex("825d2bb32965764a98338139412c7591ed54c951dd65504cd8ddaeaa0fea7b2a");
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeGreenfield);

    EXPECT_EQ(output.error(), Common::Proto::SigningError::OK);
    EXPECT_EQ(hex(output.signature()), "c1b45bd6a1005b06aa55f9a9d4c9fb88c8bbc3057fa0f8b6276796f4d04874da24cbe64bfae7a04bf918f9fba708eaea559f8a6e897dfdd8c057e6d068d501d31c");
    EXPECT_EQ(output.serialized(), R"({"mode":"BROADCAST_MODE_SYNC","tx_bytes":"CqwBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4QTgxNWFlMGIwNmRDODAzMTgxMjE3NDVCRTQwZTdGOGM2NjU0ZTlmMxIqMHg4ZGJENmM3RWRlOTA2NDZhNjFCYmM2NDk4MzFiN2MyOThCRmQzN0EwGhcKA0JOQhIQMTIzNDUwMDAwMDAwMDAwMBIWVHJ1c3QgV2FsbGV0IHRlc3QgbWVtbxJzClYKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAhm/mQgs8vzaqBLW66HrqQNv86PYTBgXyElU1OiuKD/sEgUKAwjIBRIZChQKA0JOQhINNjAwMDAwMDAwMDAwMBCwCRpBwbRb1qEAWwaqVfmp1Mn7iMi7wwV/oPi2J2eW9NBIdNoky+ZL+uegS/kY+funCOrqVZ+Kbol9/djAV+bQaNUB0xw="})");
}

} // namespace TW::Greenfield::tests
