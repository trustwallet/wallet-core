// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::Cosmos {

TEST(CosmosStaking, Staking) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto& message = *input.mutable_stake_message();
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

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgDelegate\",\"value\":{\"amount\":{\"amount\":\"10\",\"denom\":\"muon\"},\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

    ASSERT_EQ(hex(output.signature()), "c08bdf6c2b0b4428f37975e85d329f1cb19745b000994a743b5df81d57d573aa5f755349befcc848c1d1507818723b1288594bc91df685e89aff22e0303b4861");
}

TEST(CosmosStaking, Unstaking) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto& message = *input.mutable_unstake_message();
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

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgUndelegate\",\"value\":{\"amount\":{\"amount\":\"10\",\"denom\":\"muon\"},\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"j4WpUVohGIHa6/s0bCvuyjq1wtQGqbOtQCz92qPQjisTN44Tz++Ozx1lAP6F0M4+eTA03XerqQ8hZCeAfL/3nw==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

    ASSERT_EQ(hex(output.signature()), "8f85a9515a211881daebfb346c2beeca3ab5c2d406a9b3ad402cfddaa3d08e2b13378e13cfef8ecf1d6500fe85d0ce3e793034dd77aba90f216427807cbff79f");
}

TEST(CosmosStaking, Withdraw) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(7);

    auto& message = *input.mutable_withdraw_stake_reward_message();
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

    ASSERT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgWithdrawDelegationReward\",\"value\":{\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"VG8NZzVvavlM+1qyK5dOSZwzEj8sLCkvTw5kh44Oco9GQxBf13FVC+s/I3HwiICqo4+o8jNMEDp3nx2C0tuY1g==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

    ASSERT_EQ(hex(output.signature()), "546f0d67356f6af94cfb5ab22b974e499c33123f2c2c292f4f0e64878e0e728f4643105fd771550beb3f2371f08880aaa38fa8f2334c103a779f1d82d2db98d6");
    }

}
