// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
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
    auto signatureInBase64 = signer.signInBase64();

    ASSERT_EQ("wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgDelegate\",\"value\":{\"amount\":{\"amount\":\"10\",\"denom\":\"muon\"},\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"wIvfbCsLRCjzeXXoXTKfHLGXRbAAmUp0O134HVfVc6pfdVNJvvzISMHRUHgYcjsSiFlLyR32heia/yLgMDtIYQ==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

    ASSERT_EQ("a1627478a563666565a266616d6f756e7481a266616d6f756e7464313031386564656e6f6d646d756f6e6367617366313031373231646d656d6f60636d736781a2647479706576636f736d6f732d73646b2f4d736744656c65676174656576616c7565a366616d6f756e74a266616d6f756e746231306564656e6f6d646d756f6e7164656c656761746f725f61646472657373782d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430327176616c696461746f725f616464726573737834636f736d6f7376616c6f706572317a6b757072383368727a6b6e33757035656c6b747a63713374756674386e78736d77647167706a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416c636f6273507a66544e56653775714141736e6445724a416a71706c6e79756461474230662b522b703346697369676e61747572657858774976666243734c52436a7a6558586f58544b66484c4758526241416d5570304f313334485666566336706664564e4a76767a49534d485255486759636a735369466c4c79523332686569612f794c674d44744959513d3d647479706572636f736d6f732d73646b2f4d736753656e64", hex(output.encoded()));

    /*
        the sample tx on testnet
        https://hubble.figment.network/chains/gaia-13003/blocks/125922/transactions/AAE5E18516DC8B0EF864F91B9531AB63B2248E3FC9058B6A330AE79EF1B4120A?format=json
    */
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
    auto signatureInBase64 = signer.signInBase64();

    ASSERT_EQ("j4WpUVohGIHa6/s0bCvuyjq1wtQGqbOtQCz92qPQjisTN44Tz++Ozx1lAP6F0M4+eTA03XerqQ8hZCeAfL/3nw==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgUndelegate\",\"value\":{\"amount\":{\"amount\":\"10\",\"denom\":\"muon\"},\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"j4WpUVohGIHa6/s0bCvuyjq1wtQGqbOtQCz92qPQjisTN44Tz++Ozx1lAP6F0M4+eTA03XerqQ8hZCeAfL/3nw==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

    ASSERT_EQ("a1627478a563666565a266616d6f756e7481a266616d6f756e7464313031386564656e6f6d646d756f6e6367617366313031373231646d656d6f60636d736781a264747970657818636f736d6f732d73646b2f4d7367556e64656c65676174656576616c7565a366616d6f756e74a266616d6f756e746231306564656e6f6d646d756f6e7164656c656761746f725f61646472657373782d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430327176616c696461746f725f616464726573737834636f736d6f7376616c6f706572317a6b757072383368727a6b6e33757035656c6b747a63713374756674386e78736d77647167706a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416c636f6273507a66544e56653775714141736e6445724a416a71706c6e79756461474230662b522b703346697369676e617475726578586a34577055566f6847494861362f733062437675796a71317774514771624f7451437a39327150516a6973544e3434547a2b2b4f7a78316c41503646304d342b6554413033586572715138685a436541664c2f336e773d3d647479706572636f736d6f732d73646b2f4d736753656e64", hex(output.encoded()));

    /*
        the sample tx on testnet
        https://hubble.figment.network/chains/gaia-13003/blocks/125922/transactions/AAE5E18516DC8B0EF864F91B9531AB63B2248E3FC9058B6A330AE79EF1B4120A?format=json
    */
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
    auto signatureInBase64 = signer.signInBase64();

    ASSERT_EQ("VG8NZzVvavlM+1qyK5dOSZwzEj8sLCkvTw5kh44Oco9GQxBf13FVC+s/I3HwiICqo4+o8jNMEDp3nx2C0tuY1g==", signatureInBase64);

    auto output = signer.build();

    ASSERT_EQ("{\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"1018\",\"denom\":\"muon\"}],\"gas\":\"101721\"},\"memo\":\"\",\"msg\":[{\"type\":\"cosmos-sdk/MsgWithdrawDelegationReward\",\"value\":{\"delegator_address\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"validator_address\":\"cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"VG8NZzVvavlM+1qyK5dOSZwzEj8sLCkvTw5kh44Oco9GQxBf13FVC+s/I3HwiICqo4+o8jNMEDp3nx2C0tuY1g==\"}],\"type\":\"cosmos-sdk/MsgSend\"}}", output.json());

    ASSERT_EQ("a1627478a563666565a266616d6f756e7481a266616d6f756e7464313031386564656e6f6d646d756f6e6367617366313031373231646d656d6f60636d736781a264747970657826636f736d6f732d73646b2f4d7367576974686472617744656c65676174696f6e5265776172646576616c7565a27164656c656761746f725f61646472657373782d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b7967783065706836646430327176616c696461746f725f616464726573737834636f736d6f7376616c6f706572317a6b757072383368727a6b6e33757035656c6b747a63713374756674386e78736d77647167706a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416c636f6273507a66544e56653775714141736e6445724a416a71706c6e79756461474230662b522b703346697369676e617475726578585647384e5a7a567661766c4d2b3171794b35644f535a777a456a38734c436b765477356b6834344f636f39475178426631334656432b732f49334877694943716f342b6f386a4e4d454470336e7832433074755931673d3d647479706572636f736d6f732d73646b2f4d736753656e64", hex(output.encoded()));

    /*
        the sample tx on testnet
        https://hubble.figment.network/chains/gaia-13003/blocks/125922/transactions/AAE5E18516DC8B0EF864F91B9531AB63B2248E3FC9058B6A330AE79EF1B4120A?format=json
    */
}

}
