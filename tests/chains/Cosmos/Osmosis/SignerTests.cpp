// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "proto/Cosmos.pb.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

TEST(OsmosisSigner, SignTransfer_81B4) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(124703);
    input.set_chain_id("osmosis-1");
    input.set_memo("");
    input.set_sequence(0);

    Address fromAddress;
    Address toAddress;
    EXPECT_TRUE(Address::decode("osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5", fromAddress));
    EXPECT_TRUE(Address::decode("osmo18s0hdnsllgcclweu9aymw4ngktr2k0rkvn7jmn", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("uosmo");
    amountOfTx->set_amount("99800");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uosmo");
    amountOfFee->set_amount("200");

    auto privateKey = parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeOsmosis);

    // https://www.mintscan.io/osmosis/txs/81B4F01BDE72AF7FF4536E5D7E66EB218E9FC9ACAA7C5EB5DB237DD0595D5F5F
    // curl -H 'Content-Type: application/json' --data-binary '{"tx_bytes": "Co0B...rYVj", "mode": "BROADCAST_MODE_BLOCK"}' https://lcd-osmosis.keplr.app/cosmos/tx/v1beta1/txs

    assertJSONEqual(output.serialized(), "{\"tx_bytes\":\"Co0BCooBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmoKK29zbW8xbWt5Njljbjhla3R3eTA4NDV2ZWM5dXBzZHBoa3R4dDBlbjk3ZjUSK29zbW8xOHMwaGRuc2xsZ2NjbHdldTlheW13NG5na3RyMmswcmt2bjdqbW4aDgoFdW9zbW8SBTk5ODAwEmQKTgpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQLs71zkN6MCxn+VRo3ksx826RH0Z9fmpStBweE+HVY2SRIECgIIARISCgwKBXVvc21vEgMyMDAQwJoMGkAMY//Md5GRUR4lVZhk558hFS3kii9QZYoYKfg4+ac/xgNeyoiEweVDhcmEvlH1orVwjLUOnYs4ly2a/yIurYVj\",\"mode\":\"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "0c63ffcc779191511e25559864e79f21152de48a2f50658a1829f838f9a73fc6035eca8884c1e54385c984be51f5a2b5708cb50e9d8b38972d9aff222ead8563");
    EXPECT_EQ(output.error_message(), "");
}

} // namespace TW::Cosmos::tests
