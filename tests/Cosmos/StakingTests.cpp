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

}
