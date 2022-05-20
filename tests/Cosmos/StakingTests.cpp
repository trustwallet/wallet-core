// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Cosmos;

TEST(CosmosStaking, Staking) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_stake_message();
    message.set_delegator_address("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    message.set_validator_address("cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp");
    auto& amountOfTx = *message.mutable_amount();
    amountOfTx.set_denom("muon");
    amountOfTx.set_amount("10");

    auto& fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("1018");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    assertJSONEqual(output.serialized(), "{\"tx_bytes\":\"CpsBCpgBCiMvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dEZWxlZ2F0ZRJxCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISNGNvc21vc3ZhbG9wZXIxemt1cHI4M2hyemtuM3VwNWVsa3R6Y3EzdHVmdDhueHNtd2RxZ3AaCgoEbXVvbhICMTASZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAcSEgoMCgRtdW9uEgQxMDE4ENmaBhpA8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA==\",\"mode\":\"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "f0ef499bf90be996b6237a680ece6fa4ca3060980dbd808905153fbf1023b3494d658b2ae34aa94dbc0e4db3918c903952343a6ae738d2feae0854f8ab8cfeb8");
    EXPECT_EQ(output.error(), "");

    { // Json-serialization, for coverage (to be removed later)
        input.set_signing_mode(Proto::JSON);
        auto output = Signer::sign(input, TWCoinTypeCosmos);
        ASSERT_EQ(hex(output.signature()), "c08bdf6c2b0b4428f37975e85d329f1cb19745b000994a743b5df81d57d573aa5f755349befcc848c1d1507818723b1288594bc91df685e89aff22e0303b4861");
        EXPECT_EQ(output.error(), "");
        EXPECT_EQ(hex(output.serialized()), "");
    }
}

TEST(CosmosStaking, Unstaking) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_unstake_message();
    message.set_delegator_address("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    message.set_validator_address("cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp");
    auto& amountOfTx = *message.mutable_amount();
    amountOfTx.set_denom("muon");
    amountOfTx.set_amount("10");

    auto& fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("1018");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    assertJSONEqual(output.serialized(), "{\"tx_bytes\":\"Cp0BCpoBCiUvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dVbmRlbGVnYXRlEnEKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhI0Y29zbW9zdmFsb3BlcjF6a3VwcjgzaHJ6a24zdXA1ZWxrdHpjcTN0dWZ0OG54c213ZHFncBoKCgRtdW9uEgIxMBJmClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECVyhuw/N9M1V7u6oACyd0SskCOqmWfK51oYHR/5H6ncUSBAoCCAEYBxISCgwKBG11b24SBDEwMTgQ2ZoGGkBhlxHFnjBERxLtjLbMCKXcrDctaSZ9djtWCa3ely1bpV6m+6aAFjpr8aEZH+q2AtjJSEdgpQRJxP+9/gQsRTnZ\",\"mode\":\"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "619711c59e30444712ed8cb6cc08a5dcac372d69267d763b5609adde972d5ba55ea6fba680163a6bf1a1191feab602d8c9484760a50449c4ffbdfe042c4539d9");
    EXPECT_EQ(output.error(), "");

    { // Json-serialization, for coverage (to be removed later)
        input.set_signing_mode(Proto::JSON);
        auto output = Signer::sign(input, TWCoinTypeCosmos);
        ASSERT_EQ(hex(output.signature()), "8f85a9515a211881daebfb346c2beeca3ab5c2d406a9b3ad402cfddaa3d08e2b13378e13cfef8ecf1d6500fe85d0ce3e793034dd77aba90f216427807cbff79f");
        EXPECT_EQ(output.error(), "");
        EXPECT_EQ(hex(output.serialized()), "");
    }
}

TEST(CosmosStaking, Restaking) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_restake_message();
    message.set_delegator_address("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    message.set_validator_dst_address("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    message.set_validator_src_address("cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp");

    auto& amountOfTx = *message.mutable_amount();
    amountOfTx.set_denom("muon");
    amountOfTx.set_amount("10");

    auto& fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("1018");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    assertJSONEqual(output.serialized(), "{\"tx_bytes\":\"CtIBCs8BCiovY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dCZWdpblJlZGVsZWdhdGUSoAEKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhI0Y29zbW9zdmFsb3BlcjF6a3VwcjgzaHJ6a24zdXA1ZWxrdHpjcTN0dWZ0OG54c213ZHFncBotY29zbW9zMWhzazZqcnl5cWpmaHA1ZGhjNTV0YzlqdGNreWd4MGVwaDZkZDAyIgoKBG11b24SAjEwEmYKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQJXKG7D830zVXu7qgALJ3RKyQI6qZZ8rnWhgdH/kfqdxRIECgIIARgHEhIKDAoEbXVvbhIEMTAxOBDZmgYaQJ52qO5xdtBkNUeFeWrnqUXkngyHFKCXnOPPClyVI0HrULdp5jbwGra2RujEOn4BrbFCb3JFnpc2o1iuLXbKQxg=\",\"mode\":\"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "9e76a8ee7176d064354785796ae7a945e49e0c8714a0979ce3cf0a5c952341eb50b769e636f01ab6b646e8c43a7e01adb1426f72459e9736a358ae2d76ca4318");
    EXPECT_EQ(output.error(), "");

    { // Json-serialization, for coverage (to be removed later)
        input.set_signing_mode(Proto::JSON);
        auto output = Signer::sign(input, TWCoinTypeCosmos);
        ASSERT_EQ(hex(output.signature()), "e64d3761bd25a28befcda80c0a0e208d024fdb0a2b89955170e65a5c5d454aba2ce81d57e01f0c126de5a59c2b58124c109560c9803d65a17a14b548dd6c50db");
        EXPECT_EQ(output.error(), "");
        EXPECT_EQ(hex(output.serialized()), "");
    }
}

TEST(CosmosStaking, Withdraw) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_withdraw_stake_reward_message();
    message.set_delegator_address("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    message.set_validator_address("cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp");

    auto& fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("1018");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    assertJSONEqual(output.serialized(), "{\"tx_bytes\":\"CqMBCqABCjcvY29zbW9zLmRpc3RyaWJ1dGlvbi52MWJldGExLk1zZ1dpdGhkcmF3RGVsZWdhdG9yUmV3YXJkEmUKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhI0Y29zbW9zdmFsb3BlcjF6a3VwcjgzaHJ6a24zdXA1ZWxrdHpjcTN0dWZ0OG54c213ZHFncBJmClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECVyhuw/N9M1V7u6oACyd0SskCOqmWfK51oYHR/5H6ncUSBAoCCAEYBxISCgwKBG11b24SBDEwMTgQ2ZoGGkBW1Cd+0pNfMPEVXQtqG1VIijDjZP2UOiDlvUF478axnxlF8PaOAsY0S5OdUE3Wz7+nu8YVmrLZQS/8mlqLaK05\",\"mode\":\"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "56d4277ed2935f30f1155d0b6a1b55488a30e364fd943a20e5bd4178efc6b19f1945f0f68e02c6344b939d504dd6cfbfa7bbc6159ab2d9412ffc9a5a8b68ad39");
    EXPECT_EQ(output.error(), "");

    { // Json-serialization, for coverage (to be removed later)
        input.set_signing_mode(Proto::JSON);
        auto output = Signer::sign(input, TWCoinTypeCosmos);
        ASSERT_EQ(hex(output.signature()), "546f0d67356f6af94cfb5ab22b974e499c33123f2c2c292f4f0e64878e0e728f4643105fd771550beb3f2371f08880aaa38fa8f2334c103a779f1d82d2db98d6");
        EXPECT_EQ(output.error(), "");
        EXPECT_EQ(hex(output.serialized()), "");
    }
}
