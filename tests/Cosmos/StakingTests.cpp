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

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Cosmos;

TEST(CosmosStaking, Staking) {
    auto input = Proto::SigningInput();
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
    amountOfTx.set_amount(10);

    auto &fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(1018);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    ASSERT_EQ("wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgDelegate\",\"value\":{\"amount\":{\"amount\":\"10\",\"denom\":\"muon\"},\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ==\"}]}}", output.json());

    ASSERT_EQ(hex(output.signature()), "c08bdf6c2b0b4428f37975e85d329f1cb19745b000994a743b5df81d57d573aa5f755349befcc848c1d1507818723b1288594bc91df685e89aff22e0303b4861");
}

TEST(CosmosStaking, Unstaking) {
    auto input = Proto::SigningInput();
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
    amountOfTx.set_amount(10);

    auto &fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(1018);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    ASSERT_EQ("j4WpUVohGIHa6/s0bCvuyjq1wtQGqbOtQCz92qPQjisTN44Tz++Ozx1lAP6F0M4+eTA03XerqQ8hZCeAfL/3nw==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgUndelegate\",\"value\":{\"amount\":{\"amount\":\"10\",\"denom\":\"muon\"},\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"j4WpUVohGIHa6/s0bCvuyjq1wtQGqbOtQCz92qPQjisTN44Tz++Ozx1lAP6F0M4+eTA03XerqQ8hZCeAfL/3nw==\"}]}}", output.json());

    ASSERT_EQ(hex(output.signature()), "8f85a9515a211881daebfb346c2beeca3ab5c2d406a9b3ad402cfddaa3d08e2b13378e13cfef8ecf1d6500fe85d0ce3e793034dd77aba90f216427807cbff79f");
}

TEST(CosmosStaking, Restaking) {
    auto input = Proto::SigningInput();
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
    amountOfTx.set_amount(10);

    auto &fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(1018);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    ASSERT_EQ("5k03Yb0loovvzagMCg4gjQJP2woriZVRcOZaXF1FSros6B1X4B8MEm3lpZwrWBJMEJVgyYA9ZaF6FLVI3WxQ2w==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgBeginRedelegate\",\"value\":{\"amount\":{\"amount\":\"10\",\"denom\":\"muon\"},\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_dst_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_src_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"5k03Yb0loovvzagMCg4gjQJP2woriZVRcOZaXF1FSros6B1X4B8MEm3lpZwrWBJMEJVgyYA9ZaF6FLVI3WxQ2w==\"}]}}",
            output.json());

    ASSERT_EQ(hex(output.signature()), "e64d3761bd25a28befcda80c0a0e208d024fdb0a2b89955170e65a5c5d454aba2ce81d57e01f0c126de5a59c2b58124c109560c9803d65a17a14b548dd6c50db");
}

TEST(CosmosStaking, Withdraw) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_withdraw_stake_reward_message();
    message.set_delegator_address("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    message.set_validator_address("cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp");

    auto &fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(1018);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    ASSERT_EQ("VG8NZzVvavlM+1qyK5dOSZwzEj8sLCkvTw5kh44Oco9GQxBf13FVC+s/I3HwiICqo4+o8jNMEDp3nx2C0tuY1g==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgWithdrawDelegationReward\",\"value\":{\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"VG8NZzVvavlM+1qyK5dOSZwzEj8sLCkvTw5kh44Oco9GQxBf13FVC+s/I3HwiICqo4+o8jNMEDp3nx2C0tuY1g==\"}]}}", output.json());

    ASSERT_EQ(hex(output.signature()), "546f0d67356f6af94cfb5ab22b974e499c33123f2c2c292f4f0e64878e0e728f4643105fd771550beb3f2371f08880aaa38fa8f2334c103a779f1d82d2db98d6");
}

TEST(CosmosStaking, WithdrawAllRaw) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_raw_json_message();
    message.set_type("cosmos-sdk/MsgWithdrawDelegationRewardsAll");
    message.set_value("{\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\"}");
    auto &fee = *input.mutable_fee();
    fee.set_gas(101721);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(1018);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Cosmos::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    ASSERT_EQ("ImvsgnfbjebxzeBCUPeOcMoOJWMV3IhWM1apV20WiS4K11iA50fe0uXr4Xf/RTxUDXTm56cne/OjOr77BG99Aw==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgWithdrawDelegationRewardsAll\",\"value\":{\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"ImvsgnfbjebxzeBCUPeOcMoOJWMV3IhWM1apV20WiS4K11iA50fe0uXr4Xf/RTxUDXTm56cne/OjOr77BG99Aw==\"}]}}", output.json());

    ASSERT_EQ(hex(output.signature()), "226bec8277db8de6f1cde04250f78e70ca0e256315dc88563356a9576d16892e0ad75880e747ded2e5ebe177ff453c540d74e6e7a7277bf3a33abefb046f7d03");
}
